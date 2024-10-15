/*
 * @Author: Kgksdt
 * @Date: 2024-10-15 19:14:48
 * @LastEditTime: 2024-10-15 19:51:06
 * @Description: 
 * @FilePath: /mymuduo/thread.cc
 */
#include "thread.h"
std::atomic<int32_t> Thread::numCreated_ {0};

Thread::Thread (ThreadFunc func, const std::string &name)
    : started_(false)
    , joined_(false)
    , tid_(0)
    , func_(std::move(func))
    , name_(name)
{

}
Thread::~Thread () 
{
    if(started_ && !joined_)
    {
        thread_->detach();//分离线程
    }
}


void Thread::start() //一个thread对象就是一个线程的详细信息
{
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    thread_ = std::shared_ptr<std::thread>(new std::thread([&](){
        //获取tid
        tid_ = CurrentThread::tid();
        //开启一个新线程, 专门执行该线程。
        func_();
    }));

    sem_wait(&sem);
}
void Thread::join()
{
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName()
{
    int num = ++numCreated_;
    if(name_.empty())
    {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}