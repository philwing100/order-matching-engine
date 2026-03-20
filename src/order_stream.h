// order_stream.h
// Lock-free single-producer / single-consumer ring buffer.
#pragma once

#include <atomic>
#include <cstdint>
#include <memory>

// Portable spin-pause hint.
// On x86/x64 the PAUSE instruction reduces bus traffic during spin-waits and
// is critical for Hyper-Threading — without it the spinning thread monopolises
// the cache line carrying the counter it is waiting on.
// On ARM "yield" is the equivalent advisory hint.
#if defined(__x86_64__) || defined(__i386__)
#  define SPIN_PAUSE() __builtin_ia32_pause()
#elif defined(__aarch64__) || defined(__arm__)
#  define SPIN_PAUSE() __asm__ volatile("yield" ::: "memory")
#else
#  define SPIN_PAUSE() ((void)0)
#endif

template<typename T, size_t N = (1 << 20)>
class OrderStream {
    static_assert((N & (N - 1)) == 0, "N must be a power of 2");
    static constexpr uint64_t MASK = N - 1;

public:
    OrderStream() : buffer_(std::make_unique<T[]>(N)) {}

    // Called by the producer thread only.
    void push(T item) {
        const uint64_t t = tail_.load(std::memory_order_relaxed);
        while (t - head_.load(std::memory_order_acquire) >= N) {
            SPIN_PAUSE();
        }
        buffer_[t & MASK] = std::move(item);
        tail_.store(t + 1, std::memory_order_release);
    }

    // Called by the consumer thread only.
    T pop() {
        const uint64_t h = head_.load(std::memory_order_relaxed);
        while (tail_.load(std::memory_order_acquire) == h) {
            SPIN_PAUSE();
        }
        T item = std::move(buffer_[h & MASK]);
        head_.store(h + 1, std::memory_order_release);
        return item;
    }

private:
    alignas(64) std::atomic<uint64_t> head_{0};
    alignas(64) std::atomic<uint64_t> tail_{0};
    std::unique_ptr<T[]> buffer_;
};
