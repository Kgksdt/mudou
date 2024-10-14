/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 20:22:50
 * @LastEditTime: 2024-10-14 21:10:00
 * @Description: poller类
 * @FilePath: /mymuduo/Poller.h
 */
#pragma once 

#include <vector>
#include <unordered_map>

#include "Timestamp.h"
#include "noncopyable.h"

class Channel; 
class EventLoop;

class Poller : noncopyable
{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop *loop);

    virtual ~Poller() = default;

    /**
     * @brief: 给所有io复用保留统一的接口
     * @param int timeoutMs 超时时间
     * @param channelList *activeChannels 活动种的Channe
     * @return Timestamp
     */
    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;

    /**
     * @brief  变更事件
     * @param Channel *channel 当前需要变更事件
     */
    virtual void updateChannel(Channel *channel) = 0;

    /**
     * @brief 删除事件
     * @param Channel *channel 需要删除事件
     */
    virtual void removeChannel(Channel *channel) = 0;

    /**
     * @brief 判断当前channel是否在Poller
     * @param Channel *channel
     * @return bool}
     */
    bool hasChannel(Channel *channel) const;

    /**
     * @brief 通过该接口获得默认的IO复用的具体实现
     * @param EventLoop *loop
     * @return Poller*
     */
    static Poller* newDefaultPoller(EventLoop *loop);
protected:

    /**
     * @brief 将sockfd 和 channel 绑定
     * @param int key -> sockfd
     * @param Channel value -> Channel
     */
    using ChannelMap = std::unordered_map<int, Channel*>;
    ChannelMap channels_;

private:
    /**
     * @brief Poller 所属的事件循环EventLoop
     */
    EventLoop *ownerLoop_;
};