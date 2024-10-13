/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 12:34:16
 * @LastEditTime: 2024-10-13 17:34:33
 * @Description: 时间类对象实现
 * @FilePath: /muduo/muduo/Timestamp.cc
 */

#include <inttypes.h> 
#include <sys/time.h>

#include "Timestamp.h"

/**
 * @brief 转换microSecondsSinceEpoch_的值
 * @return {string} 以秒为单位的字符串 例如123456.123456
 */  
std::string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

/**
 * @brief 打印时间
 * @param {bool} showMicroseconds 是否开启打印毫秒
 * @return {string} 打印详细时间
 */    
std::string Timestamp::toFormattedString(bool showMicroseconds) const
{
    //std::cout << "this is toFormattedString" << microSecondsSinceEpoch_  << std::endl;
    struct tm tm_time;//时间结构体
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    localtime_r(&seconds, &tm_time);//将 time_t 类型的时间转换为 UTC
    //std::cout <<seconds << std::endl;
    if(showMicroseconds)
    {
        int microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
        snprintf(buf, sizeof(buf),
        "%04d/%02d/%02d  %02d:%02d:%02d:%06d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
        microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf),
        "%04d/%02d/%02d  %02d:%02d:%02d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    
    return buf;
}

Timestamp Timestamp::now()
{
    return fromUnixTime(time(nullptr));
}
