# -*- coding: utf-8 -*-

# $Id$

import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import unittest

import linalg
from rat import Fraccio as F

class TestArray(unittest.TestCase):
    def assertArrayEqual(self, real, expected):
        self.assertEqual(real.num_files, len(expected))
        self.assertEqual(real.num_columnes, len(expected[0]))
        for i, fila in enumerate(expected):
            for j, item in enumerate(fila):
                self.assertEqual(real[i, j], item)


class TestPseudogauss(TestArray):

    def assertGauss(self, m, e, s=None, diagonal=False, normalitzar=False):
        a = linalg.array(m, F)
        s2, r = a._pseudogauss(diagonal=diagonal, normalitzar=normalitzar)
        if e is None:
            self.assert_(r is None)
        else:
            self.assertArrayEqual(a, e)
        if s is not None:
            self.assertEqual(s, s2)

    def test_senzill(self):
        m = [[ 1,  5,  7],
             [-2, -7, -5]]
        e = [[1, 5, 7],
             [0, 3, 9]]
        self.assertGauss(m, e, 1)

    def test_intercanvi_files(self):
        m = [[ 0,  2,  1, -8],
             [ 1, -2, -3,  0],
             [-1,  1,  2,  3]]
        e = [[1, -2,    -3,     0],
             [0,  2,     1,    -8],
             [0,  0, F(-1, 2), -1]]
        self.assertGauss(m, e, -1)

    def test_zeros_a_la_ultima_fila(self):
        m = [[1, -2,  -6,  12],
             [2,  4,  12, -17],
             [1, -4, -12,  22]]
        self.assertGauss(m, None, 0)

    def test_diagonalitzar(self):
        m = [[ 0,  2,  1, -8],
             [ 1, -2, -3,  0],
             [-1,  1,  2,  3]]
        e = [[1, 0,    0,      -4],
             [0, 2,    0,     -10],
             [0, 0, F(-1, 2),  -1]]
        self.assertGauss(m, e, -1, diagonal=True)

    def test_normalitzar(self):
        m = [[ 0,  2,  1, -8],
             [ 1, -2, -3,  0],
             [-1,  1,  2,  3]]
        e = [[1, 0, 0, -4],
             [0, 1, 0, -5],
             [0, 0, 1,  2]]
        self.assertGauss(m, e, -1, diagonal=True, normalitzar=True)


class TestProducte(TestArray):

    def test_array_x_array(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [[ 7,  8],
              [ 9, 10],
              [11, 12]]
        e = [[ 58,  64],
             [139, 154]]
        a1 = linalg.array(m1)
        a2 = linalg.array(m2)
        r = a1.dot(a2)
        self.assertArrayEqual(r, e)

    def test_array_x_matriu_python(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [[ 7,  8],
              [ 9, 10],
              [11, 12]]
        e = [[ 58,  64],
             [139, 154]]
        a1 = linalg.array(m1)
        r = a1.dot(m2)
        self.assertArrayEqual(r, e)

    def test_array_x_vector_python_per_la_dreta(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [7, 8, 9]
        e = [[ 50],
             [122]]
        a1 = linalg.array(m1)
        r = a1.dot(m2)
        self.assertArrayEqual(r, e)

    def test_array_x_vector_python_per_la_esquerra(self):
        m1 = [7, 8]
        m2 = [[1, 2, 3],
              [4, 5, 6]]
        e = [[39, 54, 69]]
        a1 = linalg.array(m1)
        r = a1.dot(m2)
        self.assertArrayEqual(r, e)

    def test_array_x_array_incompatibles(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [[ 7,  8],
              [ 9, 10]]
        a1 = linalg.array(m1)
        a2 = linalg.array(m2)
        with self.assertRaises(ValueError):
            r = a1.dot(a2)

    def test_array_x_matriu_python_incompatibles(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [[ 7,  8],
              [ 9, 10]]
        a1 = linalg.array(m1)
        with self.assertRaises(ValueError):
            r = a1.dot(m2)

    def test_array_x_vector_python_incompatibles_per_la_dreta(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [7, 8]
        a1 = linalg.array(m1)
        with self.assertRaises(ValueError):
            r = a1.dot(m2)

    def test_array_x_vector_python_incompatible_per_la_esquerra(self):
        m1 = [7, 8, 9]
        m2 = [[1, 2, 3],
              [4, 5, 6]]
        a1 = linalg.array(m1)
        with self.assertRaises(ValueError):
            r = a1.dot(m2)

    def test_product_no_altera_operands_originals(self):
        m1 = [[1, 2, 3],
              [4, 5, 6]]
        m2 = [[ 7,  8],
              [ 9, 10],
              [11, 12]]
        e = [[ 58,  64],
             [139, 154]]
        a1 = linalg.array(m1)
        a2 = linalg.array(m2)
        r = a1.dot(a2)
        self.assertArrayEqual(a1, m1)
        self.assertArrayEqual(a2, m2)


class TestInversa(TestArray):

    def test_inversa(self):
        m = [[1, 2, 3],
             [0, 1, 4],
             [5, 6, 0]]
        e = [[-24,  18,  5],
             [ 20, -15, -4],
             [ -5,   4,  1]]
        a = linalg.array(m)
        self.assertArrayEqual(a.inv(), e)

    def test_no_invertible_retorna_None(self):
        m = [[1, 2, 3],
             [4, 5, 6],
             [3, 3, 3]]
        a = linalg.array(m)
        self.assert_(a.inv() is None)

    def test_inversa_no_altera_array_original(self):
        m = [[1, 2, 3],
             [0, 1, 4],
             [5, 6, 0]]
        a = linalg.array(m)
        r = a.inv()
        self.assertArrayEqual(a, m)

    def test_error_matriu_esquerra_no_normalitzada(self):
        m = [[1, 2],
             [3, 4]]
        e = [[-2,         1],
             [F(3, 2), F(-1, 2)]]
        a = linalg.array(m)
        self.assertArrayEqual(a.inv(), e)


class TestDeterminant(TestArray):

    def test_determinant(self):
        m = [[1, 2, 3],
             [0, 1, 4],
             [5, 6, 0]]
        a = linalg.array(m)
        self.assertEqual(a.det(), 1)

    def test_determinant_zero(self):
        m = [[1, 2, 3],
             [4, 5, 6],
             [3, 3, 3]]
        a = linalg.array(m)
        self.assertEqual(a.det(), 0)

    def test_determinant_no_altera_array_original(self):
        m = [[1, 2, 3],
             [0, 1, 4],
             [5, 6, 0]]
        a = linalg.array(m)
        r = a.det()
        self.assertArrayEqual(a, m)
