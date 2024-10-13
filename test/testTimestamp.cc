/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 17:08:55
 * @LastEditTime: 2024-10-13 17:26:30
 * @Description: 
 * @FilePath: /muduo/test/testTimestamp.cc
 */
#include <iostream>
#include "../muduo/Timestamp.h"

int main()
{
    std::cout << Timestamp::now().toFormattedString() << std::endl;
}