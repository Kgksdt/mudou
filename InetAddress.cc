/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 22:17:06
 * @LastEditTime: 2024-10-13 23:55:11
 * @Description: 地址类对象实现
 * @FilePath: /mymuduo/InetAddress.cc
 */

#include "InetAddress.h"

/**
 * @brief 传入ip和port
 * @param {uint16_t} port 端口
 * @param {string} ip IP地址(默认127.0.0.1)
 */
InetAddress::InetAddress(uint16_t port, std::string ip)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

/**
 * @brief: 返回ip
 * @return {string} ip地址
 */
std::string InetAddress::toIp() const
{
   char buf[64] ={0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    return buf;
}
/**
 * @brief: 返回 ip+端口
 * @return {string} ip+端口
 */
std::string InetAddress::toIpPort() const
{
    char buf[64] ={0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    size_t end = strlen(buf);
    uint16_t port = ntohs(addr_.sin_port);
    sprintf(buf + end, ":%u", port);
    return buf;
}

/**
 * @brief:  返回端口
 * @return {uint16_t} 端口
 */
uint16_t InetAddress::toport() const
{
    return ntohs(addr_.sin_port);
}
