/*
 * @Author: Kgksdt
 * @Date: 2024-10-15 23:21:56
 * @LastEditTime: 2024-10-16 00:13:47
 * @Description: 
 * @FilePath: /mymuduo/EventLoopThread.cc
 */

#include "EventLoopThread.h"
#include "EventLoop.h"
/**
 * @brief 构造函数
 * @param cb 可选参数，线程初始化回调函数
 * @param name 可选参数，线程名称
 */
EventLoopThread::EventLoopThread
    (const ThreadInitCallback &cb, 
     const std::string &name)
        : loop_(nullptr)
        , exiting_(false)
        , thread_(std::bind(&EventLoopThread::threadFunc, this), name)
        , mutex_()
        , cond_()
        , callback_(cb)
{

}

/**
 * @brief 析构函数，负责资源清理
 */
EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if (loop_ != nullptr)
    {
        loop_->quit();
        thread_.join();
    }
}

/**
 * @brief 启动线程并返回 EventLoop 实例
 * @return EventLoop* 指向启动的线程中 EventLoop 对象的指针
 */
EventLoop * EventLoopThread::startLoop()
{
    //启动新线程
    thread_.start();

    EventLoop *loop = nullptr;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop_ == nullptr)
        {
            cond_.wait(lock);
        }
        loop = loop_;
    }
    return loop_;
}

/**
 * @brief 在单独的新线程里面的运行的
 */
void EventLoopThread::threadFunc()
{
    EventLoop loop; 
    if (callback_)
    {
        callback_(&loop);
    }
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop();
    std::unique_lock<std::mutex> lock(mutex_);
    loop_ = nullptr;
}