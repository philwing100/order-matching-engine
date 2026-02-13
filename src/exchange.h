//Exchange.h
#pragma once
#include "order.h"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "order_stream.h"

class Exchange
{
    public:
        Exchange(OrderStream<Order>& stream,
             int orders_per_sec,
             int num_symbols,
             int duration_sec);

    void start(int opt);
    void stop();
    
    private:
        void run(int opt);

    OrderStream<Order>& stream_;
    int rate_;
    int symbols_;
    int duration_;
    std::atomic<bool> running_;
    std::thread thread_;
};

//The exchange will be used to populate the datastream with orders.


//The number of processed orders needs to be reported as well. 
//Take in the duration and it should calculate the time spent

