//
// Created by Jiyang Gong on 2019-05-15.
//

#include "test.h"

// 测试集为 MyTest，测试案例为 Sum
TEST(MyTest, Sum)
{
    std::vector<int> vec{1, 2, 3, 4, 5, 6};
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(sum, 21);
}
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
