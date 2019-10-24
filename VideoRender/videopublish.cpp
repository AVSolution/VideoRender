#include "videopublish.h"

namespace videoroute {

	CVideoPublish::CVideoPublish() {

	}

	CVideoPublish::~CVideoPublish() {

	}

	bool CVideoPublish::add_stream(const char* pStreamId, videoroute::IVideoPublishObserver* pObserver) {
		return true;
	}

	bool CVideoPublish::remote_stream(const char* pStreamId) {
		return true;
	}
}//namespace videoroute

IMPLEMENT_SINGLETON(videoroute::CVideoPublish)