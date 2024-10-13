/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 17:08:55
 * @LastEditTime: 2024-10-14 00:55:09
 * @Description: 
 * @FilePath: /mymuduo/test/testTimestamp.cc
 */
#include <iostream>
#include <memory>
#include <functional>
#include "Timestamp.h"

using AddCallback = std::function<int(int, int)>;

void add(int a, int b, AddCallback addcallback)
{
    std::cout << "a + b = " << a + b << " a * b = " << addcallback(a,b) << std::endl;
}
int main()
{
    add(3,4,[](int x, int y){return x * y;});
}
