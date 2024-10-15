/*
 * @Author: Kgksdt
 * @Date: 2024-10-16 00:17:56
 * @LastEditTime: 2024-10-16 00:38:30
 * @Description: 
 * @FilePath: /mymuduo/EventLoopThreadpool.h
 */
#pragma once

#include <functional>
#include <string>
#include <vector>
#include <memory>

class EventLoop;
class EventLoopThread;


class EventLoopThreadpool
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    EventLoopThreadpool(EventLoop *baseLoop, const std::string &nameArg);
    ~EventLoopThreadpool();

    void setThreadNum(int numThreads) {numThreads_ = numThreads;}
    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();

    bool started() const {return started_;}
    const std::string name() const { return name_; }
private:
    EventLoop *baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};