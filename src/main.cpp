#include <iostream>

#include "lazy_matrix.hpp"

int main() {
    yLAB::LazyMatrix<int> a {3, 3, 0};

    for (auto&& val : a) {
        std::cout << val << std::endl;
    }

    for (auto i = 0; i < 3; ++i) {
        std::cout << a[i][i] << std::endl;
    }
    auto b = a[1][1];
}

