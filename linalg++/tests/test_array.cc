// -*- coding: utf-8 -*-

// $Id:$

#include "gtest/gtest.h"
#include "../array.h"
#include "../rational.h"

typedef Rational<int>  Tipus;
typedef Array<Tipus>   Matriu;


static void EXPECT_ARRAY_EQ(Matriu m, Tipus *a) {
	for (int i = 0; i < m.get_num_rows(); i++) {
		for (int j = 0; j < m.get_num_cols(); j++) {
			EXPECT_EQ(m.get(i, j), *a++);
		}
	}
}

// Constructors

TEST(ArrayConstructorTest, empty_array) {
	Matriu m;

	EXPECT_EQ(m.get_num_rows(), 0);
	EXPECT_EQ(m.get_num_cols(), 0);
}

TEST(ArrayConstructorTest, copy_constructor_copies_ok) {
	Tipus e[3][5] = {{ 1, -2,  3, -4,  5},
					 {-1,  2, -3,  4, -5},
					 { 6, -6,  7, -7,  8}};
	Matriu m1(3, 5, (Tipus *)e);
	Matriu m2(m1);

	EXPECT_EQ(3, m2.get_num_rows());
	EXPECT_EQ(5, m2.get_num_cols());
	EXPECT_ARRAY_EQ(m2, (Tipus*)e);
}

TEST(ArrayConstructorTest, copy_constructor_creates_duplicated) {
	Tipus e[3][5] = {{ 1, -2,  3, -4,  5},
					 {-1,  2, -3,  4, -5},
					 { 6, -6,  7, -7,  8}};
	Matriu m1(3, 5, (Tipus *)e);
	Matriu m2(m1);

	m1.put(1, 1, 42);
	EXPECT_EQ(m2.get(1, 1), 2);
}

TEST(ArrayConstructorTest, copy_from_c_array) {
	Tipus e[3][5] = {{ 1, -2,  3, -4,  5},
					 {-1,  2, -3,  4, -5},
					 { 6, -6,  7, -7,  8}};
	Matriu m(3, 5, (Tipus *)e);

	EXPECT_EQ(3, m.get_num_rows());
	EXPECT_EQ(5, m.get_num_cols());
	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

TEST(ArrayConstructorTest, identity) {
	Tipus e[3][3] = {{1, 0, 0},
					 {0, 1, 0},
					 {0, 0, 1}};
	Matriu m = Matriu::identity(3);

	EXPECT_EQ(3, m.get_num_rows());
	EXPECT_EQ(3, m.get_num_cols());
	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

// operadors

TEST(ArrayOperatorTest, assignation_copies_ok) {
	Tipus a[2][3] = {{ 1,  5,  7},
					 {-2, -7, -5}};
	Matriu m(2, 3, (Tipus*)a);
	Matriu r;

	r = m;

	EXPECT_EQ(2, r.get_num_rows());
	EXPECT_EQ(3, r.get_num_cols());
	EXPECT_ARRAY_EQ(r, (Tipus*)a);
}

TEST(ArrayOperatorTest, assignation_creates_copy) {
	Tipus a[2][3] = {{ 1,  5,  7},
					 {-2, -7, -5}};
	Matriu m(2, 3, (Tipus*)a);
	Matriu r;

	r = m;

	m.put(0, 1, 42);
	EXPECT_EQ(r.get(0, 1), 5);
}

// Gauss

TEST(ArrayGaussTest, simple) {
	Tipus a[2][3] = {{ 1,  5,  7},
					 {-2, -7, -5}};
	Tipus e[2][3] = {{1, 5, 7},
					 {0, 3, 9}};
	Matriu m(2, 3, (Tipus*)a);
	int sign;

	sign = m.gauss(m.TRIANGULARIZE);
	EXPECT_EQ(sign, 1);
	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

TEST(ArrayGaussTest, row_swap) {
	Tipus a[3][4] = {{ 0,  2,  1, -8},
					 { 1, -2, -3,  0},
					 {-1,  1,  2,  3}};
	Tipus e[3][4] = {{1, -2,    -3,     0},
					 {0,  2,     1,    -8},
					 {0,  0, Tipus(-1, 2), -1}};
	Matriu m(3, 4, (Tipus*)a);
	int sign;

	sign = m.gauss(m.TRIANGULARIZE);
	EXPECT_EQ(sign, -1);
	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

TEST(ArrayGaussTest, test_zeros_last_row) {
	Tipus a[3][4] = {{1, -2,  -6,  12},
					 {2,  4,  12, -17},
					 {1, -4, -12,  22}};
	Matriu m(3, 4, (Tipus*)a);
	int sign;

	sign = m.gauss(m.TRIANGULARIZE);
	EXPECT_EQ(sign, 0);
}

TEST(ArrayGaussTest, test_diagonalize) {
	Tipus a[3][4] = {{ 0,  2,  1, -8},
					 { 1, -2, -3,  0},
					 {-1,  1,  2,  3}};
	Tipus e[3][4] = {{1, 0,        0,      -4},
					 {0, 2,        0,     -10},
					 {0, 0, Tipus(-1, 2),  -1}};
	Matriu m(3, 4, (Tipus*)a);
	int sign;

	sign = m.gauss(m.DIAGONALIZE);
	EXPECT_EQ(sign, -1);
	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

TEST(ArrayGaussTest, test_normalitze) {
	Tipus a[3][4] = {{ 0,  2,  1, -8},
					 { 1, -2, -3,  0},
					 {-1,  1,  2,  3}};
	Tipus e[3][4] = {{1, 0, 0, -4},
					 {0, 1, 0, -5},
					 {0, 0, 1,  2}};
	Matriu m(3, 4, (Tipus*)a);
	int sign;

	sign = m.gauss(m.NORMALIZE);
	EXPECT_EQ(sign, -1);
	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

// determinant

TEST(ArrayDeterminantTest, simple) {
	Tipus a[3][3] = {{1, 2, 3},
					 {0, 1, 4},
					 {5, 6, 0}};
	Matriu m(3, 3, (Tipus*)a);

	EXPECT_EQ(m.det(), 1);
}

TEST(ArrayDeterminantTest, test_zero_determinant) {
	Tipus a[3][3] = {{1, 2, 3},
					 {4, 5, 6},
					 {3, 3, 3}};
	Matriu m(3, 3, (Tipus*)a);

	EXPECT_EQ(m.det(), 0);
}

TEST(ArrayDeterminantTest, test_matrix_overrun) {
	Tipus a[6][6] = {{1, 0, 0, 0, 0, 0},
					 {0, 1, 0, 0, 0, 0},
					 {1, 1, 0, 0, 0, 0},
					 {0, 0, 1, 0, 0, 0},
					 {1, 0, 1, 0, 0, 0},
					 {0, 0, 0, 1, 1, 1}};
	Matriu m(6, 6, (Tipus*)a);

	EXPECT_EQ(m.det(), 0);
}



// inversa

TEST(ArrayInvertTest, invert) {
	Tipus a[3][3] = {{1, 2, 3},
					 {0, 1, 4},
					 {5, 6, 0}};
	Tipus e[3][3] = {{-24,  18,  5},
					 { 20, -15, -4},
					 { -5,   4,  1}};
	Matriu m(3, 3, (Tipus*)a);

	m.inv();

	EXPECT_ARRAY_EQ(m, (Tipus*)e);
}

TEST(ArrayInvertTest, invert_with_temp_storage) {
	Tipus a[3][3] = {{1, 2, 3},
					 {0, 1, 4},
					 {5, 6, 0}};
	Tipus e[3][6] = {{1, 0, 0, -24,  18,  5},
					 {0, 1, 0,  20, -15, -4},
					 {0, 0, 1,  -5,   4,  1}};
	Matriu m(3, 3, (Tipus*)a);
	Matriu t(3, 6);

	m.inv(t);

	EXPECT_ARRAY_EQ(t, (Tipus*)e);
}

// TEST(ArrayInvertTest, is_left_inverse) {
// 	Tipus a[3][3] = {{1, 2, 3},
// 					 {0, 1, 4},
// 					 {5, 6, 0}};
// 	Tipus e[3][3] = {{1, 0, 0},
// 					 {0, 1, 0},
// 					 {0, 0, 1}};
// 	Matriu m(3, 3, (Tipus*)a);
// 	Matriu n(m);
// 	Matriu r;

// 	r = m.dot(n.inv());

// 	EXPECT_ARRAY_EQ(r, (Tipus*)e);
// }

// TEST(ArrayInvertTest, is_right_inverse) {
// 	Tipus a[3][3] = {{1, 2, 3},
// 					 {0, 1, 4},
// 					 {5, 6, 0}};
// 	Tipus e[3][3] = {{1, 0, 0},
// 					 {0, 1, 0},
// 					 {0, 0, 1}};
// 	Matriu m(3, 3, (Tipus*)a);
// 	Matriu n(m);
// 	Matriu r;

// 	r = n.dot(m.inv());

// 	EXPECT_ARRAY_EQ(r, (Tipus*)e);
// }

TEST(ArrayInvertTest, singular_array_throws_exception) {
	Tipus a[3][3] = {{1, 2, 3},
					 {4, 5, 6},
					 {3, 3, 3}};
	Matriu m(3, 3, (Tipus*)a);

	ASSERT_THROW(m.inv(), ArrayException);
}

TEST(ArrayInvertTest, test_error_left_array_really_normalized) {
	Tipus a[2][2] = {{1, 2},
					 {3, 4}};
	Tipus e[2][2] = {{   -2,           1},
					 {Tipus(3, 2), Tipus(-1, 2)}};
	Matriu m(2, 2, (Tipus*)a);

	m.inv();

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
}

// producte

// TEST(ArrayDotTest, product_ok) {
// 	Tipus a1[2][3] = {{1, 2, 3},
// 					  {4, 5, 6}};
// 	Tipus a2[3][2] = {{ 7,  8},
// 					  { 9, 10},
// 					  {11, 12}};
// 	Tipus e[2][2] = {{ 58,  64},
// 					 {139, 154}};
// 	Matriu m1(2, 3, (Tipus*)a1);
// 	Matriu m2(3, 2, (Tipus*)a2);
// 	Matriu r;

// 	r = m1.dot(m2);

// 	EXPECT_EQ(2, r.get_num_rows());
// 	EXPECT_EQ(2, r.get_num_cols());
// 	EXPECT_ARRAY_EQ(r, (Tipus*)e);
// }

// TEST(ArrayDotTest, incompatible_arrays_throws_exception) {
// 	Tipus a1[2][3] = {{1, 2, 3},
// 					  {4, 5, 6}};
// 	Tipus a2[2][2] = {{ 7,  8},
// 					  { 9, 10}};
// 	Matriu m1(2, 3, (Tipus*)a1);
// 	Matriu m2(2, 2, (Tipus*)a2);

// 	ASSERT_THROW(m1.dot(m2), ArrayException);
// }

// TEST(ArrayDotTest, product_does_not_modifies_operands) {
// 	Tipus a1[2][3] = {{1, 2, 3},
// 					  {4, 5, 6}};
// 	Tipus a2[3][2] = {{ 7,  8},
// 					  { 9, 10},
// 					  {11, 12}};
// 	Matriu m1(2, 3, (Tipus*)a1);
// 	Matriu m2(3, 2, (Tipus*)a2);
// 	Matriu r;

// 	r = m1.dot(m2);

// 	EXPECT_ARRAY_EQ(m1, (Tipus*)a1);
// 	EXPECT_ARRAY_EQ(m2, (Tipus*)a2);
// }

// transposta

// TEST(ArrayTranposeTest, transpose_square_array) {
// 	Tipus a[2][2] = {{1, 2},
// 					 {3, 4}};
// 	Tipus e[2][2] = {{1, 3},
// 					 {2, 4}};
// 	Matriu m(2, 2, (Tipus*)a);
// 	Matriu r;

// 	r = m.t();

// 	EXPECT_EQ(2, r.get_num_rows());
// 	EXPECT_EQ(2, r.get_num_cols());
// 	EXPECT_ARRAY_EQ(r, (Tipus*)e);
// }

// TEST(ArrayTranposeTest, transpose_rectangular_array) {
// 	Tipus a[2][3] = {{1, 2, 3},
// 					 {4, 5, 6}};
// 	Tipus e[3][2] = {{1, 4},
// 					 {2, 5},
// 					 {3, 6}};
// 	Matriu m(2, 3, (Tipus*)a);
// 	Matriu r;

// 	r = m.t();

// 	EXPECT_EQ(3, r.get_num_rows());
// 	EXPECT_EQ(2, r.get_num_cols());
// 	EXPECT_ARRAY_EQ(r, (Tipus*)e);
// }
