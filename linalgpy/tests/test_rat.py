# -*- coding: utf-8 -*-

# $Id$

import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(__file__)))
import unittest

import rat


class TestRacionals(unittest.TestCase):
    def assertFraccio(self, f, num, den):
        f.simplificar()
        self.assertEqual(f.num, num)
        self.assertEqual(f.den, den)


class TestConstructor(TestRacionals):

    def test_enters(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f, 1, 2)

    def test_fraccio_clona(self):
        f1 = rat.Fraccio(1, 2)
        f2 = rat.Fraccio(f1)
        self.assertEqual(f1, f2)
        self.failIf(f1 is f2)

    def test_denominador_negatiu(self):
        f = rat.Fraccio(1, -2)
        self.assertFraccio(f, -1, 2)


class TestAritmetica(TestRacionals):

    # suma

    def test_suma_enter_zero(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f + 0, 1, 2)

    def test_suma_fraccio_zero(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(0)
        self.assertFraccio(s1 + s2, 1, 2)

    def test_suma_enter_dreta(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f + 2, 5, 2)

    def test_suma_enter_esquerra(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(2 + f, 5, 2)

    def test_suma_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(2, 3)
        self.assertFraccio(s1 + s2, 7, 6)

    def test_autosuma_enter(self):
        f = rat.Fraccio(1, 2)
        f += 2
        self.assertFraccio(f, 5, 2)

    def test_autosuma_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s1 += rat.Fraccio(2, 3)
        self.assertFraccio(s1, 7, 6)

    # resta

    def test_resta_enter_zero(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f - 0, 1, 2)

    def test_resta_fraccio_zero(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(0)
        self.assertFraccio(s1 - s2, 1, 2)

    def test_resta_enter_dreta(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f - 2, -3, 2)

    def test_resta_enter_esquerra(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(2 - f, 3, 2)

    def test_resta_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(2, 3)
        self.assertFraccio(s1 - s2, -1, 6)

    def test_autoresta_enter(self):
        f = rat.Fraccio(1, 2)
        f -= 2
        self.assertFraccio(f, -3, 2)

    def test_autoresta_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s1 -= rat.Fraccio(2, 3)
        self.assertFraccio(s1, -1, 6)

    # multiplicacio

    def test_multiplicacio_enter_zero(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f * 0, 0, 1)

    def test_multiplicacio_fraccio_zero(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(0)
        self.assertFraccio(s1 * s2, 0, 1)

    def test_multiplicacio_enter_un(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f * 1, 1, 2)

    def test_multiplicacio_fraccio_un(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(1)
        self.assertFraccio(s1 * s2, 1, 2)

    def test_multiplicacio_enter_dreta(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f * 3, 3, 2)

    def test_multiplicacio_enter_esquerra(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(3 * f, 3, 2)

    def test_multiplicacio_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(2, 3)
        self.assertFraccio(s1 * s2, 1, 3)

    def test_automultiplicacio_enter(self):
        f = rat.Fraccio(1, 2)
        f *= 3
        self.assertFraccio(f, 3, 2)

    def test_automultiplicacio_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s1 *= rat.Fraccio(2, 3)
        self.assertFraccio(s1, 1, 3)


    # divisio

    def test_divisio_enter_zero(self):
        f = rat.Fraccio(1, 2)
        with self.assertRaises(ZeroDivisionError):
            f / 0

    def test_divisio_fraccio_zero(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(0)
        with self.assertRaises(ZeroDivisionError):
            s1 / s2

    def test_divisio_enter_un(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f / 1, 1, 2)

    def test_divisio_fraccio_un(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(1)
        self.assertFraccio(s1 / s2, 1, 2)

    def test_divisio_enter_dreta(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(f / 3, 1, 6)

    def test_divisio_enter_esquerra(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(3 / f, 6, 1)

    def test_divisio_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(2, 3)
        self.assertFraccio(s1 / s2, 3, 4)

    def test_autodivisio_enter(self):
        f = rat.Fraccio(1, 2)
        f /= 3
        self.assertFraccio(f, 1, 6)

    def test_autodivisio_fraccio(self):
        s1 = rat.Fraccio(1, 2)
        s1 /= rat.Fraccio(2, 3)
        self.assertFraccio(s1, 3, 4)

    def test_divisio_fraccio_negativa(self):
        s1 = rat.Fraccio(1, 2)
        s2 = rat.Fraccio(-2, 3)
        self.assertFraccio(s1 / s2, -3, 4)

    # canvi de signe

    def test_canvi_signe(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(-f, -1, 2)

    def test_doble_canvi_signe(self):
        f = -rat.Fraccio(1, 2)
        self.assertFraccio(-f, 1, 2)

    # valor absolut

    def test_valor_absolut_fraccio_positiva(self):
        f = rat.Fraccio(1, 2)
        self.assertFraccio(abs(f), 1, 2)

    def test_valor_absolut_fraccio_negativa(self):
        f = rat.Fraccio(-1, 2)
        self.assertFraccio(abs(f), 1, 2)
