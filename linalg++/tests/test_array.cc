// -*- coding: utf-8 -*-

// $Id:$

#include "gtest/gtest.h"
#include "../array.h"
#include "../rational.h"

typedef Rational<int>  Tipus;
typedef Array<Tipus>   Matriu;


// Constructors

TEST(ArrayConstructorTest, empty_array) {
	Matriu m;

	EXPECT_EQ(m.get_num_rows(), 0);
	EXPECT_EQ(m.get_num_cols(), 0);
}

TEST(ArrayConstructorTest, fill_array) {
	Matriu m(3, 5);

	EXPECT_EQ(3, m.get_num_rows());
	EXPECT_EQ(5, m.get_num_cols());
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			EXPECT_EQ(m.get(i, j), 0);
		}
	}
}

TEST(ArrayConstructorTest, copy_constructor_copies_ok) {
	Tipus e[3][5] = {{ 1, -2,  3, -4,  5},
					 {-1,  2, -3,  4, -5},
					 { 6, -6,  7, -7,  8}};
	Matriu m1(3, 5, (Tipus *)e);
	Matriu m2(m1);

	EXPECT_EQ(3, m2.get_num_rows());
	EXPECT_EQ(5, m2.get_num_cols());
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			EXPECT_EQ(m2.get(i, j), e[i][j]);
		}
	}
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
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
}

TEST(ArrayConstructorTest, copy_from_vector_vector) {
	Tipus e[3][5] = {{ 1, -2,  3, -4,  5},
					 {-1,  2, -3,  4, -5},
					 { 6, -6,  7, -7,  8}};
	std::vector< std::vector< Tipus > > v(3);
	v[0] = std::vector<Tipus>(e[0], e[0] + 5);
	v[1] = std::vector<Tipus>(e[1], e[1] + 5);
	v[2] = std::vector<Tipus>(e[2], e[2] + 5);
	Matriu m(v);

	EXPECT_EQ(3, m.get_num_rows());
	EXPECT_EQ(5, m.get_num_cols());
	for (int i = 0; i < 3; i++) {
		std::vector<Tipus> fila = v[i];
		for (int j = 0; j < 5; j++) {
			EXPECT_EQ(m.get(i, j), fila[j]);
		}
	}
}

TEST(ArrayConstructorTest, copy_from_two_array) {
	Tipus e[3][5] = {{ 1, -2,  3, -4,  5},
					 {-1,  2, -3,  4, -5},
					 { 6, -6,  7, -7,  8}};
	Tipus e1[3][3] = {{ 1, -2,  3},
					  {-1,  2, -3},
					  { 6, -6,  7}};
	Tipus e2[3][2] = {{-4,  5},
					  { 4, -5},
					  {-7,  8}};
	Matriu a1(3, 3, (Tipus*)e1);
	Matriu a2(3, 2, (Tipus*)e2);
	Matriu m(a1, a2);

	EXPECT_EQ(3, m.get_num_rows());
	EXPECT_EQ(5, m.get_num_cols());
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
}

TEST(ArrayConstructorTest, identity) {
	Tipus e[3][3] = {{1, 0, 0},
					 {0, 1, 0},
					 {0, 0, 1}};
	Matriu m(3);

	EXPECT_EQ(3, m.get_num_rows());
	EXPECT_EQ(3, m.get_num_cols());
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
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
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			EXPECT_EQ(r.get(i, j), a[i][j]);
		}
	}

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
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
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
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
}

TEST(ArrayGaussTest, test_zeros_a_la_ultima_fila) {
	Tipus a[3][4] = {{1, -2,  -6,  12},
					 {2,  4,  12, -17},
					 {1, -4, -12,  22}};
	Matriu m(3, 4, (Tipus*)a);
	int sign;

	sign = m.gauss(m.TRIANGULARIZE);
	EXPECT_EQ(sign, 0);
}

TEST(ArrayGaussTest, test_diagonalitzar) {
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
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
}

TEST(ArrayGaussTest, test_normalitzar) {
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
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			EXPECT_EQ(m.get(i, j), e[i][j]);
		}
	}
}

// determinant

TEST(TestDeterminantTest, simple) {
	Tipus a[3][3] = {{1, 2, 3},
					 {0, 1, 4},
					 {5, 6, 0}};
	Matriu m(3, 3, (Tipus*)a);

	EXPECT_EQ(m.det(), 1);
}

TEST(TestDeterminantTest, test_determinant_zero) {
	Tipus a[3][3] = {{1, 2, 3},
					 {4, 5, 6},
					 {3, 3, 3}};
	Matriu m(3, 3, (Tipus*)a);

	EXPECT_EQ(m.det(), 0);
}
