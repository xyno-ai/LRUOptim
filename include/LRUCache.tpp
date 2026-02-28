#pragma once

// --- Constructor ---

template<typename K, typename V>
LRUCache<K, V>::LRUCache(size_t cap) : capacity(cap) {}

// --- get ---

template<typename K, typename V>
V LRUCache<K, V>::get(const K& key) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        throw std::runtime_error("Not found");

    // move key to front
    lruList.remove(key);
    lruList.push_front(key);
    return it->second;
}

// --- put ---

template<typename K, typename V>
void LRUCache<K, V>::put(const K& key, const V& value) {
    std::lock_guard<std::mutex> lock(mtx);
    if (cacheMap.size() >= capacity) {
        auto old = lruList.back();
        lruList.pop_back();
        cacheMap.erase(old);
    }
    cacheMap[key] = value;
    lruList.push_front(key);
}
