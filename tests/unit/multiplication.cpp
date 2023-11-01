#include <gtest/gtest.h>

#include "lazy_matrix.hpp"

using namespace yLAB;

TEST(LazyMatrix, Multiplication1) {
    LazyMatrix<int> m1 {2, 2, {4, 2, 9, 0}};
    LazyMatrix<int> m2 {2, 2, {3, 1, -3, 4}};
    LazyMatrix<int> ans {2, 2, {6, 12, 27, 9}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication2) {
    LazyMatrix<int> m1 {3, 2, {2, 1, -3, 0, 4, -1}};
    LazyMatrix<int> m2 {2, 3, {5, -1, 6, -3, 0, 7}};
    LazyMatrix<int> ans {3, 3, {7, -2, 19, -15, 3, -18, 23, -4, 17}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication3) {
    LazyMatrix<int> m1 {3, 3, {2, -1, 3, 4, 2, 0, -1, 1, 1}};
    LazyMatrix<int> m2 {3, 1, {1, 2, -1}};
    LazyMatrix<int> ans {3, 1, {-3, 8, 0}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication4) {
    LazyMatrix<int> m1 {3, 1, {1, 2, 3}};
    LazyMatrix<int> m2 {1, 3, {-2, -3, 4}};
    LazyMatrix<int> ans {3, 3, {-2, -3, 4, -4, -6, 8, -6, -9, 12}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication5) {
    LazyMatrix<int> m1 {1, 3, {-2, -3, 4}};
    LazyMatrix<int> m2 {3, 1, {1, 2, 3}};
    LazyMatrix<int> ans {1, 1, {4}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication6) {
    LazyMatrix<int> m1 {2, 2, {3, 5, 2, 1}};
    LazyMatrix<int> m2 {2, 3, {8, 2, 3, 1, 7, 2}};
    LazyMatrix<int> ans {2, 3, {29, 41, 19, 17, 11, 8}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication7) {
    LazyMatrix<int> m1 {3, 3, {1, 4, 3, 2, 1, 5, 3, 2, 1}};
    LazyMatrix<int> m2 {3, 3, {5, 2, 1, 4, 3, 2, 2, 1, 5}};
    LazyMatrix<int> ans {3, 3, {27, 17, 24, 24, 12, 29, 25, 13, 12}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication8) {
    LazyMatrix<int> m1 {2, 2, {100, 101, 102, 103}};
    LazyMatrix<int> m2 {2, 2, {1, 0, 0, 1}};
    LazyMatrix<int> ans {2, 2, {100, 101, 102, 103}};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication9) {
    LazyMatrix<int> m1 {4, 4, {1, 0, 2, -1, -2, 0, -4, 2, 1, 0, 2, -1, 3, 0, 6, -3}};
    LazyMatrix<int> m2 {4, 4, {2, 1, 3, -1, -4, -2, -6, 2, 2, 1, 3, -1, 6, 3, 9, -3}};
    LazyMatrix<int> ans {4, 4, 0};

    ASSERT_EQ(ans, m1 * m2);
}

TEST(LazyMatrix, Multiplication10) {
    LazyMatrix<int> m1 {4, 4, {1, 0, 2, -1, -2, 0, -4, 2, 1, 0, 2, -1, 3, 0, 6, -3}};
    LazyMatrix<int> m2 {4, 4, 0};
    LazyMatrix<int> ans {4, 4, 0};

    ASSERT_EQ(ans, m1 * m2);
}
