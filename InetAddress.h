/*
 * @Author: Kgksdt
 * @Date: 2024-10-13 22:17:10
 * @LastEditTime: 2024-10-13 23:54:47
 * @Description: 地址类
 * @FilePath: /mymuduo/InetAddress.h
 */
#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <strings.h>
#include <string.h>
#include <iostream>
/**
 * @brief 地址类
 */
class InetAddress
{
private:
    /**
     * @brief: 地址
     * struct sockaddr_in 
     * {
     * sa_family_t  sin_family;  // 地址族（地址类型），IPv4 通常为 AF_INET
     * in_port_t    sin_port;    // 端口号，使用网络字节序 (htons)
     * struct       in_addr 
     * {
     *      uint32_t s_addr;     // IPv4 地址，使用网络字节序
     * };                       
     * char         sin_zero[8]; // 填充位，保持与 struct sockaddr 相同大小
     * };
     * htons     主机port -> 网络port
     * htonl     主机Ip   -> 网络Ip 
     * ntohs     网络port -> 主机port
     * ntohl     网络Ip   -> 主机Ip
     * inet_pton Ip地址   -> 二进制网络Ip
     * inet_ntop 二进制网络Ip -> Ip地址
     */
    sockaddr_in addr_;
public:

   
    /**
     * @brief 传入ip和port
     * @param {uint16_t} port 端口
     * @param {string} ip IP地址(默认127.0.0.1)
     */
    explicit
    InetAddress(uint16_t port, std::string ip = "127.0.0.1");

    /**
     * @brief: sockaddr构造
     * @param {sockaddr_in} &addr 
     */
    explicit 
    InetAddress(const sockaddr_in &addr) 
        :addr_(addr) {}


    /**
     * @brief: 返回ip
     * @return {string} ip地址
     */
    std::string toIp() const;

    /**
     * @brief: 返回 ip+端口
     * @return {string} ip+端口
     */
    std::string toIpPort() const;

    /**
     * @brief:  返回端口
     * @return {uint16_t} 端口
     */
    uint16_t toport() const;
    
    const sockaddr_in* getSockaddr() const {return &addr_;}
};

