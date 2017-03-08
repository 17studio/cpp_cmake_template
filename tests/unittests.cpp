
#include <gtest.h>
#include "./foo.h"

TEST(FooTest, HandleNoneZeroInput)
{
    Foo foo1(2);
    Foo foo2(6);

    EXPECT_EQ(2, foo1.getId());
    EXPECT_EQ(6, foo2.getId());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}