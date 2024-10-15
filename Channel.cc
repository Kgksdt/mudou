/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 00:00:46
 * @LastEditTime: 2024-10-15 18:52:31
 * @Description: 类对象函数实现
 * @FilePath: /mymuduo/Channel.cc
 */

#include <sys/epoll.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"

const int Channel::KNoneEvent = 0;
const int Channel::KReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::KWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop)
    , fd_(fd)
    , events_(0)
    , revents_(0)
    , index_(-1)
    , tied_(false) {}

Channel::~Channel() {}

void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ =obj;
    tied_ = true;
}

void Channel::update()
{
    // 通过channel所属的EventLoop, 调用EventLoop方法
    loop_->updataChannel(this);
}

void Channel::remove()
{
    // 通过channel所属的EventLoop, 调用EventLoop方法
    loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    if(tied_)
    {
        std::shared_ptr<void> guard = tie_.lock();
        if(guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{

    LOG_INFO("channel handleEvent revents : %d \n", revents_);

    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }

    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}