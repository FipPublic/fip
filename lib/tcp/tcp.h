//
// Created by linhf on 8/16/24.
//

#include <vector>

class tcpConn {
private:
    int _clientFd;
public:
    int clientFd();
    /**
     * read tcp msg
     * @return
     */
    char* readBytes();
    /**
     * write rcp msg
     * @param bytes
     * @return
     */
    int writeBytes(char* bytes);
    /**
     * close tcp conn
     */
    void close();
};

/**
 * tcp server
 */
class tcpServer {
private:
    char* address;
    int port;
    int maxConn;
    int connCount;
    std::vector<int> clientFds;
    int listenFd;
public:
    /**
     * set tcp server listen address
     * @param _address
     */
    void bindAddress(char *_address);
    /**
     * set tcp server listen port
     * @param _port
     */
    void bindPort(int _port);
    /**
     *set tcp server max connect size
     * @param size
     */
    void maxConnection(int size);
    /**
     * run tcp server
     * @return -1 is failed,0 is success
     */
    int run();
    /**
     * get a tcp client conn
     * @return tcpConn
     */
    tcpConn* accept();
};

void tcpServer::bindAddress(char *_address) {
    this->address = _address;
}

void tcpServer::bindPort(int _port) {
    this->port = _port;
}

void tcpServer::maxConnection(int size) {
    this->maxConn = size;
}