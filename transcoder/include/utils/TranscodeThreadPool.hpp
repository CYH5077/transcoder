#pragma once

#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

#include "dto/DtoWSTranscodeProgress.hpp"
#include "http/ws_recv_handler/WSClient.hpp"

namespace tr {
    using ThreadQueuePair = std::pair<std::function<void()>, WSClientPtr>;

    template <typename R>
    class TranscodeThreadPool {
    public:
        explicit TranscodeThreadPool(unsigned int threadCount = std::thread::hardware_concurrency())
            : isRunning(false) {
            this->createThread(threadCount);
        }

        virtual ~TranscodeThreadPool() {
            this->stop();
        }

    public:
        template <typename Func, typename... Args>
        std::future<R> enqueue(WSClientPtr client, Func&& func, Args&&... args) {
            auto task = std::make_shared<std::packaged_task<R()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

            std::future<R> result = task->get_future();
            {
                std::lock_guard<std::mutex> lockGuard(this->mutex);
                this->taskQueue.push_back(ThreadQueuePair([task]() { (*task)(); }, client));
                client->sendResponse(DtoWSTranscodeProgress::createWaitMessage(this->taskQueue.size()));
            }

            this->condition.notify_one();
            return result;
        }

        size_t getThreadCount() {
            return this->threadList.size();
        }

        void stop() {
            this->isRunning = false;
            this->condition.notify_all();

            for (auto& thread : this->threadList) {
                if (thread.joinable() == false) {
                    continue;
                }
                thread.join();
            }
        }

        size_t getTaskQueueSize() {
            return this->taskQueue.size();
        }

    private:
        void createThread(unsigned int threadCount) {
            for (unsigned int i = 0; i < threadCount; i++) {
                this->threadList.emplace_back([this]() {
                    while (true) {
                        ThreadQueuePair task;
                        {
                            std::unique_lock<std::mutex> lockGaurd(this->mutex);
                            this->condition.wait(
                                lockGaurd, [this]() { 
                                    std::cout << "isRunning: " << isRunning << " taskQueue.empty(): " << taskQueue.empty() << std::endl; 
                                    return this->isRunning == false || !this->taskQueue.empty(); 
                                });
                            if (this->isRunning == false && this->taskQueue.empty() == true) {
                                return;
                            }

                            task = std::move(this->taskQueue.front());
                            this->taskQueue.pop_front();

                            int count = 1;
                            std::cout << "send" << std::endl;
                            for (auto& task : this->taskQueue) {
                                task.second->sendResponse(DtoWSTranscodeProgress::createWaitMessage(count++));
                            }
                            std::cout << "send finish" << std::endl;
                        }
                        task.first();
                    }
                });

                this->isRunning = true;
            }
        }

    private:
        bool isRunning;

        std::vector<std::thread> threadList;

        std::mutex mutex;
        std::condition_variable condition;

        std::list<ThreadQueuePair> taskQueue;
    };
}
