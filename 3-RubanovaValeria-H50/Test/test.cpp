#include "pch.h"
#include "LabH.h"

TEST(lala, lala1) {
	aa* tree = insertTree(1, NULL);

	EXPECT_EQ(tree->data, 1);
	destroy(tree);
	EXPECT_TRUE(!bottom);
}