/*
 * @Author: Kgksdt
 * @Date: 2024-10-15 19:14:37
 * @LastEditTime: 2024-10-15 23:33:01
 * @Description: 线程类，用于封装线程创建、启动、销毁等功能
 * @FilePath: /mymuduo/thread.h
 */
#pragma once

#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>
#include <sys/syscall.h>
#include <semaphore.h>

#include "noncopyable.h"
#include "CurrentThread.h"

/**
 * @brief 线程类，用于创建、启动和管理一个线程对象
 */
class Thread : noncopyable
{
public:
    /**
     * @brief 定义线程执行函数类型
     */
    using ThreadFunc = std::function<void()>;

    /**
     * @brief 构造函数，接受线程执行函数和线程名称
     * @param func 线程的执行函数
     * @param name 可选参数，线程名称
     */
    explicit Thread (ThreadFunc func, const std::string &name = std::string());

    /**
     * @brief 析构函数，如果线程已启动且未 join，则进行分离
     */
    ~Thread ();

    /**
     * @brief 启动线程
     */
    void start();

    /**
     * @brief 阻塞等待线程执行完成
     */
    void join();
     
    /**
     * @brief 检查线程是否已启动
     * @return bool 返回线程的启动状态
     */
    bool started() const {return started_;}

    /**
     * @brief 获取线程的进程 ID
     * @return pid_t 线程的进程 ID
     */
    pid_t tid() const {return tid_;}

    /**
     * @brief 获取线程名称
     * @return const std::string& 线程的名称
     */
    const std::string& name() const {return name_;}

    /**
     * @brief 获取已创建线程的总数
     * @return int 返回已创建的线程总数
     */
    static int numCreated() {return numCreated_;}

private:

    /**
     * @brief 设置线程默认名称
     */
    void setDefaultName();

    bool started_;                           /**< 线程启动状态 */
    bool joined_;                            /**< 线程 join 状态 */
    std::shared_ptr<std::thread> thread_;    /**< std::thread 智能指针 */
    pid_t tid_;                              /**< 线程的进程 ID */
    ThreadFunc func_;                        /**< 线程的执行函数 */
    std::string name_;                       /**< 线程名称 */
    static std::atomic_int32_t numCreated_;  /**< 已创建线程的计数 */
};
