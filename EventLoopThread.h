/*
 * @Author: Kgksdt
 * @Date: 2024-10-15 23:22:13
 * @LastEditTime: 2024-10-15 23:35:18
 * @Description: 管理单个 EventLoop 的线程类，用于创建和管理独立的线程执行事件循环
 * @FilePath: /mymuduo/EventLoopThread.h
 */
#pragma once

#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

#include "noncopyable.h"
#include "thread.h"

class EventLoop;

/**
 * @brief 用于管理单个 EventLoop 的线程类，支持线程初始化回调函数
 */
class EventLoopThread : noncopyable
{
public:
    /**
     * @brief 定义线程初始化的回调函数类型，用于在线程启动时执行特定操作
     */
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    /**
     * @brief 构造函数，创建一个 EventLoopThread 实例
     * @param cb 可选参数，线程初始化回调函数，用于在 EventLoop 启动前执行初始化
     * @param name 可选参数，线程名称，便于识别
     */
    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(), const std::string &name = std::string());

    /**
     * @brief 析构函数，负责资源清理并确保线程安全退出
     */
    ~EventLoopThread();

    /**
     * @brief 启动线程并返回 EventLoop 实例指针
     * @return EventLoop* 指向线程中 EventLoop 对象的指针
     */
    EventLoop *startLoop();

private:
    /**
     * @brief 在单独的新线程里面的运行的
     */
    void threadFunc();

    EventLoop *loop_;                       /**< 指向线程中运行的 EventLoop 实例的指针 */
    bool exiting_;                           /**< 标记线程是否退出的标志 */
    Thread thread_;                          /**< 线程对象，用于执行线程操作 */
    std::mutex mutex_;                       /**< 互斥锁，用于线程间资源保护 */
    std::condition_variable cond_;           /**< 条件变量，用于线程间的同步 */
    ThreadInitCallback callback_;            /**< 线程初始化回调函数 */
};
