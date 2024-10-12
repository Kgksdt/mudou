/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 01:19:50
 * @LastEditTime: 2024-10-13 02:11:46
 * @Description: 提供无法拷贝构造和赋值操作的基类
 * @FilePath: /muduo/noncopyable.h
 */
#pragma once

#include <map>


/**
 * @brief: noncopyable类被继承后，派生类对象可以析构和构造但无法拷贝构造和赋值操作
 */
class noncopyable
{
protected:
    noncopyable() = default;    //默认构造函数
    ~noncopyable() = default;   //默认析构函数
public:
    noncopyable(const noncopyable&) = delete;   //禁止拷贝构造
    void operator=(const noncopyable&) = delete;//禁止赋值操作
};
