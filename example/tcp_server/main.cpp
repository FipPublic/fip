//
// Created by linhf on 8/19/24.
//

#include <iostream>
#include "../../lib/tcp/tcp.h"

int main() {
    auto *tcpServer = new TcpServer();
    tcpServer->BindAddress((char *)"0.0.0.0");
    tcpServer->BindPort(8099);
    tcpServer->MaxConnection(10);

    TcpConn* conn = tcpServer->Accept();
    while (true) {
        std::cout<< conn->ReadBytes() <<std::endl;
    }


    return 0;
}