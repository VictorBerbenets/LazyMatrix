#include <gtest/gtest.h>

#include "lazy_matrix.hpp"

using namespace yLAB;

TEST(LazyMatrix, Addition1) {
    LazyMatrix<long long> m1 {2, 2, {1, 2, 3, 4}};
    LazyMatrix<long long> m2 {2, 2, {2, 4, 6, 8}};
    LazyMatrix<long long> ans {2, 2, {3, 6, 9, 12}};

    ASSERT_EQ(m1 + m2, ans);
}

TEST(LazyMatrix, Addition2) {
    LazyMatrix<long long> m1 {1, 1, 100};
    LazyMatrix<long long> m2 {1, 1, 5};
    LazyMatrix<long long> ans {1, 1, 105};

    ASSERT_EQ(m1 + m2, ans);
}

TEST(LazyMatrix, Addition3) {
    LazyMatrix<long long> m1 {1000, 1000, 0};
    LazyMatrix<long long> m2 {1000, 1000, 1000};
    LazyMatrix<long long> ans {1000, 1000, 1000};

    ASSERT_EQ(m1 + m2, ans);
}

TEST(LazyMatrix, Addition4) {
    LazyMatrix<long long> m1 {3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9}};
    LazyMatrix<long long> m2 {3, 3, 0};
    LazyMatrix<long long> ans {3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9}};

    ASSERT_EQ(m1 + m2, ans);
}

TEST(LazyMatrix, Addition5) {
    LazyMatrix<long long> m1 {5, 5, 1};
    LazyMatrix<long long> m2 {5, 5, 1};
    LazyMatrix<long long> ans {5, 5, 2};

    ASSERT_EQ(m1 + m2, ans);
}


TEST(LazyMatrix, Substraction1) {
    LazyMatrix<long long> m1 {1, 1, 100};
    LazyMatrix<long long> m2 {1, 1, 5};
    LazyMatrix<long long> ans {1, 1, 95};
    
    ASSERT_EQ(m1 - m2, ans);
}

TEST(LazyMatrix, Substraction2) {
    LazyMatrix<long long> m1 {50, 50, 100};
    LazyMatrix<long long> m2 {50, 50, 100};
    LazyMatrix<long long> ans {50, 50, 0};
    
    ASSERT_EQ(m1 - m2, ans);
}

TEST(LazyMatrix, Substraction3) {
    LazyMatrix<long long> m1 {1, 1, 5};
    LazyMatrix<long long> m2 {1, 1, 5};
    LazyMatrix<long long> ans {1, 1, 0};
    
    ASSERT_EQ(m1 - m2, ans);
}

TEST(LazyMatrix, Substraction4) {
    LazyMatrix<long long> m1 {100, 100, -10};
    LazyMatrix<long long> m2 {100, 100, 500};
    LazyMatrix<long long> ans {100, 100, -510};
    
    ASSERT_EQ(m1 - m2, ans);
}

TEST(LazyMatrix, Substraction5) {
    LazyMatrix<long long> m1 {3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1}};
    LazyMatrix<long long> m2 {3, 3, 0};
    LazyMatrix<long long> ans {3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1}};
    
    ASSERT_EQ(m1 - m2, ans);
}

TEST(LazyMatrix, ScalarMul1) {
    LazyMatrix<long long> m1 {3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1}};
    long long coeff = 0;
    LazyMatrix<long long> ans {3, 3, 0};

    ASSERT_EQ(m1 *= coeff, ans);
}

TEST(LazyMatrix, ScalarMul2) {
    LazyMatrix<long long> m1 {3, 3, {-1, 0, 0, 0, -1, 0, 0, 0, -1}};
    long long coeff = -10;
    LazyMatrix<long long> ans {3, 3, {10, 0, 0, 0, 10, 0, 0, 0, 10}};

    ASSERT_EQ(m1 * coeff, ans);
}

TEST(LazyMatrix, ScalarMul3) {
    LazyMatrix<long long> m1 {3, 3, 0};
    long long coeff = 100000;
    LazyMatrix<long long> ans {3, 3, 0};

    ASSERT_EQ(coeff * m1, ans);
}

TEST(LazyMatrix, ScalarMul4) {
    LazyMatrix<long long> m1 {1, 1, 500};
    long long coeff = 10;
    LazyMatrix<long long> ans {1, 1, 5000};

    ASSERT_EQ(m1 *=coeff, ans);
}

TEST(LazyMatrix, ScalarMul5) {
    LazyMatrix<long long> m1 {3, 3, 0};
    long long coeff = 0;
    LazyMatrix<long long> ans {3, 3, 0};

    ASSERT_EQ(coeff * m1, ans);
}

TEST(LazyMatrix, ScalarDivision1) {
    LazyMatrix<long long> m1 {3, 3, 0};
    long long coeff = -100;
    LazyMatrix<long long> ans {3, 3, 0};

    ASSERT_EQ(m1 /= coeff, ans);
}

TEST(LazyMatrix, ScalarDivision2) {
    LazyMatrix<long long> m1 {3, 3, 3};
    long long coeff = 3;
    LazyMatrix<long long> ans {3, 3, 1};

    ASSERT_EQ(m1 / coeff, ans);
}

TEST(LazyMatrix, ScalarDivision3) {
    LazyMatrix<long long> m1 {1, 1, 100 };
    long long coeff = -100;
    LazyMatrix<long long> ans {1, 1, -1};

    ASSERT_EQ(m1 / coeff, ans);
}

TEST(LazyMatrix, ScalarDivision4) {
    LazyMatrix<long long> m1 {500, 500, 1000000};
    long long coeff = -10000;
    LazyMatrix<long long> ans {500, 500, -100};

    ASSERT_EQ(m1 /= coeff, ans);
}

TEST(LazyMatrix, ScalarDivision5) {
    LazyMatrix<long long> m1 {500, 500, 0};
    long long coeff = -10000000;
    LazyMatrix<long long> ans {500, 500, 0};

    ASSERT_EQ(m1 / coeff, ans);
}

