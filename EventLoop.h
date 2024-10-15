/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 23:53:43
 * @LastEditTime: 2024-10-15 18:12:24
 * @Description: 事件循环类
 * @FilePath: /mymuduo/EventLoop.h
 */
#pragma once

#include <functional>
#include <vector>
#include <atomic>
#include <unistd.h>
#include <sys/syscall.h>
#include <memory>
#include <mutex>

#include "noncopyable.h"
#include "Timestamp.h"
#include "CurrentThread.h"

class Channel;
class Poller;

/**
 * @brief 主要包含 Channel Poller
 */
class EventLoop : noncopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    /**
     * @brief 开启事件循环
     */
    void loop();

    /**
     * @brief退出事件循环
     */
    void quit();

    Timestamp pollReturnTime() const { return pollReturnTime_;}

    /**
     * @brief 当前loop执行cb
     * @param Functor cb 回调函数
     */
    void runInloop(Functor cb);

    
    /**
     * @brief 把cb放进队列中 唤醒loop所在的线程 执行cb
     * @param Functor cb
     */
    void queueInloop(Functor cb);

    /**
     * @brief 用来唤醒loop所在线程的
     */
    void wakeup();

    /**
     * @brief EventLoop -> Poller的方法
     * @param Channel *channel
     */
    void updataChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    /**
     * @brief 判断EventLoop对象是否在自己的线程里
     * @return {bool}
     */
    bool isInLoopThread() const {return threadId_ == CurrentThread::tid(); }

private:

    /**
     * @brief 唤醒wakeup
     */
    void handleRead();

    /**
     * @brief 执行回调
     */
    void doPendingFunctors();
    using ChannelList = std::vector<Channel *>;

    /**
     * @brief 原子操作
     */
    std::atomic_bool looping_;

    /**
     * @brief 退出loop循环
     */
    std::atomic_bool quit_;

    /**
     * @brief 记录当前线程的tid
     */
    const pid_t threadId_;

    /**
     * @brief 返回时间
     */
    Timestamp pollReturnTime_;

    std::unique_ptr<Poller> poller_;

    /**
     * @brief 当mainLoop获得一个新用户的channel, 通过轮询算法选择一个subloop,  通过该成员唤醒subloop处理channel
     */
    int wakeupFd_;

    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;

    /**
     * @brief 标识当前loop是否需要执行回调操作
     */
    std::atomic_bool callingPendingFunctors_;

    /**
     * @brief 存储loop需要执行的所有的回调操作
     */
    std::vector<Functor> pendingFunctors_;
    /**
     * @brief 互斥锁 用来保护vector线程安全
     */
    std::mutex mutex_;

};