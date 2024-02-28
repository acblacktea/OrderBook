#pragma once
#include <assert.h>             // assert
#include <atomic>               // std::atomic
#include <memory>               // std::addressof/allocator/allocator_traits
#include <new>                  // std::bad_alloc
#include <type_traits>          // std::integral_constant/false_type/true_type
#include <utility>              // std::move/swap


namespace detail {

    template <typename _Alloc>
    inline void swap_allocator(_Alloc&, _Alloc&, std::false_type) noexcept
    {
    }

    template <typename _Alloc>
    inline void swap_allocator(_Alloc& lhs, _Alloc& rhs, std::true_type) noexcept
    {
        using std::swap;
        swap(lhs, rhs);
    }

} /* namespace detail */

template <class _Tp, class _Alloc = std::allocator<_Tp>>
class fc_queue : private _Alloc {
public:
    typedef _Tp                                       value_type;
    typedef _Alloc                                    allocator_type;
    typedef std::allocator_traits<_Alloc>             allocator_traits;
    typedef typename allocator_traits::size_type      size_type;
    typedef typename allocator_traits::pointer        pointer;
    typedef typename allocator_traits::const_pointer  const_pointer;
    typedef value_type&                               reference;
    typedef const value_type&                         const_reference;
    typedef std::atomic<pointer>                      atomic_pointer;

    fc_queue() = default;
    constexpr explicit fc_queue(const allocator_type& alloc) noexcept
        : allocator_type(alloc)
    {
    }

    explicit fc_queue(size_type             max_size,
                      const allocator_type& alloc = allocator_type())
        : allocator_type(alloc)
    {
        assert(max_size != 0);
        initialize_capacity(max_size);
    }

    fc_queue(const fc_queue& rhs, const allocator_type& alloc)
        : fc_queue(rhs.capacity(), alloc)
    {
        copy_elements(rhs);
    }

    fc_queue(fc_queue&& rhs, const allocator_type& alloc)
        : allocator_type(alloc)
    {
        if (alloc == rhs.get_alloc()) {
            move_container(std::move(rhs));
        } else {
            initialize_capacity(rhs.capacity());
            move_elements(std::move(rhs));
        }
    }

    fc_queue(const fc_queue& rhs)
        : fc_queue(rhs,
                   allocator_traits::select_on_container_copy_construction(
                           rhs.get_alloc()))
    {
    }

    fc_queue(fc_queue&& rhs) noexcept
        : allocator_type(std::move(rhs.get_alloc()))
    {
        move_container(std::move(rhs));
    }

    ~fc_queue()
    {
        clear();
        deallocate();
    }

    bool empty() const noexcept
    {
        return _M_head == _M_tail;
    }

    bool full() const noexcept
    {
        return _M_head == increment(_M_tail);
    }

    size_type capacity() const noexcept
    {
        return _M_end - _M_begin - 1;
    }

    size_type size() const noexcept
    {
        typename allocator_traits::difference_type dist = _M_tail - _M_head;
        if (dist < 0) {
            dist += _M_end - _M_begin;
        }
        return dist;
    }

    reference front()
    {
        assert(!empty());
        return *_M_head;
    }

    const_reference front() const
    {
        assert(!empty());
        return *_M_head;
    }

    reference back()
    {
        assert(!empty());
        return *decrement(_M_tail);
    }

    const_reference back() const
    {
        assert(!empty());
        return *decrement(_M_tail);
    };

    template <typename... _Targs>
    void push(_Targs&&... args)
    {
        assert(capacity() > 0);
        allocator_traits::construct(get_alloc(), std::addressof(*_M_tail),
                                    std::forward<decltype(args)>(args)...);
        if (full()) {
            pop();
        }
        self_increment(_M_tail);
    }

    void pop()
    {
        assert(!empty());
        destroy(std::addressof(*_M_head));
        self_increment(_M_head);
    }

    template <typename... _Targs>
    bool write(_Targs&&... args)
    {
        assert(capacity() > 0);
        auto tail = _M_tail.load(std::memory_order_relaxed);
        auto new_tail = increment(tail);
        if (new_tail == _M_head.load(std::memory_order_acquire)) {
            return false;
        }
        allocator_traits::construct(get_alloc(), std::addressof(*tail),
                                    std::forward<decltype(args)>(args)...);
        _M_tail.store(new_tail, std::memory_order_release);
        return true;
    }

    bool read(reference dest)
    {
        auto head = _M_head.load(std::memory_order_relaxed);
        if (head == _M_tail.load(std::memory_order_acquire)) {
            return false;
        }
        dest = std::move(*head);
        destroy(std::addressof(*head));
        _M_head.store(increment(head), std::memory_order_release);
        return true;
    }

    bool contains(const value_type& value) const
    {
        pointer ptr = _M_head;
        pointer tail = _M_tail;
        while (ptr != tail) {
            if (*ptr == value) {
                return true;
            }
            self_increment(ptr);
        }
        return false;
    }

    void swap(fc_queue& rhs) noexcept
    {
        assert(allocator_traits::propagate_on_container_swap::value ||
               get_alloc() == rhs.get_alloc());
        detail::swap_allocator(
                get_alloc(), rhs.get_alloc(),
                typename allocator_traits::propagate_on_container_swap{});
        swap_pointer(_M_head,  rhs._M_head);
        swap_pointer(_M_tail,  rhs._M_tail);
        swap_pointer(_M_begin, rhs._M_begin);
        swap_pointer(_M_end,   rhs._M_end);
    }

    allocator_type get_allocator() const
    {
        return get_alloc();
    }

private:
    pointer increment(pointer ptr) const noexcept
    {
        ++ptr;
        if (ptr >= _M_end) {
            ptr = _M_begin;
        }
        return ptr;
    }
    pointer decrement(pointer ptr) const noexcept
    {
        if (ptr == _M_begin) {
            ptr = _M_end;
        }
        return --ptr;
    }
    void self_increment(pointer& ptr) const noexcept
    {
        ptr = increment(ptr);
    }
    void self_decrement(pointer& ptr) const noexcept
    {
        ptr = decrement(ptr);
    }
    void self_increment(atomic_pointer& ptr) const noexcept
    {
        ptr = increment(ptr.load(std::memory_order_relaxed));
    }
    void self_decrement(atomic_pointer& ptr) const noexcept
    {
        ptr = decrement(ptr.load(std::memory_order_relaxed));
    }

    void clear() noexcept
    {
        pointer ptr = _M_head;
        pointer tail = _M_tail;
        while (ptr != tail) {
            destroy(std::addressof(*ptr));
            self_increment(ptr);
        }
        _M_head = _M_begin;
        _M_tail = _M_begin;
    }
    void deallocate() noexcept
    {
        if (_M_begin) {
            allocator_traits::deallocate(get_alloc(), _M_begin,
                                         _M_end - _M_begin);
        }
        _M_begin = _M_end = nullptr;
    }
    void destroy(pointer ptr) noexcept
    {
        allocator_traits::destroy(get_alloc(), ptr);
    }

    void initialize_capacity(size_type max_size)
    {
        if (max_size == 0) {
            return;
        }
        if (max_size + 1 == 0) {
            throw std::bad_alloc();
        }
        _M_begin = allocator_traits::allocate(get_alloc(), max_size + 1);
        _M_end = _M_begin + max_size + 1;
        _M_head = _M_begin;
        _M_tail = _M_begin;
    }

    void copy_elements(const fc_queue& rhs)
    {
        pointer ptr = rhs._M_head;
        pointer tail = rhs._M_tail;
        while (ptr != tail) {
            push(*ptr);
            ptr = rhs.increment(ptr);
        }
    }
    void move_elements(fc_queue&& rhs)
    {
        pointer ptr = rhs._M_head;
        pointer tail = rhs._M_tail;
        while (ptr != tail) {
            push(std::move(*ptr));
            ptr = rhs.increment(ptr);
        }
    }
    void move_container(fc_queue&& rhs) noexcept
    {
        _M_head.store(rhs._M_head.load(std::memory_order_relaxed),
                      std::memory_order_relaxed);
        _M_tail.store(rhs._M_tail.load(std::memory_order_relaxed),
                      std::memory_order_relaxed);
        rhs._M_head.store(nullptr, std::memory_order_relaxed);
        rhs._M_tail.store(nullptr, std::memory_order_relaxed);
        _M_begin = rhs._M_begin;
        _M_end = rhs._M_end;
        rhs._M_begin = nullptr;
        rhs._M_end = rhs._M_begin;
    }

    allocator_type& get_alloc() noexcept
    {
        return static_cast<allocator_type&>(*this);
    }
    const allocator_type& get_alloc() const noexcept
    {
        return static_cast<const allocator_type&>(*this);
    }

    static void swap_pointer(pointer& lhs, pointer& rhs) noexcept
    {
        using std::swap;
        swap(lhs, rhs);
    }
    static void swap_pointer(atomic_pointer& lhs,
                             atomic_pointer& rhs) noexcept
    {
        pointer temp = lhs.load(std::memory_order_relaxed);
        lhs.store(rhs.load(std::memory_order_relaxed),
                  std::memory_order_relaxed);
        rhs.store(temp, std::memory_order_relaxed);
    }

    atomic_pointer  _M_head{};
    atomic_pointer  _M_tail{};
    pointer         _M_begin{};
    pointer         _M_end{};
};

template <class _Tp, class _Alloc>
void swap(fc_queue<_Tp, _Alloc>& lhs, fc_queue<_Tp, _Alloc>& rhs) noexcept
{
    lhs.swap(rhs);
}
