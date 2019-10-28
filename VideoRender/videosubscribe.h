#ifndef __VIDEOSUBSCRIBE_H__
#define __VIDEOSUBSCRIBE_H__

#include "SingletonImpl.h"
#include "videoroute.h"
#include "videopublish.h"
#include <map>
#include <mutex>

namespace videoroute {

	class CVideoSubscribe :public util::CSingleTon<CVideoSubscribe> {
	public:
		CVideoSubscribe();
		~CVideoSubscribe();

		bool add_subscribe_stream(const char* pPublishStreamId, const char* pSubscribeStreamId,IVideoSubscribeObserver* pObserver);
		bool remove_subscribe_stream(IVideoSubscribeObserver* pObserver);
		
	private:
		std::mutex	m_mutex;
		std::map<IVideoSubscribeObserver*, std::shared_ptr<CVideoPublishImpl>> m_mapSubscribe;
	};

}//namespace videoroute

#endif
