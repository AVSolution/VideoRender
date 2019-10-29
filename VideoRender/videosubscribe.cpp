#include "videosubscribe.h"
#include "videopublish.h"

namespace videoroute {
	
	CVideoSubscribe::CVideoSubscribe() {

	}

	CVideoSubscribe::~CVideoSubscribe() {

	}

	bool CVideoSubscribe::add_subscribe_stream(const char* pPublishStreamId, const char* pSubscribeStreamId, IVideoSubscribeObserver* pObserver) {
		if (pObserver) {
			std::lock_guard<std::mutex> autoLock(m_mutex);
			std::shared_ptr<CVideoPublishImpl> videopublishImpl = CVideoPublish::getInstance()->getVideoPublishImpl(pPublishStreamId);
			
			videopublishImpl->add_subscribe(pObserver);
			m_mapSubscribe.insert(make_pair(pObserver, videopublishImpl));
		}
		
		return true;
	}

	bool CVideoSubscribe::remove_subscribe_stream(IVideoSubscribeObserver* pObserver) {
		if (pObserver) {
			std::lock_guard<std::mutex> autoLock(m_mutex);
			auto it = m_mapSubscribe.find(pObserver);
			if (m_mapSubscribe.end() != it) {
				it->first->onSubscribeStatus(evrType_Subscribe_Stop);
				m_mapSubscribe.erase(it);
			}
		}

		return true;
	}

}//namespace videoroute

IMPLEMENT_SINGLETON(videoroute::CVideoSubscribe)