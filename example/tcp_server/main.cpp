//
// Created by linhf on 8/19/24.
//

#include <iostream>
#include <thread>
#include "../../lib/tcp/tcp.h"


void handlerConn(TcpConn *conn) {
    char * buf = conn->ReadBytes();
    if (buf ==nullptr) {
        conn->Close();
        return;
    }
    printf("server accept msg: %s\n", buf);
    long writeLen =  conn->WriteBytes((char*)"server accept msg");
    printf("server write msg len: %ld\n",writeLen);
    delete conn;
}

int main() {
    auto *tcpServer = new TcpServer();
    tcpServer->BindAddress((char *)"0.0.0.0");
    tcpServer->BindPort(8099);
    tcpServer->MaxConnection(10);

    int isRun = tcpServer->Run();
    if (isRun == -1) {
        printf("run tcp server failed");
        return -1;
    }


    while (true) {
        TcpConn* conn = tcpServer->Accept();
        if (conn == nullptr) {
            continue;
        }
        std::thread handlerThread(handlerConn, conn);
        handlerThread.detach();
    }

    delete tcpServer;
    return 0;
}
