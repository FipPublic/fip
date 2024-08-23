//
// Created by linhf on 8/16/24.
//
#include <sys/socket.h>
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "tcp.h"


int TcpServer::Run() {

    // 获取fd
    this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1) {
        return -1;
    }

    // 定义网络参数
    struct sockaddr_in bindAddr{};
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port = htons(this->port);

    // 将fd绑定到网络参数中
    if (bind(listenFd, (struct sockaddr *) &bindAddr, sizeof(bindAddr)) == -1) {
        return -1;
    }

    // fd监听网络请求
    if (listen(listenFd, SOMAXCONN) == -1) {
        return -1;
    }

    return 0;
}

TcpConn* TcpServer::Accept() const {

    // 定义客户端配置
    struct sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

    // 接收请求并记录客户端fd
    int clientFd = ::accept(this->listenFd,(struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        return nullptr;
    }

    return new TcpConn(clientFd);
}

char* TcpConn::ReadBytes() const {
    char buf[256];
    long len;
read_fd:
    len = read(this->_clientFd, buf, sizeof(buf) - 1);
    if (len <= 0) {
        goto read_fd;
    }
    buf[len] = '\0';
    char* result = new char[len + 1];
    std::memcpy(result, buf, len + 1);
    return result;
}

long TcpConn::WriteBytes(char* bytes) const {
    return write(this->_clientFd,bytes, strlen(bytes));
}

void TcpConn::Close() {
    delete this;
}

