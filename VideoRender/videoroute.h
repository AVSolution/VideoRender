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

		evrType_UNKNOWN = 0xff,
	};

	class IVdieoPublishData {
	public:
		virtual ~IVdieoPublishData() { ; }
		virtual void onPublishData(unsigned long ulTps,std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) = 0;
	};

	class IVideoPublishObserver {
	public:
		IVideoPublishObserver(const std::string &publishStreamId);
		virtual ~IVideoPublishObserver() { ; }

		void onPublishPath(std::string &publishStreamId);

		void onPublishData(IVdieoPublishData* const pObserver);
		void onPublishStatus(eVideoRouteType evrType);

		virtual void onNotifyPublish() = 0;

	protected:
		IVdieoPublishData*	m_pVideoPublishData;
		eVideoRouteType		m_evrPublishType;

	private:
		std::string	m_strPublishStreamId;
	};
	
	class IVideoSubscribeObserver {
	public:
		IVideoSubscribeObserver();
		virtual ~IVideoSubscribeObserver() { ; }

		virtual void onSubscribeStatus(const char* publishSteamID, eVideoRouteType evrType) = 0;
		virtual void onSubscribeData(unsigned long ulTps, const char* publishStreamId, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) = 0;
	};

	bool add_publish(IVideoPublishObserver* const pObserver);
	bool remove_publish(IVideoPublishObserver* const pObserver);
	bool add_subscribe(const char* publishStreamID,IVideoSubscribeObserver* const pObserver);
	bool remove_subscribe(const char* publishStreamID,IVideoSubscribeObserver* const pObserver);

}//namespace videoroute

#endif
