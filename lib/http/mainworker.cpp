//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 2017/7/28
// Time: 上午9:14
// To change this template use File | Settings | File Templates.
//

#include "mainworker.h"

NS_HIVE_BEGIN

EpollActive::EpollActive(void) : Active(){

}
EpollActive::~EpollActive(void){

}

// from Active
void EpollActive::onActive(uint64 value){

}

DEFINE_INSTANCE_CPP(MainWorker)

MainWorker::MainWorker(void) : RefObject(), Thread(), Sync(),
    m_pEpollActive(NULL), m_pEpoll(NULL), m_pTimer(NULL), m_pHandler(NULL), m_pListenerPool(NULL),
    m_pHttpPool(NULL), m_pMultiCurl(NULL), m_listenPort(0), m_callbackIDIncrease(0) {

}
MainWorker::~MainWorker(void){

}
int MainWorker::threadFunction(void){
    HiveNS::MainWorker::getInstance()->initialize();
//    HiveNS::MainWorker::getInstance()->openHttpListener("0.0.0.0", 9090);
    HiveNS::MainWorker::getInstance()->openHttpListener(m_listenIP.c_str(), m_listenPort);
    update();
    return 0;
}

void MainWorker::onAcceptHttp(int fd, const char* ip, uint16 port, Listener* pListener){
    LOG_DEBUG("MainWorker::onAcceptHttp fd=%d ip=%s port=%d", fd, ip, port);
	MainWorker::getInstance()->openHttp(fd, ip, port);
}

void MainWorker::sendHttpPOST(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers, uint32& callbackID){
    callbackID = getNextCallbackID();
    RequestData* pRequest = new RequestData("POST", url.c_str(), body.c_str(), body.length(), 0, callbackID);
    std::map<std::string, std::string>::const_iterator itHead = headers.begin();
    for(; itHead != headers.end() ;itHead++){
        pRequest->appendHeader(itHead->first.c_str(), itHead->second.c_str());
    }
    pRequest->retain();
    pRequest->retain();
	// LOG_DEBUG("sendHttpPOST callbackID=%u", callbackID);
    lock();
    m_requestQueue.push_back(pRequest);
    unlock();
    pRequest->release();
    m_pEpollActive->active();
}
void MainWorker::sendHttpGET(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers, uint32& callbackID){
    callbackID = getNextCallbackID();
    RequestData* pRequest = new RequestData("GET", url.c_str(), body.c_str(), body.length(), 0, callbackID);
    std::map<std::string, std::string>::const_iterator itHead = headers.begin();
    for(; itHead != headers.end() ;itHead++){
        pRequest->appendHeader(itHead->first.c_str(), itHead->second.c_str());
    }
    pRequest->retain();
    pRequest->retain();
    lock();
    m_requestQueue.push_back(pRequest);
    unlock();
    pRequest->release();
    m_pEpollActive->active();
}
void MainWorker::sendBackToRequest(uint32 request_id, int status_code, const std::string& body, const std::map<std::string, std::string>& headers){
    HttpBackData data;
    data.body = body;
    data.request_id = request_id;
    data.status_code = status_code;
    data.headers = headers;
    lock();
    m_sendBackQueue.push_back(data);
    unlock();
    m_pEpollActive->active();
}
void MainWorker::dispatchMessage(void){
    std::vector<RequestData*> requestQueue;
    std::vector<HttpBackData> sendBackQueue;
    lock();
    m_requestQueue.swap(requestQueue);
    m_sendBackQueue.swap(sendBackQueue);
    unlock();
    for(int i=0; i<(int)requestQueue.size(); ++i){
        RequestData* pRequest = requestQueue[i];
        m_pMultiCurl->acceptRequest(pRequest);
        pRequest->release();
    }
    for(int i=0; i<(int)sendBackQueue.size(); ++i){
        HttpBackData& data = sendBackQueue[i];
        Http* pHttp = getHttp(data.request_id);
        if(NULL == pHttp){
            LOG_ERROR("dispatchMessage pHttp not found request_id=%d", data.request_id);
        }else{
            pHttp->responseBody(data.body.c_str(), data.body.length(), data.status_code, data.headers);
#ifdef TRACE_MODE
			LOG_INFO("handle=%d,us:%lld", pHttp->getHandle(), get_time_us());
#endif
        }
    }
}

void MainWorker::onReceiveHttp(Http* pHttp){
	m_pHandler->onReceiveHttp(pHttp);
}
Http* MainWorker::openHttp(int fd, const char* ip, uint16 port){
	if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
		LOG_ERROR("openHttp set none block failed fd=%d ip=%s port=%d", fd, ip, port);
		return 0;
	}
	Http* pHttp = m_pHttpPool->create();
	if(NULL == pHttp){
		LOG_ERROR("create http NULL == pHttp");
		return 0;
	}
	pHttp->initialize();
	pHttp->setEpoll(m_pEpoll);
	pHttp->setSocketFD(fd);
	pHttp->setSocket(ip, port);
	pHttp->setState(HTTP_STATE_READ);
	pHttp->setTimer(HTTP_HANDLE_TIMEOUT, getTimer());
#ifdef TRACE_MODE
	LOG_INFO("handle=%d,us:%lld", pHttp->getHandle(), get_time_us());
#endif
	// 读取数据
	pHttp->epollIn();
	return pHttp;
}
Http* MainWorker::getHttp(uint32 handle){
	return m_pHttpPool->get(handle);
}
bool MainWorker::closeHttp(Http* pHttp){
	LOG_DEBUG("handle=%d", pHttp->getHandle());
	m_pHandler->onCloseHttp(pHttp);
	pHttp->resetData();
    return m_pHttpPool->idle(pHttp);
}
Listener* MainWorker::openHttpListener(const char* ip, uint16 port){
	return this->openListener(ip, port, MainWorker::onAcceptHttp, false, false);
}
Listener* MainWorker::openListener(const char* ip, uint16 port, AcceptSocketFunction pFunc, bool isNeedEncrypt, bool isNeedDecrypt){
	LOG_DEBUG("try to open Listener ip=%s port=%d", ip, port);
	Listener* pListener = (Listener*)m_pListenerPool->create();
	if(NULL == pListener){
		LOG_ERROR("NULL == pListener");
		return NULL;
	}
	pListener->setEpoll(m_pEpoll);
	pListener->setSocket(ip, port);
	pListener->setAcceptSocketFunction(pFunc);
	if( !pListener->openSocket() ){
		closeListener(pListener);
		LOG_ERROR("Listener openSocket failed");
		return NULL;
	}
	if( !m_pEpoll->objectAdd(pListener, EPOLLIN) ){
		pListener->closeSocket();
		closeListener(pListener);
		LOG_ERROR("Listener objectAdd to epoll failed");
		return NULL;
	}
	pListener->setIsNeedEncrypt(isNeedEncrypt);
	pListener->setIsNeedDecrypt(isNeedDecrypt);
	LOG_INFO("handle=%d fd=%d ip=%s port=%d", pListener->getHandle(), pListener->getSocketFD(), ip, port);
	return pListener;
}
bool MainWorker::closeListener(Listener* pListener){
	LOG_DEBUG("handle=%d", pListener->getHandle());
	m_pHandler->onCloseListener(pListener);
	m_pEpoll->objectRemove(pListener);
	pListener->resetData();
	return m_pListenerPool->idle(pListener);
}
bool MainWorker::sendCurlRequest(RequestData* pRequest){
	return m_pMultiCurl->acceptRequest(pRequest);
}
void MainWorker::onCurlResponse(Buffer* pBuffer, uint32 nodeID, uint32 callbackID, bool isRequestOK){
	m_pHandler->onCurlResponse(pBuffer, nodeID, callbackID, isRequestOK);
}

void MainWorker::update(void){
	LOG_INFO("start MainWorker");
	int64 timeout;
	while(1){
		timeout = m_pTimer->getWaitTimeout();
		if(m_pMultiCurl->isBusy()){
            if(timeout > 10 || timeout < 0){
                timeout = 10;
            }
		}
		m_pEpoll->update(timeout);
		m_pTimer->update();
		m_pMultiCurl->timerCallback();
		dispatchMessage();
	}
	LOG_INFO("exit MainWorker");
}
void MainWorker::initialize(void){
	if(NULL == m_pEpoll){
		m_pEpoll = new Epoll();
		m_pEpoll->retain();
		if( !m_pEpoll->createEpoll() ){
			LOG_ERROR("createEpoll failed");
//			exit(0);
		}
	}
	if(NULL == m_pEpollActive){
	    m_pEpollActive = new EpollActive();
	    m_pEpollActive->setEpoll(m_pEpoll);
	    if(!m_pEpollActive->createEvent()){
	        LOG_ERROR("EpollActive createEvent failed");
	    }
	}
	if(NULL == m_pTimer){
		m_pTimer = new Timer();
		m_pTimer->retain();
		m_pTimer->start(MAX_0824_HASH_SIZE);
	}
	if(NULL == m_pListenerPool){
		m_pListenerPool = new ListenerPool();
		m_pListenerPool->retain();
	}
	if(NULL == m_pHttpPool){
		m_pHttpPool = new HttpPool();
		m_pHttpPool->retain();
		m_pHttpPool->setMaxHashNumber(MAX_0824_HASH_SIZE);
	}
	if(NULL == m_pMultiCurl){
		m_pMultiCurl = new MultiCurl();
		m_pMultiCurl->retain();
		m_pMultiCurl->setEpoll(m_pEpoll);
		m_pMultiCurl->initialize();
	}
	initializeHttp();
}
void MainWorker::initializeHttp(void){
	http_parser_settings_init(&m_settings);
	m_settings.on_message_begin 	= Http::onMessageBegin;
	m_settings.on_url 				= Http::onUrl;
	m_settings.on_header_field 		= Http::onHeaderField;
	m_settings.on_header_value 		= Http::onHeaderValue;
	m_settings.on_headers_complete 	= Http::onHeadersComplete;
	m_settings.on_body 				= Http::onBody;
	m_settings.on_message_complete 	= Http::onMessageComplete;
}
void MainWorker::destroy(void){
	SAFE_RELEASE(m_pHttpPool)
	SAFE_RELEASE(m_pMultiCurl)
	SAFE_RELEASE(m_pListenerPool)
	SAFE_DELETE(m_pEpollActive)
	SAFE_RELEASE(m_pEpoll)
	SAFE_RELEASE(m_pTimer)
}


NS_HIVE_END
