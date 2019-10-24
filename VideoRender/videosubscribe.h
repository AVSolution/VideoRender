#ifndef __VIDEOSUBSCRIBE_H__
#define __VIDEOSUBSCRIBE_H__

#include "SingletonTem.h"
#include "videoroute.h"

namespace videoroute {

	class CVideoSubscribe :public util::CSingleTon<CVideoSubscribe> {
	public:
		CVideoSubscribe();
		~CVideoSubscribe();

		bool add_stream(const char* pStreamId, IVideoSubscribeObserver* pObserver);
		bool remote_stream(const char* pStreamId);
	};

}//namespace videoroute

#endif
