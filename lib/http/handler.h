//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/9/7
// Time: 下午9:30
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__handler__
#define __hive__handler__

#include "buffer.h"
#include "http.h"
#include "listener.h"

NS_HIVE_BEGIN

class Handler : public RefObject
{
public:
	Handler(void);
	virtual ~Handler(void);

	// 连接相关的接口
	virtual void onReceiveHttp(Http* pHttp) = 0;
	virtual void onCurlResponse(Buffer* pBuffer, uint32 nodeID, uint32 callbackID, bool isRequestOK) = 0;
	virtual void onCloseHttp(Http* pHttp) = 0;
	virtual void onCloseListener(Listener* pListener) = 0;
};// end class Handler

NS_HIVE_END

#endif
