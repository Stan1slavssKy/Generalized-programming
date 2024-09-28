#include <gtest/gtest.h>

#include "cow_string.h"

TEST(CowStringTest, constructor)
{
    CowString str("Hello");
    std::cout << str.Data() << std::endl;   
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
