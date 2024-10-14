/*
 * @Author: Kgksdt
 * @Date: 2024-10-14 20:22:55
 * @LastEditTime: 2024-10-14 21:14:33
 * @Description: 
 * @FilePath: /mymuduo/Poller.cc
 */
#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop)
    : ownerLoop_(loop) {}

bool Poller::hasChannel(Channel *channel) const
{
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}