#include "gtest/gtest.h"
#include "concurrent_trie.hpp"
#include <thread>

TEST(TrieStoreTest, PutAndGet) {
    TrieStore trie;

    trie.Put<int>("one", 1);
    trie.Put<int>("two", 2);

    EXPECT_EQ(*(trie.Get<int>("one").value()), 1);
    EXPECT_EQ(*(trie.Get<int>("two").value()), 2);
}

TEST(TrieStoreTest, ConcurrentPut) {
    TrieStore trie;

    std::thread t1([&]() { trie.Put<int>("one", 1); });
    std::thread t2([&]() { trie.Put<int>("two", 2); });

    t1.join();
    t2.join();

    EXPECT_EQ(*(trie.Get<int>("one").value()), 1);
    EXPECT_EQ(*(trie.Get<int>("two").value()), 2);
}

TEST(TrieStoreTest, Remove) {
    TrieStore trie;

    trie.Put<int>("one", 1);
    trie.Remove("one");

    EXPECT_EQ(trie.Get<int>("one"), std::nullopt);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}