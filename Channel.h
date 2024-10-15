/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 00:00:50
 * @LastEditTime: 2024-10-15 17:56:56
 * @Description: 类似于通道
 * @FilePath: /mymuduo/Channel.h
 */
#pragma once

#include <functional>
#include <memory>

#include "noncopyable.h"
#include "Timestamp.h"
class EventLoop;

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
    EventLoop *loop_;

    /**
     * @brief Poller监听对象
     */
    const int fd_;

    /**
     * @brief 注册fd感兴趣的事件
     */
    int events_;

    /**
     * @brief poller返回的具体发生的事件
     */
    int revents_;

    /**
     * @brief eventloop里的channel状态
     */
    int index_;

    /**
     * @brief 监听其他描述符 
     */
    std::weak_ptr<void> tie_;
    
    /**
     * @brief 是否在监听状态
     */
    bool tied_;

public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    /**
     * @brief fd得到poller后处理事件 
     * @param Timestamp receiveTime 回调函数
     */
    void handleEvent(Timestamp receiveTime);

    void setReadCallback(ReadEventCallback cb) {readCallback_ = std::move(cb);}

    void setWriteCallback(EventCallback cb) {writeCallback_ = std::move(cb);}

    void setcloseCallback(EventCallback cb) {closeCallback_ = std::move(cb);}

    void seterrorCallback(EventCallback cb) {errorCallback_ = std::move(cb);}



    /**
     * @brief 设置fd的事件
     * const int Channel::KNoneEvent = 0;
     * const int Channel::KReadEvent = EPOLLIN | EPOLLPRI;
     * const int Channel::KWriteEvent = EPOLLOUT;
     */
    void enableReading() { events_ |= KReadEvent; update();}
    void disableReading() {events_ &= ~KReadEvent; update();}
    void enableWritng() {events_ |= KWriteEvent; update();}
    void disableWritng() {events_ &= ~KWriteEvent; update();}
    void disableAll() {events_=KNoneEvent; update();}

    void set_revents(int revt) { revents_ = revt;}

    void set_index(int idx) {index_ = idx;}

    int fd() const {return fd_;}

    int index() {return index_;}

    int events() const {return events_;}

    bool isNoneEvent() const {return events_ == KNoneEvent; }

    bool isWriting() const {return events_ & KWriteEvent;}

    bool isReading() const {return events_ & KReadEvent;}

    EventLoop *ownerloop(){ return loop_;}
    
    void remove();

    void tie(const std::shared_ptr<void> &obj);

private:

    void update();
    
    /**
     * @brief 根据poller通知的channel发生的具体事件，由channel回调函数
     * @param Timestamp receiveTime 时间
     */
    void handleEventWithGuard(Timestamp receiveTime);
    /**
     * @brief: 因为channel通道里面能够获知fd最终发生的具体的事件revents，所以负责调用具体事件的回调函数
     */
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

};