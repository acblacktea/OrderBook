#include <initializer_list>
#include <utility>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <map>

int main() {
    auto list = std::list<int>{1, 2, 3};
    auto it = ++list.begin();
    cout << *it << endl;
}
