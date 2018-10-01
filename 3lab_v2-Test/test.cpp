#include "pch.h"

TEST(TestCaseName, TestName) {


	EXPECT_EQ(ldecimal(3), ldecimal(8));

}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}