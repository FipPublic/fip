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

    int isRun = tcpServer->Run();
    if (isRun == -1) {
        printf("run tcp server failed");
        return -1;
    }

    TcpConn* conn = tcpServer->Accept();
    while (true) {
        char * buf = conn->ReadBytes();
        if (buf == nullptr) {
            continue;
        }
        printf("%s,%lu\n", buf, sizeof("server accept msg"));
        long writeLen =  conn->WriteBytes("server accept msg");
        printf("%ld\n",writeLen);
    }

    return 0;
}