#include <iostream>
#include <type_traits>
#include <memory>

#include "lazy_matrix.hpp"

int main() {
    std::vector<int> vec {1, 2, 3, 5};

    yLAB::LazyMatrix<int> a {2, 2, 1000};
    yLAB::LazyMatrix<int> b {2, 2, {100, 2, 3, 5}};
    yLAB::LazyMatrix<int> c {2, 2, vec.begin(), vec.end()};

    auto tmp1 = a;
    auto tmp2 = a;
    auto tmp3 = a;
    auto tmp4 = a;
    tmp4.determinant();
    a.negate();
    auto tmp = a;
    a[1][1] = 5;
    //b += a;
    //tmp += b;
    for (auto&& val : tmp) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
    for (auto&& val : tmp1) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
    for (auto&& val : a) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
    for (auto&& val : b) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
    c.negate();
    for (auto&& val : c) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
    //std::shared_ptr<double[]> sp7 = std::make_shared<double[]>(256, 2.0);
}

