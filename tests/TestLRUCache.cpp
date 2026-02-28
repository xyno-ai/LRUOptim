#include <gtest/gtest.h>
#include "LRUCache.h"
#include <string>
#include <thread>
#include <vector>

// --- Basic Operations ---

TEST(LRUCacheTest, PutAndGet) {
    LRUCache<int, std::string> cache(3);
    cache.put(1, "one");
    cache.put(2, "two");
    EXPECT_EQ(cache.get(1), "one");
    EXPECT_EQ(cache.get(2), "two");
}

TEST(LRUCacheTest, GetMissingKeyThrows) {
    LRUCache<int, int> cache(2);
    EXPECT_THROW(cache.get(42), std::runtime_error);
}

TEST(LRUCacheTest, OverwriteExistingKey) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "old");
    cache.put(1, "new");
    EXPECT_EQ(cache.get(1), "new");
}

// --- Eviction ---

TEST(LRUCacheTest, EvictsLeastRecentlyUsed) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three"); // should evict key 1

    EXPECT_THROW(cache.get(1), std::runtime_error);
    EXPECT_EQ(cache.get(2), "two");
    EXPECT_EQ(cache.get(3), "three");
}

TEST(LRUCacheTest, GetPromotesKey) {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "one");
    cache.put(2, "two");
    cache.get(1);          // promote key 1
    cache.put(3, "three"); // should evict key 2 (now LRU)

    EXPECT_EQ(cache.get(1), "one");
    EXPECT_EQ(cache.get(3), "three");
    EXPECT_THROW(cache.get(2), std::runtime_error);
}

TEST(LRUCacheTest, EvictionChain) {
    LRUCache<int, int> cache(3);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    cache.put(4, 40); // evicts 1
    cache.put(5, 50); // evicts 2

    EXPECT_THROW(cache.get(1), std::runtime_error);
    EXPECT_THROW(cache.get(2), std::runtime_error);
    EXPECT_EQ(cache.get(3), 30);
    EXPECT_EQ(cache.get(4), 40);
    EXPECT_EQ(cache.get(5), 50);
}

// --- Capacity 1 ---

TEST(LRUCacheTest, CapacityOne) {
    LRUCache<int, int> cache(1);
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100);

    cache.put(2, 200); // evicts 1
    EXPECT_THROW(cache.get(1), std::runtime_error);
    EXPECT_EQ(cache.get(2), 200);
}

// --- Thread Safety ---

TEST(LRUCacheTest, ConcurrentPuts) {
    LRUCache<int, int> cache(1000);
    std::vector<std::thread> threads;

    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&cache, t]() {
            for (int i = t * 100; i < (t + 1) * 100; ++i) {
                cache.put(i, i * 10);
            }
        });
    }
    for (auto& th : threads) th.join();

    // All 400 keys should be present (capacity is 1000)
    for (int i = 0; i < 400; ++i) {
        EXPECT_EQ(cache.get(i), i * 10);
    }
}

TEST(LRUCacheTest, ConcurrentGetAndPut) {
    LRUCache<int, int> cache(50);
    cache.put(0, 0);

    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&cache, t]() {
            for (int i = 1; i <= 100; ++i) {
                cache.put(t * 1000 + i, i);
                try { cache.get(0); } catch (...) {}
            }
        });
    }
    for (auto& th : threads) th.join();
    // No crashes or deadlocks = pass
}

// --- String Keys ---

TEST(LRUCacheTest, StringKeys) {
    LRUCache<std::string, int> cache(2);
    cache.put("alpha", 1);
    cache.put("beta", 2);
    EXPECT_EQ(cache.get("alpha"), 1);

    cache.put("gamma", 3); // evicts "beta"
    EXPECT_THROW(cache.get("beta"), std::runtime_error);
    EXPECT_EQ(cache.get("gamma"), 3);
}
