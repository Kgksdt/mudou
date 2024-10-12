/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 01:19:50
 * @LastEditTime: 2024-10-13 02:11:46
 * @Description: 
 *      noncopyable类被继承后，派生类对象可以析构和构造
 *      但无法拷贝构造和赋值操作
 * @FilePath: /muduo/noncopyable.h
 */
#pragma once
#include <map>
class noncopyable
{
protected:
    noncopyable() = delete;
    ~noncopyable() = delete;
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
};
