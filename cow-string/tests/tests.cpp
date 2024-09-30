#include <gtest/gtest.h>

#include "cow_string.h"

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

TEST(CowStringTest, FindFirstOf)
{
    {
        CowString str("Hello, world!");

        auto pos = str.FindFirstOf(',');
        ASSERT_EQ(pos, 5);

        pos = str.FindFirstOf(',', 20);
        ASSERT_EQ(pos, CowString::NPOS);

        pos = str.FindFirstOf('l');
        ASSERT_EQ(pos, 2);
        pos = str.FindFirstOf('l', 3);
        ASSERT_EQ(pos, 3);
    }
    {
        WCowString str(L"Hello, world!");

        auto pos = str.FindFirstOf(L',');
        ASSERT_EQ(pos, 5);

        pos = str.FindFirstOf(L',', 20);
        ASSERT_EQ(pos, WCowString::NPOS);

        pos = str.FindFirstOf(L'l');
        ASSERT_EQ(pos, 2);
        pos = str.FindFirstOf(L'l', 3);
        ASSERT_EQ(pos, 3);
    }
}

TEST(CowStringTest, Substring)
{
    {
        CowString str("Hello, world!");
        auto start = str.FindFirstOf(' ') + 1;
        auto end = str.FindFirstOf('!');

        CowString str2 = str.Substring(start, end);
        ASSERT_STREQ(str2.Data(), "world");
    }
    {
        WCowString str(L"Hello, world!");
        auto start = str.FindFirstOf(L' ') + 1;
        auto end = str.FindFirstOf(L'!');

        WCowString str2 = str.Substring(start, end);
        ASSERT_STREQ(str2.Data(), L"world");
    }
}

TEST(CowStringTest, Tokenization)
{
    {
        constexpr const std::size_t NMB_OF_TOKENS = 4;
        CowString str("|first*second+third|apple&");

        auto tokens = CowString::Tokenize(str, "|*+&");
        std::array<const char *, NMB_OF_TOKENS> expectedTokens = {"first", "second", "third", "apple"};

        for (std::size_t idx = 0; idx < NMB_OF_TOKENS; ++idx) {
            ASSERT_STREQ(tokens[idx].Data(), expectedTokens[idx]);
        }
    }
    {
        constexpr const std::size_t NMB_OF_TOKENS = 4;
        CowString str("first*s*a*second");

        auto tokens = CowString::Tokenize(str, "|*+&");
        std::array<const char *, NMB_OF_TOKENS> expectedTokens = {"first", "s", "a", "second"};

        for (std::size_t idx = 0; idx < NMB_OF_TOKENS; ++idx) {
            ASSERT_STREQ(tokens[idx].Data(), expectedTokens[idx]);
        }
    }
    {
        CowString str("|s|");
        ASSERT_STREQ(CowString::Tokenize(str, "|")[0].Data(), "s");

        str = "|s";
        ASSERT_STREQ(CowString::Tokenize(str, "|")[0].Data(), "s");

        str = "s|";
        ASSERT_STREQ(CowString::Tokenize(str, "|")[0].Data(), "s");
    }
}

TEST(CowStringTest, TokenizationWString)
{
    {
        constexpr const std::size_t NMB_OF_TOKENS = 4;
        WCowString str(L"|first*second+third|apple&");

        auto tokens = WCowString::Tokenize(str, L"|*+&");
        std::array<const wchar_t *, NMB_OF_TOKENS> expectedTokens = {L"first", L"second", L"third", L"apple"};

        for (std::size_t idx = 0; idx < NMB_OF_TOKENS; ++idx) {
            ASSERT_STREQ(tokens[idx].Data(), expectedTokens[idx]);
        }
    }
    {
        constexpr const std::size_t NMB_OF_TOKENS = 4;
        WCowString str(L"first*s*a*second");

        auto tokens = WCowString::Tokenize(str, L"|*+&");
        std::array<const wchar_t *, NMB_OF_TOKENS> expectedTokens = {L"first", L"s", L"a", L"second"};

        for (std::size_t idx = 0; idx < NMB_OF_TOKENS; ++idx) {
            ASSERT_STREQ(tokens[idx].Data(), expectedTokens[idx]);
        }
    }
    {
        WCowString str(L"|s|");
        ASSERT_STREQ(WCowString::Tokenize(str, L"|")[0].Data(), L"s");

        str = L"|s";
        ASSERT_STREQ(WCowString::Tokenize(str, L"|")[0].Data(), L"s");

        str = L"s|";
        ASSERT_STREQ(WCowString::Tokenize(str, L"|")[0].Data(), L"s");

        str = L"|||||||s";
        ASSERT_STREQ(WCowString::Tokenize(str, L"|")[0].Data(), L"s");
    }
}

TEST(CowStringTest, FindSubstring)
{
    const char *quote =
        "There are only two kinds of languages: the ones people complain about and the ones nobody uses.";
    CowString str(quote);
    std::string stdStr(quote);

    {
        auto pos = str.Find("kinds");
        auto expectedPos = stdStr.find("kinds");

        ASSERT_NE(pos, CowString::NPOS);
        ASSERT_NE(expectedPos, std::string::npos);
        ASSERT_EQ(pos, expectedPos);
    }
    {
        ASSERT_EQ(str.Find("error"), CowString::NPOS);
        ASSERT_EQ(str.Find("x"), CowString::NPOS);
        ASSERT_EQ(str.Find(""), CowString::NPOS);
    }
    {
        auto pos = str.Find("the");
        auto expectedPos = stdStr.find("the");
        ASSERT_EQ(pos, expectedPos);

        pos = str.Find("the", pos + 1);
        expectedPos = stdStr.find("the", expectedPos + 1);
        ASSERT_EQ(pos, expectedPos);
    }
    {
        auto pos = str.Find(quote);
        auto expectedPos = stdStr.find(quote);
        ASSERT_EQ(pos, expectedPos);
    }
    {
        auto pos = str.Find("kinds of languages:");
        auto expectedPos = stdStr.find("kinds of languages:");
        ASSERT_EQ(pos, expectedPos);
    }
}

TEST(CowStringTest, FindSubstringWString)
{
    const wchar_t *quote =
        L"There are only two kinds of languages: the ones people complain about and the ones nobody uses.";
    WCowString str(quote);
    std::wstring stdStr(quote);

    {
        auto pos = str.Find(L"kinds");
        auto expectedPos = stdStr.find(L"kinds");

        ASSERT_NE(pos, WCowString::NPOS);
        ASSERT_NE(expectedPos, std::wstring::npos);
        ASSERT_EQ(pos, expectedPos);
    }
    {
        ASSERT_EQ(str.Find(L"error"), WCowString::NPOS);
        ASSERT_EQ(str.Find(L"x"), WCowString::NPOS);
        ASSERT_EQ(str.Find(L""), WCowString::NPOS);
    }
    {
        auto pos = str.Find(L"the");
        auto expectedPos = stdStr.find(L"the");
        ASSERT_EQ(pos, expectedPos);

        pos = str.Find(L"the", pos + 1);
        expectedPos = stdStr.find(L"the", expectedPos + 1);
        ASSERT_EQ(pos, expectedPos);
    }
    {
        auto pos = str.Find(quote);
        auto expectedPos = stdStr.find(quote);
        ASSERT_EQ(pos, expectedPos);
    }
    {
        auto pos = str.Find(L"kinds of languages:");
        auto expectedPos = stdStr.find(L"kinds of languages:");
        ASSERT_EQ(pos, expectedPos);
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
