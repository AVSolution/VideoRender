#include "videoroute.h"
#include "videopublish.h"
#include "videosubscribe.h"

namespace videoroute {

	IVideoPublishObserver::IVideoPublishObserver(const std::string &publishStreamId):
	m_strPublishStreamId(publishStreamId),
		m_pVideoPublishData(nullptr) {
	}

	void IVideoPublishObserver::onPublishPath(std::string &publishStreamId) {
		publishStreamId = m_strPublishStreamId;
	}

	void IVideoPublishObserver::onPublishData(IVdieoPublishData* const pObserver) {
		m_pVideoPublishData = pObserver;
	}

	void IVideoPublishObserver::onPublishStatus(eVideoRouteType evrType) {
		m_evrPublishType = evrType;
	}

	IVideoSubscribeObserver::IVideoSubscribeObserver(const std::string &publishStreamId, const std::string &subscribeStreamId):
	m_strPublishStreamId(publishStreamId), m_strSubscribeStreamId(subscribeStreamId) {
	}

	void IVideoSubscribeObserver::onSubscribePath(std::string &publishStreamId, std::string &subscribeStreamId) {
		publishStreamId = m_strPublishStreamId;
		subscribeStreamId = m_strSubscribeStreamId;
	}

	void IVideoSubscribeObserver::onSubscribeStatus(eVideoRouteType evrType) {
		m_evrSubscribeType = evrType;
	}

	bool add_publish(IVideoPublishObserver* const pObserver) {
		if (pObserver) {
			std::string pStreamId;
			pObserver->onPublishPath(pStreamId);
			if(pStreamId.length())
				return CVideoPublish::getInstance()->add_publish_stream(pStreamId.c_str(), pObserver);
		}
	}

	bool remove_publish(IVideoPublishObserver* const pObserver) {
		if (pObserver) {
			std::string pStreamId;
			pObserver->onPublishPath(pStreamId);
			if(pStreamId.length())
				return CVideoPublish::getInstance()->remove_publish_stream(pStreamId.c_str());
		}
	}

	bool add_subscribe(IVideoSubscribeObserver* const pObserver) {
		if (pObserver) {
			std::string publishStreamId, subscribeStreamId;
			pObserver->onSubscribePath(publishStreamId, subscribeStreamId);
			if(publishStreamId.length() && subscribeStreamId.length())
				return CVideoSubscribe::getInstance()->add_subscribe_stream(publishStreamId.c_str(),subscribeStreamId.c_str(),pObserver);
		}
	}

	bool remove_subscribe(IVideoSubscribeObserver* const pObserver) {
		if (pObserver) {
			std::string publishStreamId, subscribeStreamId;
			pObserver->onSubscribePath(publishStreamId, subscribeStreamId);
			if (publishStreamId.length() && subscribeStreamId.length())
				return CVideoSubscribe::getInstance()->remove_subscribe_stream(pObserver);
		}
	}

}//namespace videoroute