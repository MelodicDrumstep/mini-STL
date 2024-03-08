#include "gtest/gtest.h"
#include "../lru/lru.hpp"

using namespace sjtu;

TEST(HashMapTest, InsertAndFind) {
    hashmap<int, std::string> map;
    map.insert({1, "one"});
    map.insert({2, "two"});
    map.insert({3, "three"});

    ASSERT_EQ(map.find(1)->second, "one");
    ASSERT_EQ(map.find(2)->second, "two");
    ASSERT_EQ(map.find(3)->second, "three");
    ASSERT_EQ(map.find(4), map.end());
}

TEST(HashMapTest, Remove) {
    hashmap<int, std::string> map;
    map.insert({1, "one"});
    map.insert({2, "two"});
    map.insert({3, "three"});

    ASSERT_TRUE(map.remove(1));
    ASSERT_EQ(map.find(1), map.end());
    ASSERT_FALSE(map.remove(4));
}

TEST(HashMapTest, Clear) {
    hashmap<int, std::string> map;
    map.insert({1, "one"});
    map.insert({2, "two"});
    map.insert({3, "three"});

    map.clear();
    ASSERT_EQ(map.find(1), map.end());
    ASSERT_EQ(map.find(2), map.end());
    ASSERT_EQ(map.find(3), map.end());
}

TEST(HashMapTest, Iterator) {
    hashmap<int, std::string> map;
    map.insert({1, "one"});
    map.insert({2, "two"});
    map.insert({3, "three"});

    for(auto it = map.begin(); it != map.end(); ++it) {
        if((*(it.p2hashmap -> table))[it.index] != nullptr)
        {
            ASSERT_TRUE(it->first >= 1 && it->first <= 3);
        }
    }
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}