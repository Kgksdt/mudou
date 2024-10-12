/*
TcpServer 服务器
TcpClient 客户端 
epoll + 线程池

1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务类的构造函数当中，注册处理链接的回调函数和处理读写事件的回调函数
5.设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop* loop,//事件循环
            const muduo::net::InetAddress& listenAddr,//ip + port
            const std::string& nameArg) // 服务器的名字
            : _server(loop, listenAddr, nameArg)
            , _loop(loop)
    {
        //给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(
            std::bind(
            &ChatServer::onConnection, 
            this, 
            std::placeholders::_1));
        
        //给服务器注册用户读写事件回调
        _server.setMessageCallback(
            std::bind(
            &ChatServer::onMessage,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));

        //设置服务端的连接数量
        _server.setThreadNum(4);
    }
    //开启事件循环
    void start()
    {
        _server.start();
    }
private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn)
    {
        if(conn->connected())
        {   
            std::cout << conn->peerAddress().toIpPort()
                      << "->"
                      << conn->localAddress().toIpPort()
                      << "state:online"
                      << std::endl;
        }
        else
        {
            std::cout << conn->peerAddress().toIpPort()
                                << "->"
                                << conn->localAddress().toIpPort()
                                << "state:online"
                                << std::endl;
            conn->shutdown();
            
        }
    }
    void onMessage(const muduo::net::TcpConnectionPtr& conn,//连接
                            muduo::net::Buffer* buffer,//缓冲区
                            muduo::Timestamp time)//时间信息
    {
        std::string buf = buffer->retrieveAllAsString();
        std::cout << "recv data:"
                  << buf
                  << "time:"
                  << time.toString()
                  << std::endl;
        conn->send(buf);
        
    }
    muduo::net::TcpServer _server;//组合TcpServer对象
    muduo::net::EventLoop *_loop;//创建EvertLoop事件循环对象的指针

}; 

int main()
{
    muduo::net::EventLoop loop;
    muduo::net::InetAddress addr("127.0.0.1", 8889);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();//epoll_wait以阻塞方式等待新用户连接，已连接用户读写事件等

    return 0;
}