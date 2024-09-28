#include <gtest/gtest.h>

#include "cow_string.h"

TEST(CowStringTest, RawStringConstructor)
{
    CowString str("Hello");

    WCowString str2 (L"Hello");
}

TEST(CowStringTest, CopyConstructor)
{
    {
        CowString str1("Hello");
        CowString str2(str1);

        ASSERT_EQ(str1.Data(), str2.Data());
        ASSERT_EQ(str1.Length(), str2.Length());
    }
    {
        WCowString str1(L"Hello");
        WCowString str2(str1);

        ASSERT_EQ(str1.Data(), str2.Data());
        ASSERT_EQ(str1.Length(), str2.Length());
    }
}

TEST(CowStringTest, CopyOperator)
{
    {
        CowString str1("Hello");
        CowString str2("World");

        str1 = str2;

        ASSERT_EQ(str1.Data(), str2.Data());
        ASSERT_EQ(str1.Length(), str2.Length());
    }
    {
        WCowString str1(L"Hello");
        WCowString str2(L"World");

        str1 = str2;

        ASSERT_EQ(str1.Data(), str2.Data());
        ASSERT_EQ(str1.Length(), str2.Length());
    }
}

TEST(CowStringTest, SubscriptOperator)
{
    {
        CowString str("Hello");
        str[3] = 'p';
        str[4] = '!';
        ASSERT_STREQ(str.Data(), "Help!");
    }
    {
        CowString str1("Hello");
        CowString str2(str1);

        ASSERT_EQ(str1.Data(), str2.Data());

        str1[3] = 'p';
        str1[4] = '!';

        ASSERT_STREQ(str1.Data(), "Help!");
        ASSERT_NE(str1.Data(), str2.Data());
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
