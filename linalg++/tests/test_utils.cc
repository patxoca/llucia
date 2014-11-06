// -*- coding: utf-8 -*-

// $Id:$

#include "gtest/gtest.h"
#include "../utils.h"

TEST(UtilsDecimalACoalicio, mida_coalicio_coincideix_amb_DIMENSIO) {
	Coalicio v;

	v = enter_a_coalicio(3, 6);
	EXPECT_EQ(v.size(), 6);
}

TEST(UtilsDecimalACoalicio, contingut_coalicio_buida) {
	Coalicio v;
	Enter e[] = {0, 0, 0, 0, 0, 0};

	v = enter_a_coalicio(0, 6);
	for (int i = 0; i < 6; i++) {
		EXPECT_EQ(e[i], v[i]);
	}
}

TEST(UtilsDecimalACoalicio, contingut_coalicio_no_buida) {
	Coalicio v;
	Enter e[] = {1, 1, 0, 0, 0, 0};

	v = enter_a_coalicio(3, 6);
	for (int i = 0; i < 6; i++) {
		EXPECT_EQ(e[i], v[i]);
	}
}

TEST(UtilsDecimalACoalicio, contingut_coalicio_total) {
	Coalicio v;
	Enter e[] = {1, 1, 1, 1, 1, 1};

	v = enter_a_coalicio(63, 6);
	for (int i = 0; i < 6; i++) {
		EXPECT_EQ(e[i], v[i]);
	}
}
