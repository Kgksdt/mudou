/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 00:00:50
 * @LastEditTime: 2024-10-14 00:23:12
 * @Description: 类似于通道
 * @FilePath: /mymuduo/Channel.h
 */
#pragma once

#include <functional>
#include <memory>

#include "noncopyable.h"
#include "Timestamp.h"
class EvenLoop;

/**
 * @brief: 封装了sockfd和感兴趣的event，如epollin， eopllout事件 绑定了poller返回的具体事件
 */
class Channel : noncopyable
{
private:
    static const int KNoneEvent;
    static const int KReadEvent;
    static const int KWriteEvent;

    /**
     * @brief 事件循环
     */
    EvenLoop *loop_;

    /**
     * @brief Poller监听对象
     */
    const int fd_;

    /**
     * @brief 注册fd感兴趣的事件
     */
    int events;

    /**
     * @brief poller返回的具体发生的事件
     */
    int revents;

    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;

public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EvenLoop *loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

private:
    /**
     * @brief: 因为channel通道里面能够获知fd最终发生的具体的事件revents，所以负责调用具体事件的回调函数
     */
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

};