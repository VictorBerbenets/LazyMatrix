#include <iostream>

#include "lazy_matrix.hpp"

int main() {
    yLAB::LazyMatrix<int> a {3, 3, 0};

    for (auto&& val : a) {
        std::cout << val << std::endl;
    }
}

