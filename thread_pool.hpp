//
// Created by smacdonald on 12/04/2021.
//

#ifndef MANDELBROT_THREAD_POOL_HPP
#define MANDELBROT_THREAD_POOL_HPP

#endif //MANDELBROT_THREAD_POOL_HPP

#include <memory>
#include <queue>
#include <mutex>
#include <utility>
#include <string>
#include <thread>
#include <future>
#include <condition_variable>

#include "type_defs.hpp"
#include "compute_scalar.hpp"
#include "compute_avx.hpp"
#include "colour_chunk.hpp"


class ThreadPool {
private:
    bool queue_made = false;
    std::condition_variable cv;

    std::vector<std::thread> thread_pool;

    mandelbrot mandelbrot_data{};

    std::deque<mandelbrotPair> task_queue;
    std::mutex queue_mut;

    void Thread_func() {
        while (true) {
            queue_mut.lock();
            if (task_queue.empty()) {
                queue_mut.unlock();
                return;
            }

            mandelbrotPair task_pair = task_queue.front();
            task_queue.pop_front();
            queue_mut.unlock();

            mandelbrotFN task = task_pair.first;
            chunk chunk_data = task_pair.second;

            task(std::ref(chunk_data), std::ref(mandelbrot_data));
        }
    }


public:
    uint thread_count = std::thread::hardware_concurrency();

    //this needs to be a multiple of 4 for the avx implementation to work:
    const int chunk_size = 4;

    explicit ThreadPool(mandelbrot &mandelbrot_pass) {
        mandelbrot_data = mandelbrot_pass;

        std::async(std::launch::async, [&]{init_queue(); });

        thread_pool.reserve(thread_count);

        for (uint i = 0; i < thread_count; ++i) {
            thread_pool.emplace_back(std::thread(&ThreadPool::Thread_func, this));
        }
    };

    ~ThreadPool() {
        if (thread_pool.empty()) {
            return;
        } else {
            close_threads();
        }
    }

    void close_threads() {
        // blocks until threads are joinable
        // once all threads are joined, clear the vector
        for (auto & i : thread_pool) {
            i.join();
        }

        thread_pool.clear();
    }

    void init_queue(){
        if (queue_made) {
            return;
        }
        for (int y = 0; y < image::y_res; y += chunk_size) {
            for (int x = 0; x < image::x_res; x += chunk_size) {
                chunk temp_chunk{};
                temp_chunk.min_x = x;
                temp_chunk.max_x = (x + chunk_size);
                temp_chunk.min_y = y;
                temp_chunk.max_y = (y + chunk_size);

                task_queue.emplace_back(compute_scalar, temp_chunk);
//                task_queue.emplace_back(compute_avx, temp_chunk);     // I'd like this to work but it doesn't :(
            }
            for (int x = 0; x < image::x_res; x += chunk_size) {
                chunk temp_chunk{};
                temp_chunk.min_x = x;
                temp_chunk.max_x = (x + chunk_size);
                temp_chunk.min_y = y;
                temp_chunk.max_y = (y + chunk_size);

                task_queue.emplace_back(colour_chunk, temp_chunk);
            }
        }
        queue_made = true;
    }
};