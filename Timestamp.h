/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 12:34:20
 * @LastEditTime: 2024-10-13 17:40:21
 * @Description: 时间类
 * @FilePath: /muduo/muduo/Timestamp.h
 */

#pragma once

#include <iostream>
#include <string>
#include <ctime>

class Timestamp
{
private:
    /**
     * @brief 自1970年1月1日到现在的微秒数字 
     */
    int64_t microSecondsSinceEpoch_;
public:

    /**
     * @brief 默认构造使时间为0，传参构造使时间为传入时间
     */
    Timestamp()
        : microSecondsSinceEpoch_(0) {}
    
    explicit
    Timestamp(int64_t microSecondsSinceEpochArg)
        : microSecondsSinceEpoch_(microSecondsSinceEpochArg) 
    {
        //std::cout << "this is Timestamp(int64_t)" << microSecondsSinceEpoch_ << std::endl;
    }

    /**
     * @brief 交换时间戳
     * @param {Timestamp} &that 交换的对象 
     */
    void swap(Timestamp &that)
    {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    /**
     * @brief 转换microSecondsSinceEpoch_的值
     * @return {string} 以秒为单位的字符串 例如123456.123456
     */    
    std::string toString() const;
    
    /**
     * @brief 打印时间
     * @param {bool} showMicroseconds 是否开启打印毫秒
     * @return {string} 打印详细时间
     */    
    std::string toFormattedString(bool showMicroseconds = true) const;

    /**
     * @brief 检验时间戳是否合法
     * @return {bool} true -> 合法 
     */
    bool valid() const {return microSecondsSinceEpoch_ > 0;}

    /**
     * @brief 获取对象时间
     * @return {int64_t} microSecondsSinceEpoch_
     */    
    int64_t microSecondsSinceEpoch() const {return microSecondsSinceEpoch_;}

    /**
     * @brief 转换 int64_t -> time_t
     * @return {time_t} 当前秒数
     */
    time_t SecondsSinceEpoch()
    {
        return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    }

    /**
     * @brief 微秒换算秒
     */    
    static const int kMicroSecondsPerSecond = 1000 * 1000;
    
    /**
     * @brief: 返回当前时间
     * @return {Timestamp}带参构造 时间为当前微秒数
     */
    static Timestamp now();

    /**
     * @brief 标准失败对象，用于时间比较
     * @return {Timestamp} 默认构造 时间为0
     */
    static Timestamp invalid()
    {
        return Timestamp();
    }

    /**
     * @brief 用秒构造函数
     * @param {time_t} t 当前秒数
     * @return {Timestamp} 带参构造
     */
    static Timestamp fromUnixTime(time_t t)
    {
        //std::cout << "this is Timestamp::fromUnixTime(time_t)" << std::endl;
        return fromUnixTime(t, 0);
    }

    /**
     * @brief 用秒+毫秒构造函数
     * @param {time_t} t 当前秒数
     * @param {int} microseconds 当前毫秒数
     * @return {Timestamp}  带参构造
     */
    static Timestamp fromUnixTime(time_t t, int microseconds)
    {
        //std::cout << "this is Timestamp::fromUnixTime(time_t, int)" << std::endl;
        return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
    }

};


/**
 * @brief 运算符重载
 */
inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

/**
 * @brief 返回时间差异值
 * @param {Timestamp} high  较大时间
 * @param {Timestamp} low   较低时间
 * @return {double}  时间差值
 */
inline double timeDifferent(Timestamp high, Timestamp low)
{
    return static_cast<double>(high.microSecondsSinceEpoch() -low.microSecondsSinceEpoch()) / Timestamp::kMicroSecondsPerSecond;
}

/**
 * @brief 当前时间相加秒数
 * @param {Timestamp} timestamp 当前时间
 * @param {double} seconds 秒数
 * @return {Timestamp} Timestamp类对象
 */
inline Timestamp addTime(Timestamp timestamp, const double seconds)
{
    int64_t tmp = (seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + tmp);
}