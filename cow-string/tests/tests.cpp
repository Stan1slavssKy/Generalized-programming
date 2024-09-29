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

TEST(CowStringTest, CopyAssignOperator)
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

TEST(CowStringTest, MoveConstructor)
{
    {
        CowString str1("Hello world!");
        auto *data1 = str1.Data();

        CowString str2(std::move(str1));
        ASSERT_EQ(data1, str2.Data());
    }
    {
        WCowString str1(L"Hello world!");
        auto *data1 = str1.Data();

        WCowString str2(std::move(str1));
        ASSERT_EQ(data1, str2.Data());
    }
}

TEST(CowStringTest, MoveAssignOperator)
{
    {
        CowString str1("Hello");
        auto *data1 = str1.Data();

        CowString str2("World");
        auto *data2 = str2.Data();

        str1 = std::move(str2);
        ASSERT_EQ(str1.Data(), data2);
        ASSERT_NE(str1.Data(), data1);

        str1 = std::move(str1);
        ASSERT_EQ(str1.Data(), data2);
    }
    {
        WCowString str1(L"Hello");
        auto *data1 = str1.Data();

        WCowString str2(L"World");
        auto *data2 = str2.Data();

        str1 = std::move(str2);
        ASSERT_EQ(str1.Data(), data2);
        ASSERT_NE(str1.Data(), data1);

        str1 = std::move(str1);
        ASSERT_EQ(str1.Data(), data2);
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
        WCowString str(L"Cat");
        str[0] = L'猫';
        str[1] = L'!';
        str[2] = L'!';
        ASSERT_STREQ(str.Data(), L"猫!!");
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
    {
        WCowString str1(L"Cat");
        WCowString str2(str1);

        ASSERT_EQ(str1.Data(), str2.Data());

        str1[0] = L'猫';
        str1[1] = L'!';
        str1[2] = L'!';

        ASSERT_STREQ(str1.Data(), L"猫!!");
        ASSERT_NE(str1.Data(), str2.Data());
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
