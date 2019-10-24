#ifndef __VIDEOPUBLISH_H__
#define __VIDEOPUBLISH_H__

#include "SingletonTem.h"
#include "videoroute.h"

namespace videoroute {

	class CVideoPublish :public util::CSingleTon<CVideoPublish> {
	public:
		CVideoPublish();
		~CVideoPublish();

	public:
		bool add_stream(const char* pStreamId, IVideoPublishObserver* pObserver);
		bool remote_stream(const char* pStreamId);
	};
}//namespace videoroute

#endif