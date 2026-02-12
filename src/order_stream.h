// order_stream.h
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class OrderStream {
public:
    void push(T order) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(order));
        }
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]{ return !queue_.empty(); });

        T order = std::move(queue_.front());
        queue_.pop();
        return order;
    }

private:
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
};
