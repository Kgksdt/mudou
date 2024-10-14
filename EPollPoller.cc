/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 22:36:46
 * @LastEditTime: 2024-10-15 02:59:03
 * @Description: 
 * @FilePath: /mymuduo/EPollPoller.cc
 */

#include <errno.h>
#include <unistd.h>
#include <cstring>

#include "Channel.h"
#include "EPollPoller.h"
#include "Logger.h"

const int KNew = -1;//未添加
const int KAdded = 1;//已经添加
const int KDelete = 2;//删除

/**
 *               EventLoop 
 *  ChannelList             Poller
 *  std::vector<Channel*>    ChannelMap <fd, channel*> epollfd
 * EventLoop 包含一个 Poller 对象，用于处理底层 I/O 事件。
 * EventLoop 维护一个 ChannelList，以便管理所有注册的 Channel。
 * 当 Poller 检测到某个文件描述符的事件发生时，它将事件信息返回给 EventLoop，然后 EventLoop 根据返回的事件查找相应的 Channel。
 * Channel 是与文件描述符相关联的封装，负责处理特定的事件（如可读、可写等），并且它持有事件处理的回调函数。
 */
EPollPoller::EPollPoller(EventLoop *loop)
    : Poller(loop)
    , epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , events_(KInitEventListSize)
{
    if (epollfd_ < 0)
    {
        LOG_FATAL("epoll_create error: %d \n", errno);
    }
}

EPollPoller::~EPollPoller()
{
    ::close(epollfd_);
}

Timestamp EPollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    LOG_INFO("func = %s => total count: %ld \n",
              __FUNCTION__,
              channels_.size());
    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno;
    Timestamp now(Timestamp::now());

    if (numEvents > 0)
    {
        LOG_INFO("%d events happend \n", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if (numEvents == 0)
    {
        LOG_DEBUG("%s timeout! \n", __FUNCTION__);
    }
    else{
        if (saveErrno != EINTR)
        {
            errno = saveErrno;
            LOG_ERROR("EPollPoller::poll err!");
        }
    }
    return now;
}

void EPollPoller::updateChannel(Channel *channel)
{
    const int index = channel->index();
    LOG_INFO("func = %s =>fd = %d events = %d index = %d \n",
              __FUNCTION__,
              channel->fd(), 
              channel->events(), 
              channel->index());
    if(index == KNew || index == KDelete)
    {
        int fd = channel->fd();
        if(index == KNew)
        {
            channels_[fd] = channel;
        }
        channel->set_index(KAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        int fd = channel->fd();
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(KDelete);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EPollPoller::removeChannel(Channel *channel)
{
    int fd = channel->fd();
    int index = channel->index();
    LOG_INFO("func = %s =>fd = %d \n",
              __FUNCTION__,
              fd);
    channels_.erase(fd);
    if (index == KAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(KNew);
}

void EPollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for(int i = 0; i < numEvents; ++i)
    {
        Channel *channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void EPollPoller::update(int operation, Channel *channel)
{
    int fd = channel->fd();
    struct epoll_event event;
    memset(&event,0, sizeof(event));
    event.data.ptr = channel;
    event.data.fd = fd; 
    event.events = channel->events();
   
    if(::epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
         if(operation == EPOLL_CLOEXEC)
        {
            LOG_ERROR("epoll_ctl del error: %d \n", errno);
        
        }
        else
        {
            LOG_FATAL("epoll_ctl del FATAL: %d \n", errno);
        }
    }
}
    