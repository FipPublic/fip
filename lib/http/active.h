//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/11/22
// Time: 下午10:00
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__active__
#define __hive__active__

#include "log.h"
#define LOG_DEBUG log_debug
#define LOG_INFO log_info
#define LOG_ERROR log_error

#include "epoll.h"
#include <sys/eventfd.h>

NS_HIVE_BEGIN

class Active : public EpollObject
{
public:
	Active(void);
	virtual ~Active(void);

	// from EpollObject
	virtual bool epollActive(uint32 events);
	virtual bool epollIn(void);
	virtual bool epollOut(void);
	virtual void epollRemove(void);
	virtual void epollCheck(void);

	virtual void onActive(uint64 value) = 0;

    bool createEvent(void);
	inline bool active(void){
		uint64 value = 1;
		int nwrite = write(m_fd, &value, sizeof(uint64));
		if(nwrite != sizeof(uint64)){
			LOG_ERROR("write failed m_fd=%d", m_fd);
			return false;
		}
//		fprintf(stderr, "--Active::wakeup m_fd write %llu\n", value);
		return true;
	}
protected:
	int readEvent(void);
	void closeEvent(void);
protected:

};

NS_HIVE_END

#endif