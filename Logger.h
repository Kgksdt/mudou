/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 02:26:37
 * @LastEditTime: 2024-10-13 13:32:04
 * @Description: 日志
 * @FilePath: /muduo/Logger.h
 */
#pragma once

#include <string>

#include "noncopyable.h"

/**
 * @brief 定义日志级别
 */
enum LogLevel
{
    INFO,   //0 普通信息  
    ERROR,  //1 错误信息  
    FATAL,  //2 core信息  
    DEBUG,  //3 调试信息  
};

/**
 * @brief 日志类
 */
class Logger : noncopyable
{
private:
   /**
    * @brief 单例模式拷贝构造私有化, 防止外部创建实例
    */
   Logger(){}

    /**
     * @brief 日志级别
     */
    int logLevel_;

public:
    /**
     * @brief 获取日志唯一实例 (单例模式)
     * @return 返回一个Logger实例的引用
     */
    static Logger& instance();

    /**
     * @brief 设置日志级别
     * @param {int} lev要设置的日志级别
     */
    void setLogLevel(int lev);

    /**
     * @brief 打印信息
     * @param {string} msg 要打印的信息
     *  [级别信息] time : msg
     */
    void log(const std::string& msg);

};

/**
 * @brief 记录信息级别的日志
 * 
 * 这些宏用于记录信息级别的日志信息。它会获取日志实例，
 * 如设置日志级别为 INFO，然后使用 snprintf 格式化日志消息。
 * 最后，将格式化后的消息发送到日志系统。
 *
 * @param logmsgFormat 日志消息的格式字符串
 * @param ...         可变参数，格式化字符串所需的参数
 * 
 * @example
 * LOG_INFO("this is a/an %s", exp); ->  this is a/an "example"
 */
#define LOG_INFO(logmsgFormat, ...)                         \
    do                                                      \
    {                                                       \
        Logger &logger = Logger::instance();                \
        logger.setLogLevel(INFO);                           \
        char buf[1024] = {0};                               \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);   \
        logger.log(buf);                                    \
    } while (0);

#define LOG_ERROR(logmsgFormat, ...)                        \
    do                                                      \
    {                                                       \
        Logger &logger = Logger::instance();                \
        logger.setLogLevel(ERROR);                          \
        char buf[1024] = {0};                               \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);   \
        logger.log(buf);                                    \
    } while (0);

#define LOG_FATAL(logmsgFormat, ...)                        \
    do                                                      \
    {                                                       \
        Logger &logger = Logger::instance();                \
        logger.setLogLevel(FATAL);                          \
        char buf[1024] = {0};                               \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);   \
        logger.log(buf);                                    \
    } while (0);


/**
 * @brief: MUDUODEBUG开关是否输出debug
 */
#ifdef MUDUODEBUG
#define LOG_DEBUG(logmsgFormat, ...)                        \
    do                                                      \
    {                                                       \
        Logger &logger = Logger::instance();                \
        logger.setLogLevel(DEBUG);                          \
        char buf[1024] = {0};                               \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);   \
        logger.log(buf);                                    \
    } while (0);

#else
    #define LOG_DEBUG(logmsgFormat, ...)
#endif

