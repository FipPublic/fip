//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 2017/7/28
// Time: 上午9:14
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__mainworker__
#define __hive__mainworker__

#include "common.h"
#include "epoll.h"
#include "timer.h"
#include "log.h"
#include "http.h"
#include "buffer.h"
#include "listener.h"
#include "multicurl.h"
#include "handler.h"
#include "active.h"

NS_HIVE_BEGIN

#define EPOLL_READ_BUFFER_SIZE 8192

#define HTTP_HANDLE_TIMEOUT 5000
#define COMMAND_HTTP_REQUEST 2

class EpollActive : public Active
{
public:
    EpollActive(void);
    virtual ~EpollActive(void);

    // from Active
    virtual void onActive(uint64 value);
public:

};

typedef struct HttpBackData{
    uint32 request_id;
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
}HttpBackData;

class MainWorker : public RefObject, public Thread, public Sync
{
public:
	char m_pReadBuffer[EPOLL_READ_BUFFER_SIZE];
	EpollActive* m_pEpollActive;
	Epoll* m_pEpoll;
	Timer* m_pTimer;
	Handler* m_pHandler;
	ListenerPool* m_pListenerPool;
	HttpPool* m_pHttpPool;
	MultiCurl* m_pMultiCurl;
    http_parser_settings m_settings;
    std::string m_listenIP;
    unsigned int m_listenPort;
    uint32 m_callbackIDIncrease;
    std::vector<RequestData*> m_requestQueue;
    std::vector<HttpBackData> m_sendBackQueue;
public:
	MainWorker(void);
	virtual ~MainWorker(void);

	DEFINE_INSTANCE_H(MainWorker)

	static void onAcceptHttp(int fd, const char* ip, uint16 port, Listener* pListener);

    // From Thread
    virtual int threadFunction(void);

	void onReceiveHttp(Http* pHttp);
	Http* openHttp(int fd, const char* ip, uint16 port);
	Http* getHttp(uint32 handle);
	bool closeHttp(Http* pHttp);

	Listener* openHttpListener(const char* ip, uint16 port);
	Listener* openListener(const char* ip, uint16 port, AcceptSocketFunction pFunc, bool isNeedEncrypt, bool isNeedDecrypt);
	Listener* getListener(uint32 handle){
		return (Listener*)m_pListenerPool->get(handle);
	}
	bool closeListener(Listener* pListener);

    // 对外发送接口
    void sendHttpPOST(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers, uint32& callbackID);
    void sendHttpGET(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers, uint32& callbackID);
    void sendBackToRequest(uint32 request_id, int status_code, const std::string& body, const std::map<std::string, std::string>& headers);
    void dispatchMessage(void);

	bool sendCurlRequest(RequestData* pRequest);
	void onCurlResponse(Buffer* pBuffer, uint32 nodeID, uint32 callbackID, bool isRequestOK);

	void update(void);
	void initialize(void);
	void destroy(void);

	void initializeHttp(void);
	void setListenerAddress(const std::string& ip, uint32 port){
	    m_listenIP = ip;
	    m_listenPort = port;
	}
	inline void setHandler(Handler* pHandler){ m_pHandler = pHandler; }
	inline Handler* getHandler(void){ return m_pHandler; }
	inline http_parser_settings* getSettings(void) { return &m_settings; }
	inline MultiCurl* getMultiCurl(void){ return m_pMultiCurl; }
	inline char* getReadBuffer(void){ return m_pReadBuffer; }
	inline Epoll* getEpoll(void){ return m_pEpoll; }
	inline Timer* getTimer(void){ return m_pTimer; }
	uint32 getNextCallbackID(void){
	    ++m_callbackIDIncrease;
	    if(m_callbackIDIncrease == 0){
	        m_callbackIDIncrease = 1;
	    }
	    return m_callbackIDIncrease;
	}
};

NS_HIVE_END

#endif
