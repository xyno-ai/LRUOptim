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
    LRUCache(size_t cap) : capacity(cap) {}

    V get(const K& key) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = cacheMap.find(key);
        if (it == cacheMap.end())
            throw std::runtime_error("Not found");

        // move key to front
        lruList.remove(key);
        lruList.push_front(key);
        return it->second;
    }

    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (cacheMap.size() >= capacity) {
            auto old = lruList.back();
            lruList.pop_back();
            cacheMap.erase(old);
        }
        cacheMap[key] = value;
        lruList.push_front(key);
    }
};
