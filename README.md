# LRUOptim

A fast, thread-safe LRU cache in C++.

## What's in here

- **`LRUCache<K, V>`** — single-lock LRU cache. O(1) get/put using an unordered map + doubly-linked list with stored iterators.
- **`ShardedLRUCache<K, V>`** — same thing but splits across N independent shards, so threads don't fight over one lock.

## Build & test

```
mkdir -p build && cd build
cmake ..
cmake --build .
./lru_tests
```

## Usage

```cpp
// basic
LRUCache<int, std::string> cache(100);
cache.put(1, "hello");
cache.get(1); // "hello"

// sharded (better under concurrency)
ShardedLRUCache<int, std::string> cache(1000, 16); // 1000 capacity, 16 shards
cache.put(1, "hello");
cache.get(1); // "hello"
```

Missing keys throw `std::runtime_error`.
