// pool.h
// Intrusive slab-based free-list allocator with exponential slab growth.
//
// Each free slot stores the next-free pointer in its own bytes (intrusive list),
// so no separate free-list array is needed.  alloc() / dealloc() are two
// pointer operations with no branch in the steady state.
//
// Slab sizes double on every allocation (256 → 512 → 1024 → …) so:
//   - A new book costs only initial_slab × sizeof(T) bytes upfront.
//   - A heavily loaded book amortises growth cost over O(log N) slab allocs.
//
// Requirement: sizeof(T) >= sizeof(void*).
#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

template<typename T>
class Pool {
    static_assert(sizeof(T) >= sizeof(void*),
                  "Pool<T>: T must be at least pointer-sized for the intrusive free list");

public:
    // initial_slab: number of slots in the first slab.  Kept small (256) so
    // that a large number of books doesn't cause a huge upfront RSS.
    explicit Pool(size_t initial_slab = 256)
        : next_slab_size_(initial_slab), free_head_(nullptr) {
        add_slab();
    }

    T* alloc() {
        if (!free_head_) [[unlikely]] {
            add_slab();
        }
        void** node = static_cast<void**>(free_head_);
        free_head_  = *node;
        return reinterpret_cast<T*>(node);
    }

    void dealloc(T* p) {
        void** node = reinterpret_cast<void**>(p);
        *node       = free_head_;
        free_head_  = node;
    }

    size_t slab_count() const { return slabs_.size(); }

private:
    struct alignas(alignof(T)) Slot {
        unsigned char bytes[sizeof(T)];
    };

    void add_slab() {
        const size_t n = next_slab_size_;
        next_slab_size_ *= 2;   // exponential growth: 256→512→1024→…

        auto& slab = slabs_.emplace_back(std::make_unique<Slot[]>(n));
        for (size_t i = 0; i < n; ++i) {
            void** node = reinterpret_cast<void**>(&slab[i].bytes);
            *node       = free_head_;
            free_head_  = node;
        }
    }

    size_t next_slab_size_;
    void*  free_head_;
    std::vector<std::unique_ptr<Slot[]>> slabs_;
};
