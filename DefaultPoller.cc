/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 21:23:31
 * @LastEditTime: 2024-10-14 22:35:39
 * @Description:实现newDefaultPoller
 * @FilePath: /mymuduo/DefaultPoller.cc
 */

#include <stdlib.h>

#include "Poller.h"

Poller* Poller::newDefaultPoller(EventLoop *loop)
{
    if(::getenv("MUDUO_USE_POLL"))
    {
        //生成poll的实例
        return nullptr;
    }
    else
    {
        //生成epoll的实例
        return nullptr;
    }
}
