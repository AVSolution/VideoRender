#include "videoroute.h"
#include "videopublish.h"
#include "videosubscribe.h"

namespace videoroute {

	bool add_publish(const char* pStreamId, IVideoPublishObserver* pObserver) {
		return CVideoPublish::getInstance()->add_stream(pStreamId, pObserver);
	}

	bool remove_publish(const char* pStreamId) {
		return CVideoPublish::getInstance()->remote_stream(pStreamId);
	}

	bool add_subscribe(const char* pStreamId, IVideoSubscribeObserver* pObserver) {
		return CVideoSubscribe::getInstance()->add_stream(pStreamId,pObserver);
	}

	bool remove_subscribe(const char* pStreamId) {
		return CVideoSubscribe::getInstance()->remote_stream(pStreamId);
	}

}//namespace videoroute