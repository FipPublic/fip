//
// Created by linhf on 8/16/24.
//
#include <sys/socket.h>
#include <algorithm>
#include <netinet/in.h>
#include <unistd.h>
#include "tcp.h"


int TcpServer::Run() {

    this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1) {
        return -1;
    }

    struct sockaddr_in bindAddr{};
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port = htons(this->port);

    if (bind(listenFd, (struct sockaddr *) &bindAddr, sizeof(bindAddr)) == -1) {
        return -1;
    }

    if (listen(listenFd, SOMAXCONN) == -1) {
        return -1;
    }

    return 0;
}

TcpConn* TcpServer::Accept() {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = ::accept(this->listenFd,(struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        return nullptr;
    }

    return new TcpConn(clientFd);
}


char* TcpConn::ReadBytes() {
    char buf[256];
    int len;

    len = read(this->_clientFd,buf,sizeof(buf)-1);
    buf[len] = '\n';
    return buf;
}

int TcpConn::WriteBytes(char* bytes) {
    return 0;
}

void TcpConn::Close() {

}

