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
		onNotifyPublish();
	}

	IVideoSubscribeObserver::IVideoSubscribeObserver(){
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

	bool add_subscribe(const char* publishStreamID,IVideoSubscribeObserver* const pObserver) {
		if (pObserver && publishStreamID && strlen(publishStreamID))
			return CVideoPublish::getInstance()->add_subscribe_stream(publishStreamID, pObserver);
	}

	bool remove_subscribe(const char* publishStreamID, IVideoSubscribeObserver* const pObserver) {
		if (pObserver && publishStreamID && strlen(publishStreamID))
			return CVideoPublish::getInstance()->remove_subscribe_stream(publishStreamID, pObserver);
	}

}//namespace videoroute