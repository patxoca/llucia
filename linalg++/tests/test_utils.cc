// -*- coding: utf-8 -*-

// $Id:$

#include "gtest/gtest.h"
#include "../utils.h"

TEST(UtilsDecimalACoalicio, valor_de_DIMENSIO_es_6) {
	/* els tests depenen de que la DIMENSIO sigui 6 */
	EXPECT_EQ(DIMENSIO, 6);
}

TEST(UtilsDecimalACoalicio, mida_coalicio_coincideix_amb_DIMENSIO) {
	Coalicio v;

	v = enter_a_coalicio(3);
	EXPECT_EQ(v.size(), DIMENSIO);
}

TEST(UtilsDecimalACoalicio, contingut_coalicio_buida) {
	Coalicio v;
	Enter e[] = {0, 0, 0, 0, 0, 0};

	v = enter_a_coalicio(0);
	for (int i = 0; i < DIMENSIO; i++) {
		EXPECT_EQ(e[i], v[i]);
	}
}

TEST(UtilsDecimalACoalicio, contingut_coalicio_no_buida) {
	Coalicio v;
	Enter e[] = {1, 1, 0, 0, 0, 0};

	v = enter_a_coalicio(3);
	for (int i = 0; i < DIMENSIO; i++) {
		EXPECT_EQ(e[i], v[i]);
	}
}

TEST(UtilsDecimalACoalicio, contingut_coalicio_total) {
	Coalicio v;
	Enter e[] = {1, 1, 1, 1, 1, 1};

	v = enter_a_coalicio(63);
	for (int i = 0; i < DIMENSIO; i++) {
		EXPECT_EQ(e[i], v[i]);
	}
}
