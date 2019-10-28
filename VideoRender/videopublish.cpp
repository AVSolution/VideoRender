#include "videopublish.h"
#include "videosubscribe.h"

namespace videoroute {

	CVideoPublish::CVideoPublish() {
	}

	CVideoPublish::~CVideoPublish() {
		m_mapPublish.clear();
	}

	bool CVideoPublish::add_publish_stream(const char* pStreamId, videoroute::IVideoPublishObserver* pObserver) {
		std::lock_guard<std::mutex> autoLock(m_mutex);
		auto it = m_mapPublish.find(pStreamId);
		if (m_mapPublish.end() == it) {
			std::shared_ptr<CVideoPublishImpl> publishImp = std::make_shared<CVideoPublishImpl>(pStreamId,pObserver);
			m_mapPublish.insert(make_pair(pStreamId, publishImp));
		}
		else {
			return it->second->reset_publish_observer(pObserver);
		}

		return true;
	}

	bool CVideoPublish::remove_publish_stream(const char* pStreamId) {
		std::lock_guard<std::mutex> autoLock(m_mutex);
		auto it = m_mapPublish.find(pStreamId);
		if (m_mapPublish.end() != it) {
			it->second.reset();
			m_mapPublish.erase(it);
		}

		return true;
	}

	std::shared_ptr<CVideoPublishImpl>& CVideoPublish::getVideoPublishImpl(const std::string &publishStream) {
		std::lock_guard<std::mutex> autoLock(m_mutex);
		auto it = m_mapPublish.find(publishStream);
		if (m_mapPublish.end() != it) {
			return it->second;
		}

		return std::shared_ptr<CVideoPublishImpl>(nullptr);
	}

	CVideoPublishImpl::CVideoPublishImpl() {
	}

	CVideoPublishImpl::CVideoPublishImpl(const std::string& streamId, IVideoPublishObserver* pObserver):
	m_strStreamId(streamId),
	m_pObserver(pObserver),
	m_evrType(evrType_null) {
		m_evrType = evrType_Publish_Init;
		if (m_pObserver) {
			m_pObserver->onPublishStatus(m_evrType);
			m_pObserver->onPublishData(this);
		}
	}

	CVideoPublishImpl::~CVideoPublishImpl() {

		for (auto &it: m_listSubObserver) {
			CVideoSubscribe::getInstance()->remove_subscribe_stream(it);
		}
		if (m_pObserver) {
			m_pObserver->onPublishData(nullptr);
			m_pObserver->onPublishStatus(evrType_Publish_unInit);
		}

		m_pObserver = nullptr;
		m_strStreamId.clear();
	}

	bool CVideoPublishImpl::add_subscribe(IVideoSubscribeObserver* pObserver) {
		std::lock_guard <std::mutex> autoLock(m_mutex);
		if (pObserver) {
			auto it = find(m_listSubObserver.begin(), m_listSubObserver.end(), pObserver);
			if (m_listSubObserver.end() == it) {
				m_listSubObserver.push_back(pObserver);
				if (m_pObserver) {
					m_pObserver->onPublishStatus(evrType_Publish_Start);
					pObserver->onSubscribeStatus(evrType_Subscribe_Start);
				}
			}
		}

		return true;
	}

	bool CVideoPublishImpl::reset_publish_observer(IVideoPublishObserver* pObserver) {
		std::lock_guard<std::mutex> autoLock(m_mutex);
		if (nullptr != pObserver) {
			m_pObserver = pObserver;

			decltype(m_listSubObserver.size()) num = m_listSubObserver.size();
			if (num >= 1)
				m_evrType = evrType_Publish_Start;

			m_pObserver->onPublishStatus(m_evrType);
			m_pObserver->onPublishData(this);
			
			//range for notify subscribe observer.
 			for (auto &it : m_listSubObserver) {
 				it->onSubscribeStatus(evrType_Subscribe_Start);
 			}
			return true;
		}
		else if (pObserver == m_pObserver)
			return false;//replace
		else
			;//logic error,should user different streamId

		return false;
	}

	void CVideoPublishImpl::onPublishData(unsigned long ulTps, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) {
		std::lock_guard<std::mutex> autoLock(m_mutex);
 		for (auto &it : m_listSubObserver)
			it->onSubscribeData(ulTps, buffer, nBufferLen, nWidth, nHeight);
	}

}//namespace videoroute

IMPLEMENT_SINGLETON(videoroute::CVideoPublish)