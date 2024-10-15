/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 23:53:55
 * @LastEditTime: 2024-10-15 19:08:45
 * @Description: 事件循环类对象实现
 * @FilePath: /mymuduo/EventLoop.cc
 */
#include <sys/eventfd.h>
#include <fcntl.h>
#include <errno.h>
#include <memory>

#include "Poller.h"
#include "Logger.h"
#include "EventLoop.h"
#include "Channel.h"

/**
 * @brief 防止一个线程创建多个EventLoop thread_local
 */
__thread EventLoop *t_loopInThisThread = nullptr;

/**
 * @brief 定义默认的Poller IO复用超时时间
 */
const int KPollTimeMs = 5000;

/**
 * @brief wakeupfd， 用来notify唤醒subReactor处理新来的channel
 * 第一个参数 0：表示初始计数为 0。
 * EFD_NONBLOCK：以非阻塞模式操作，避免因读写操作而被阻塞。
 * EFD_CLOEXEC：执行 exec 时关闭文件描述符，防止文件描述符泄露到子进程。
 * @return {int} evtfd
 */
int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_FATAL("evenfd error %d \n", errno);
    }
    return evtfd;
}

EventLoop::EventLoop()
    : looping_(false)
    , quit_(false)
    , callingPendingFunctors_(false)
    , threadId_(static_cast<pid_t>(CurrentThread::tid()))
    , poller_(Poller::newDefaultPoller(this))
    , wakeupFd_(createEventfd())
    , wakeupChannel_(new Channel(this, wakeupFd_))
{
    LOG_DEBUG("EventLoop created %p in thread %d \n", this, threadId_);
    if(t_loopInThisThread)
    {
        LOG_FATAL("Another EventLoop %p exists in this thread %d \n", t_loopInThisThread, threadId_);
    }
    else
    {
        t_loopInThisThread = this;
    }

    // 设置wakeupfd 的事件类型以及发生事件后的回调操作
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
}

void EventLoop::loop()
{
    looping_ = true;
    quit_ =false;

    LOG_INFO("EventLoop %p statr looping \n", this);

    while(!quit_)
    {
        activeChannels_.clear();
        //监听client fd 和 wakeupfd
        pollReturnTime_ = poller_->poll(KPollTimeMs, &activeChannels_);
        for(Channel *channel : activeChannels_)
        {
            channel->handleEvent(pollReturnTime_);
        }
        /**
         * IOxiancheng1 mainLoop 接受fd <-channel subloop老用户
         * subloop 实现注册回调cb wakeup唤醒subloop后 
         * 执行mainloop注册cb操作
         * 
         */
        doPendingFunctors();
    }
    LOG_INFO("EventLoop %p stop looping \n", this);
}

void EventLoop::quit()
{
    quit_ = true;
    //如果在其他线程quit主线程,唤醒阻塞
    if(!isInLoopThread())
    {
        wakeup();
    }
}


void EventLoop::runInloop(Functor cb)
{
    if(isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInloop(cb);
    }
}

void EventLoop::queueInloop(Functor cb)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }
    if (!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
    
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if(n != sizeof(one))
    {
        LOG_ERROR("EventLoop::wakeup() writes %lu bytes instead of 8", n);
    }
}

void EventLoop::updataChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}
bool EventLoop::hasChannel(Channel *channel)
{
    return poller_->hasChannel(channel);

}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for(const Functor &functor : functors)
    {
        //执行当前loop需要执行的回调操作
        functor();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n =read(wakeupFd_, &one, sizeof(one));
    if(n != sizeof(one))
    {
        LOG_ERROR("EventLoop::hanleRead() read %ld bytes instead of 8", n);
    }
}
