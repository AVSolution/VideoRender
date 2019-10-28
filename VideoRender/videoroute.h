#ifndef __VIDEOROUTE_H__
#define __VIDEOROUTE_H__

#include <memory>
#include <string>

namespace videoroute {

	enum eVideoRouteType {
		evrType_null,
		evrType_Publish_Init,
		evrType_Publish_Start,
		evrType_Publish_Stop,
		evrType_Publish_unInit,
		evrType_Subscribe_Start,
		evrType_Subscribe_Stop,
	};

	class IVdieoPublishData {
	public:
		virtual ~IVdieoPublishData() { ; }
		virtual void onPublishData(unsigned long ulTps, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) = 0;
	};

	class IVideoPublishObserver {
	public:
		IVideoPublishObserver(const std::string &publishStreamId);
		virtual ~IVideoPublishObserver() { ; }

		void onPublishPath(std::string &publishStreamId);

		void onPublishData(IVdieoPublishData* const pObserver);
		void onPublishStatus(eVideoRouteType evrType);

	protected:
		IVdieoPublishData*	m_pVideoPublishData;

	private:
		std::string	m_strPublishStreamId;
		eVideoRouteType		m_evrPublishType;
	};
	
	class IVideoSubscribeObserver {
	public:
		IVideoSubscribeObserver(const std::string &publishStreamId, const std::string &subscribeStreamId);
		virtual ~IVideoSubscribeObserver() { ; }

		void onSubscribePath(std::string &publishStreamId, std::string &subscribeStreamId);

		void onSubscribeStatus(eVideoRouteType evrType);

		virtual void onSubscribeData(unsigned long ulTps, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) = 0;

	private:
		std::string	m_strPublishStreamId;
		std::string m_strSubscribeStreamId;
		eVideoRouteType	m_evrSubscribeType;
	};

	bool add_publish(IVideoPublishObserver* const pObserver);
	bool remove_publish(IVideoPublishObserver* const pObserver);
	bool add_subscribe(IVideoSubscribeObserver* const pObserver);
	bool remove_subscribe(IVideoSubscribeObserver* const pObserver);

}//namespace videoroute

#endif
