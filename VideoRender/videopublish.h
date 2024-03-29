#ifndef __VIDEOPUBLISH_H__
#define __VIDEOPUBLISH_H__

#include "SingletonImpl.h"
#include "videoroute.h"
#include <string>
#include <list>
#include <mutex>
#include <map>

namespace videoroute {

	class CVideoPublishImpl;
	class CVideoSubscribe;
	class CVideoPublish :public util::CSingleTon<CVideoPublish> {
	public:
		CVideoPublish();
		~CVideoPublish();

	public:
		bool add_publish_stream(const char* pStreamId, IVideoPublishObserver* pObserver);
		bool remove_publish_stream(const char* pStreamId);
		bool add_subscribe_stream(const char* publishStreamId,IVideoSubscribeObserver* pObserver);
		bool remove_subscribe_stream(const char* publishStreamId, IVideoSubscribeObserver* pObserver);

		std::shared_ptr<CVideoPublishImpl>& getVideoPublishImpl(const std::string &publishStream);

		std::mutex	m_mutex;
		std::map<std::string, std::shared_ptr<CVideoPublishImpl>> m_mapPublish;
	};

	class CVideoPublishImpl: public IVdieoPublishData {
	public:
		friend CVideoSubscribe;
		CVideoPublishImpl();
		CVideoPublishImpl(const std::string& streamId, IVideoPublishObserver* pObserver);
		~CVideoPublishImpl();

		bool remove_publish();
		bool add_subscribe(IVideoSubscribeObserver* pObserver);
		bool remove_subscribe(IVideoSubscribeObserver* pObserver);
		bool reset_publish_observer(IVideoPublishObserver* pObserver);

	protected:
		virtual void onPublishData(unsigned long ulTps, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) override;

	private:
		eVideoRouteType		   m_evrType;
		std::string			   m_strStreamId;
		IVideoPublishObserver* m_pObserver;
		std::mutex				m_mutex;
		std::list<IVideoSubscribeObserver*>	m_listSubObserver;
	};

}//namespace videoroute

#endif