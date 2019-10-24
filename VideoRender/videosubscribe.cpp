#include "videosubscribe.h"

namespace videoroute {
	
	CVideoSubscribe::CVideoSubscribe() {

	}

	CVideoSubscribe::~CVideoSubscribe() {

	}

	bool CVideoSubscribe::add_stream(const char* pStreamId, IVideoSubscribeObserver* pObserver) {
		return true;
	}

	bool CVideoSubscribe::remote_stream(const char* pStreamId) {
		return true;
	}
}//namespace videoroute

IMPLEMENT_SINGLETON(videoroute::CVideoSubscribe)