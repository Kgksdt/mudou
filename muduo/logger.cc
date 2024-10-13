/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 02:26:43
 * @LastEditTime: 2024-10-13 13:33:08
 * @Description: 
 * @FilePath: /muduo/logger.cc
 */

#include <iostream>

#include "Logger.h"

/**
 * @brief 获取日志唯一实例 (单例模式)
 * @return 返回一个Logger实例的引用
 */
Logger& Logger::instance()
{
    static Logger log;
    return log;
}

/**
 * @brief 设置日志级别
 * @param {int} lev要设置的日志级别
 */
void Logger::setLogLevel(int lev)
{
    logLevel_ = lev;
}

/**
 * @brief 打印信息
 * @param {string} msg 要打印的信息
 *  [级别信息] time : msg
 */
void Logger::log(const std::string& msg)
{
    //打印级别
    switch (logLevel_)
    {
    case INFO:
        std::cout << "[INFO]";
        break;
    case ERROR:
        std::cout << "[ERROR]";
        break;
    case FATAL:
        std::cout << "[FATAL]";
        break;
    case DEBUG:
        std::cout << "[DEBUG]";
        break;
    default:
        break;
    }
    //打印时间
    std::cout << "时间" 
              << " :"
              << msg
              << std::endl;
}