// -*- coding: utf-8 -*-

// $Id:$

#include "gtest/gtest.h"
#include "../combinator.h"

static void EXPECT_COMB_EQ(int len, const int *v, const int *e) {
	for (int i = 0; i < len; i++) {
		EXPECT_EQ(v[i], e[i]);
	}
}

static void EXPECT_COMBINATOR_EQ(Combinator & comb, int *e, int size, int batch = 1) {
	const int *c;

	for (c = comb.first(); c != NULL; c = comb.next(batch), e += size) {
		EXPECT_COMB_EQ(size, c, e);
	}
}

TEST(CombinatorTest, one_by_one) {
	Combinator comb(4, 2);
	int e[][2] = {{1, 2},
				  {1, 3},
				  {1, 4},
				  {2, 3},
				  {2, 4},
				  {3, 4},
				  {-1, -1}};

	EXPECT_COMBINATOR_EQ(comb, (int*)&e, 2);
}

TEST(CombinatorTest, n_less_than_m_gives_no_combinations) {
	Combinator comb(2, 4);
	int e[][2] = {{-1, -1}};

	EXPECT_COMBINATOR_EQ(comb, (int*)&e, 2);
}

TEST(CombinatorTest, batch_combinator) {
	Combinator comb(4, 2);
	int e[][2] = {{1, 2},
				  // {1, 3}, ignored
				  // {1, 4}, ignored
				  {2, 3},
				  // {2, 4}, ignored
				  // {3, 4}, ignored
				  {-1, -1}};

	EXPECT_COMBINATOR_EQ(comb, (int*)&e, 2, 3);
}

TEST(CombinatorTest, seeded_combinator) {
	int seed[] = {1, 4};
	Combinator comb(4, 2, (int*)seed, 3);
	int e[][2] = {{1, 4},
				  {2, 3},
				  {2, 4},
				  {-1, -1}};

	EXPECT_COMBINATOR_EQ(comb, (int*)&e, 2);
}
