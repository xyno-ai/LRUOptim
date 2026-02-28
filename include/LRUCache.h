#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <functional>

template<typename K, typename V>
class LRUCache {
    using ListIterator = typename std::list<K>::iterator;
    std::unordered_map<K, std::pair<V, ListIterator>> cacheMap;
    std::list<K> lruList;
    std::mutex mtx;
    size_t capacity;

public:
    LRUCache(size_t cap);

    V get(const K& key);

    void put(const K& key, const V& value);
};

template<typename K, typename V>
class ShardedLRUCache {
    std::vector<std::unique_ptr<LRUCache<K, V>>> shards;
    size_t shardCount;

    size_t getShard(const K& key) const;

public:
    ShardedLRUCache(size_t capacity, size_t numShards = 16);

    V get(const K& key);

    void put(const K& key, const V& value);
};

#include "LRUCache.tpp"
