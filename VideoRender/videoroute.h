#ifndef __VIDEOROUTE_H__
#define __VIDEOROUTE_H__

namespace videoroute {

	class IVideoPublishObserver {
	public:
		virtual ~IVideoPublishObserver();

		virtual void onPublishStatus(bool bstatus) = 0;
		virtual void onPublishStream(const char* pStreamId) = 0;
		virtual void onPublishData(std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) = 0;
	};

	class IVideoSubscribeObserver {
	public:
		virtual ~IVideoSubscribeObserver();
	};

	bool add_publish(const char* pStreamId, IVideoPublishObserver* pObserver);
	bool remove_publish(const char* pStreamId);
	bool add_subscribe(const char* pStreamId, IVideoSubscribeObserver* pObserver);
	bool remove_subscribe(const char* pStreamId);

}//namespace videoroute

#endif
