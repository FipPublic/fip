//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/11/22
// Time: 下午10:00
// To change this template use File | Settings | File Templates.
//

#include "active.h"
#include "epoll.h"

NS_HIVE_BEGIN

Active::Active(void) : EpollObject() {

}
Active::~Active(void){
	closeEvent();
}

bool Active::epollActive(uint32 events){
	this->readEvent();
	return true;
}
bool Active::epollIn(void){
    return true;
}
bool Active::epollOut(void){
    return true;
}
void Active::epollRemove(void){

}
void Active::epollCheck(void){

}

int Active::readEvent(void){
    uint64 value;
    read(m_fd, &value, sizeof(uint64));
	this->onActive(value);
	return 0;
}
void Active::closeEvent(void){
	if(m_fd >= 0){
		getEpoll()->objectRemove(this);
		close(m_fd);
		m_fd = -1;
	}
}
bool Active::createEvent(void){
	m_fd = eventfd(0, 0);
	if (m_fd == -1){
		LOG_ERROR("m_fd == -1");
		goto ACTIVE_CREATE_FAILED;
	}
	if (!set_non_blocking(m_fd)){
		LOG_ERROR("set_non_blocking m_fd=%d failed", m_fd);
		goto ACTIVE_CREATE_FAILED;
	}
	if (!getEpoll()->objectAddLT(this, EPOLLIN)){
		LOG_ERROR("objectAddLT failed");
		goto ACTIVE_CREATE_FAILED;
	}
	return true;
ACTIVE_CREATE_FAILED:
	closeEvent();
	return false;
}

NS_HIVE_END