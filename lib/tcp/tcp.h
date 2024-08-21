//
// Created by linhf on 8/16/24.
//

#include <vector>

class TcpConn {
private:
    int _clientFd;
public:
    /**
     * constructors
     * @param clientFd
     */
    explicit TcpConn(int clientFd) {
        this->_clientFd = clientFd;
    }
    /**
     * read tcp msg
     * @return tcp msg char*
     */
    char* ReadBytes();
    /**
     * write rcp msg
     * @param bytes
     * @return write len
     */
    int WriteBytes(char* bytes);
    /**
     * Close tcp conn
     */
    void Close();

};



/**
 * tcp server
 */
class TcpServer {
private:
    char* address;
    int port;
    int maxConn;
    int connCount;
    std::vector<int> clientFds;
    int listenFd;
public:
    /**
     * constructors
     */
    TcpServer() {
        this->address = (char *)"0.0.0.0";
        this->port = 0;
        this->maxConn = 10;
        this->connCount = 0;
        this->listenFd = -1;
    }
    /**
     * set tcp server listen address
     * @param _address
     */
    void BindAddress(char *_address);
    /**
     * set tcp server listen port
     * @param _port
     */
    void BindPort(int _port);
    /**
     *set tcp server max connect size
     * @param size
     */
    void MaxConnection(int size);
    /**
     * Run tcp server
     * @return -1 is failed,0 is success
     */
    int Run();
    /**
     * get a tcp client conn
     * @return TcpConn
     */
    TcpConn * Accept();
};

void TcpServer::BindAddress(char *_address) {
    this->address = _address;
}

void TcpServer::BindPort(int _port) {
    this->port = _port;
}

void TcpServer::MaxConnection(int size) {
    this->maxConn = size;
}