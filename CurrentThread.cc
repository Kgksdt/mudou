/*
 * @Author: Kgksdt
 * @Date: 2024-10-15 13:14:44
 * @LastEditTime: 2024-10-15 15:01:33
 * @Description:
 * @FilePath: /mymuduo/CurrentThread.cc
 */

#include "CurrentThread.h"


namespace CurrentThread
{
    __thread int t_cachedTid = 0;

    void cacheTid()
    {
        if( t_cachedTid == 0)
        {
            t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }
}