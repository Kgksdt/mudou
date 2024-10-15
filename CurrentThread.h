/*
 * @Author: Kgksdt
 * @Date: 2024-10-15 13:14:37
 * @LastEditTime: 2024-10-15 13:22:34
 * @Description: 
 * @FilePath: /mymuduo/CurrentThread.h
 */

#pragma once

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread
{
    extern  __thread int t_cachedTid;

    void cacheTid();

    inline int tid()
    {
        if (__builtin_expect(t_cachedTid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedTid;
    }
}