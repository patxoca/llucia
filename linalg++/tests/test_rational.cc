#include "gtest/gtest.h"
#include "../rational.h"

typedef Rational<int> Fraccio;


// 		                _                   _
//   ___ ___  _ __  ___| |_ _ __ _   _  ___| |_ ___  _ __
//  / __/ _ \| '_ \/ __| __| '__| | | |/ __| __/ _ \| '__|
// | (_| (_) | | | \__ \ |_| |  | |_| | (__| || (_) | |
//  \___\___/|_| |_|___/\__|_|   \__,_|\___|\__\___/|_|


TEST(RationalConstructorTest, SenseParametresEsZero) {
	Fraccio f;

	EXPECT_EQ(0, f.getNumerator());
	EXPECT_EQ(1, f.getDenominator());
}

TEST(RationalConstructorTest, EnterZero) {
	Fraccio f(0);

	EXPECT_EQ(0, f.getNumerator());
	EXPECT_EQ(1, f.getDenominator());
}

TEST(RationalConstructorTest, RacionalZero) {
	Fraccio f(0, 2);

	EXPECT_EQ(0, f.getNumerator());
	EXPECT_EQ(1, f.getDenominator());
}

TEST(RationalConstructorTest, UnEnter) {
	Fraccio f(2);

	EXPECT_EQ(2, f.getNumerator());
	EXPECT_EQ(1, f.getDenominator());
}

TEST(RationalConstructorTest, DosEntersPositius) {
	Fraccio f(2, 3);

	EXPECT_EQ(2, f.getNumerator());
	EXPECT_EQ(3, f.getDenominator());
}

TEST(RationalConstructorTest, DosEntersNumeradorNegatiu) {
	Fraccio f(-2, 3);

	EXPECT_EQ(-2, f.getNumerator());
	EXPECT_EQ(3, f.getDenominator());
}

TEST(RationalConstructorTest, DosEntersDenominadorNegatiu) {
	Fraccio f(2, -3);

	EXPECT_EQ(-2, f.getNumerator());
	EXPECT_EQ(3, f.getDenominator());
}

TEST(RationalConstructorTest, Racional) {
	Fraccio f1(2, 3);
	Fraccio f2(f1);

	EXPECT_EQ(f1.getNumerator(), f2.getNumerator());
	EXPECT_EQ(f1.getDenominator(), f2.getDenominator());
}

TEST(RationalConstructorTest, RacionalClona) {
	Fraccio f1(2, 3);
	Fraccio f2(f1);

	EXPECT_EQ(f1.getNumerator(), f2.getNumerator());
	f1.setNumerator(1);
	EXPECT_NE(f1.getNumerator(), f2.getNumerator());
}

//  	       _ _                  _   _
//   __ _ _ __(_) |_ _ __ ___   ___| |_(_) ___ __ _
//  / _` | '__| | __| '_ ` _ \ / _ \ __| |/ __/ _` |
// | (_| | |  | | |_| | | | | |  __/ |_| | (_| (_| |
//  \__,_|_|  |_|\__|_| |_| |_|\___|\__|_|\___\__,_|

//======================================================================
// suma

TEST(RationalArithmeticTest, SumaEnterZero) {
	Fraccio f(1, 2);
	Fraccio res;

	res = f + 0;

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, SumaFraccioZero) {
	Fraccio f(1, 2);
	Fraccio res;

	res = f + Fraccio();

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, SumaEnterDreta) {
	Fraccio f(1, 2);
	Fraccio res;

	res = f + 2;

	EXPECT_EQ(res.getNumerator(), 5);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, SumaFraccio) {
	Fraccio f(1, 2);
	Fraccio res = f + Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), 7);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, SumaFraccioDenominadorNegatiu) {
	Fraccio f(1, 2);
	Fraccio res = f + Fraccio(2, -3);

	EXPECT_EQ(res.getNumerator(), -1);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, AutosumaEnter) {
	Fraccio f(1, 2);

	f += 2;

	EXPECT_EQ(f.getNumerator(), 5);
	EXPECT_EQ(f.getDenominator(), 2);
}

TEST(RationalArithmeticTest, AutosumaFraccio) {
	Fraccio f(1, 2);

	f += Fraccio(2, 3);

	EXPECT_EQ(f.getNumerator(), 7);
	EXPECT_EQ(f.getDenominator(), 6);
}

//======================================================================
// resta

TEST(RationalArithmeticTest, RestaEnterZero) {
	Fraccio res;

	res = Fraccio(1, 2) - 0;

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, RestaFraccioZero) {
	Fraccio res;

	res = Fraccio(1, 2) - Fraccio(0);

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, RestaEnterDreta) {
	Fraccio res;

	res = Fraccio(1, 2) - 2;

	EXPECT_EQ(res.getNumerator(), -3);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, RestaFraccio) {
	Fraccio res;

	res = Fraccio(1, 2) - Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), -1);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, AutorestaEnter) {
	Fraccio res = Fraccio(1, 2);

	res -= 2;

	EXPECT_EQ(res.getNumerator(), -3);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, AutorestaFraccio) {
	Fraccio res = Fraccio(1, 2);

	res -= Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), -1);
	EXPECT_EQ(res.getDenominator(), 6);
}

//======================================================================
// multiplicacio

TEST(RationalArithmeticTest, TestMultiplicacioEnterZero) {
	Fraccio res;

	res = Fraccio(1, 2) * 0;

	EXPECT_EQ(res.getNumerator(), 0);
	EXPECT_EQ(res.getDenominator(), 1);
}

TEST(RationalArithmeticTest, TestMultiplicacioFraccioZero) {
	Fraccio res;

	res = Fraccio(1, 2) * Fraccio(0);

	EXPECT_EQ(res.getNumerator(), 0);
	EXPECT_EQ(res.getDenominator(), 1);
}

TEST(RationalArithmeticTest, TestMultiplicacioEnterUn) {
	Fraccio res;

	res = Fraccio(1, 2) * 1;
	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestMultiplicacioFraccioUn) {
	Fraccio res;

	res = Fraccio(1, 2) * Fraccio(1);

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestMultiplicacioEnterDreta) {
	Fraccio res;

    res = Fraccio(1, 2) * 3;

	EXPECT_EQ(res.getNumerator(), 3);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestMultiplicacioFraccio) {
	Fraccio res;

	res = Fraccio(1, 2) * Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), 2);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, TestAutomultiplicacioEnter) {
	Fraccio res = Fraccio(1, 2);

	res *= 3;

	EXPECT_EQ(res.getNumerator(), 3);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestAutomultiplicacioFraccio) {
	Fraccio res = Fraccio(1, 2);

	res *= Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), 2);
	EXPECT_EQ(res.getDenominator(), 6);
}

//======================================================================
// diviso

// TEST(RationalArithmeticTest, TestDivisioEnterZeroDisparaExcepcio) {
// 	EXPECT_THROW({ Fraccio(1, 2) / 0 ; }, NULL);
// }

// TEST(RationalArithmeticTest, TestDivisioFraccioZeroDisparaExcepcio) {
// 	EXPECT_THROW({ Fraccio(1, 2) / Fraccio(0) ; }, NULL);
// }

TEST(RationalArithmeticTest, TestDivisioEnterUn) {
	Fraccio res;

	res = Fraccio(1, 2) / 1;

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestDivisioFraccioUn) {
	Fraccio res;

	res = Fraccio(1, 2) / Fraccio(1);

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestDivisioEnterDreta) {
	Fraccio res;

	res = Fraccio(1, 2) / 3;

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, TestDivisioFraccio) {
	Fraccio res;

	res = Fraccio(1, 2) / Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), 3);
	EXPECT_EQ(res.getDenominator(), 4);
}

TEST(RationalArithmeticTest, TestAutodivisioEnter) {
	Fraccio res = Fraccio(1, 2);

	res /= 3;

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, TestAutodivisioFraccio) {
	Fraccio res = Fraccio(1, 2);

	res /= Fraccio(2, 3);

	EXPECT_EQ(res.getNumerator(), 3);
	EXPECT_EQ(res.getDenominator(), 4);
}

TEST(RationalArithmeticTest, TestDivisioFraccioNegativa) {
	Fraccio res;

	res = Fraccio(1, 2) / Fraccio(-2, 3);

	EXPECT_EQ(res.getNumerator(), -3);
	EXPECT_EQ(res.getDenominator(), 4);
}

TEST(RationalArithmeticTest, TestAutodivisioEnterNegatiu) {
	Fraccio res = Fraccio(1, 2);

	res /= -3;

	EXPECT_EQ(res.getNumerator(), -1);
	EXPECT_EQ(res.getDenominator(), 6);
}

TEST(RationalArithmeticTest, TestAutodivisioFraccioNegativa) {
	Fraccio res = Fraccio(1, 2);

	res /= Fraccio(-2, 3);

	EXPECT_EQ(res.getNumerator(), -3);
	EXPECT_EQ(res.getDenominator(), 4);
}

//======================================================================
// canvi de signe

TEST(RationalArithmeticTest, TestCanviSigne) {
	Fraccio res;

	res = -Fraccio(1, 2);

	EXPECT_EQ(res.getNumerator(), -1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestDobleCanviSigneNumerador) {
	Fraccio res;

	res = -Fraccio(-1, 2);

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestDobleCanviSigneDenominador) {
	Fraccio res;

	res = -Fraccio(1, -2);

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

//======================================================================
// valor absolut

TEST(RationalArithmeticTest, TestValorAbsolutFraccioPositiva) {
	Fraccio res;

	res = abs(Fraccio(1, 2));

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}

TEST(RationalArithmeticTest, TestValorAbsolutFraccioNegativa) {
	Fraccio res;

	res = abs(Fraccio(-1, 2));

	EXPECT_EQ(res.getNumerator(), 1);
	EXPECT_EQ(res.getDenominator(), 2);
}
