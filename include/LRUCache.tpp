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

    // move key to front in O(1)
    lruList.erase(it->second.second);
    lruList.push_front(key);
    it->second.second = lruList.begin();
    return it->second.first;
}

// --- put ---

template<typename K, typename V>
void LRUCache<K, V>::put(const K& key, const V& value) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = cacheMap.find(key);
    if (it != cacheMap.end()) {
        lruList.erase(it->second.second);
    } else if (cacheMap.size() >= capacity) {
        cacheMap.erase(lruList.back());
        lruList.pop_back();
    }
    lruList.push_front(key);
    cacheMap[key] = {value, lruList.begin()};
}
