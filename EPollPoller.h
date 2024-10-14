/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 22:34:52
 * @LastEditTime: 2024-10-15 02:58:30
 * @Description: EPollpoller
 * @FilePath: /mymuduo/EPollPoller.h
 */
#pragma once

#include <vector>
#include <sys/epoll.h>

#include "Poller.h"

class Channel;
/**
 * @brief: 
 * epoll的使用
 * epoll_create EPollPoller
 * epoll_ctl updateChannel removeChannel
 * epoll_wait poll
 */
class EPollPoller : public Poller
{
public:
    EPollPoller(EventLoop *loop);
    ~EPollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;
private:

    /**
     * @brief 初始长度
     */
    static const int KInitEventListSize = 16;
    
    /**
     * @brief: 
     * @param int numEvents 事件编号
     * @param ChannelList *activeChannels 监听容器
     */
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;

    /**
     * @brief 更新channel
     * @param int operation
     * @param Channel *channel
     */
    void update(int operation, Channel *channel);

    using EventList = std::vector<epoll_event>;

    int epollfd_;
    
    /**
     * @brief 监视文件容器
     */
    EventList events_;
};