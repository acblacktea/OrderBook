#include <initializer_list>
#include <utility>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
class c {
public:
    c(int c, int d): a(c), b(d){};

    int a, b;
};

class ob {
public:
   ob() {
       std::cout << "ob" << std::endl;
   }

   ob(const ob& o) {
       std::cout << "ob& " << std::endl;
   }

   ob(ob&&) {
       std::cout << "ob&& " << std::endl;
   }

};

int main() {
    auto pt = std::make_unique<ob>(ob());
    return 0;
}
