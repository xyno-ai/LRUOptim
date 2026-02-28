#pragma once

#include <iostream>
#include <map>
#include <list>
#include <mutex>
#include <stdexcept>

template<typename K, typename V>
class LRUCache {
    std::map<K, V> cacheMap;
    std::list<K> lruList;
    std::mutex mtx;
    size_t capacity;

public:
    LRUCache(size_t cap);

    V get(const K& key);

    void put(const K& key, const V& value);
};

#include "LRUCache.tpp"
