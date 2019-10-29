#include "videoFile.h"
#include <Windows.h>
#include "VideoConv.h"

namespace videofile {

	CVideoFile::CVideoFile():pfile_(nullptr),
	evType_(evType_NULL),
	bRunning_(false) ,
	IVideoPublishObserver(nullptr){
	}

	CVideoFile::CVideoFile(const char* pStrPublish,const char* pStrFilePath, eVideoFileType evType):
	IVideoPublishObserver(pStrPublish),
	evType_(evType),
	pfile_(nullptr),
	bRunning_(false),
	nFrameLen_(0),
	nFrameWidth_(0),
	nFrameHeight_(0) {
		pfile_ = fopen(pStrFilePath, "rb");
	}

	CVideoFile::~CVideoFile() {
		bRunning_ = false;
		if (pfile_)
			fclose(pfile_);
		pfile_ = nullptr;
	}

	void CVideoFile::startRead(int nWidth, int nHeight) {
		
		int nFrameLen = 0;
		switch (evType_) {
		case evType_I420: {
			nFrameLen_ = nWidth * nHeight * 3 / 2;
		} break;
		case evType_BMP: {
			nFrameLen_ = nWidth * nHeight * 3;
		} break;
		case evType_RGB24: {
			nFrameLen_ = nWidth * nHeight * 3;
		} break;
		default:break;
		}

		if (nFrameLen_ && pfile_) {
			nFrameWidth_ = nWidth;
			nFrameHeight_ = nHeight;
			videobuffer_ = std::shared_ptr<uint8_t>(new uint8_t[nFrameLen_]);
		}

		if (pfile_) {
			bRunning_ = true;
			threadReadFile_ = std::thread(&CVideoFile::ThreadRead, this);
		}
	}

	void CVideoFile::stopRead() {
		bRunning_ = false;
	}

	void CVideoFile::onNotifyPublish() {
		if (m_evrPublishType == evrType_Publish_Start)
			startRead(640, 480);
		else if (m_evrPublishType == evrType_Publish_Stop || evrType_Publish_unInit == m_evrPublishType)
			stopRead();
	}

	void CVideoFile::ThreadRead() {
		
		std::shared_ptr<uint8_t> bufferTemp;
		std::shared_ptr<uint8_t> bufferTemp1;
		if (evType_I420 == evType_) {
			bufferTemp.reset(new uint8_t[nFrameLen_ * 2]);
			bufferTemp1.reset(new uint8_t[nFrameLen_ * 2]);
		}
		
		while (bRunning_) {
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			if (pfile_ &&  (evrType_Publish_Start == m_evrPublishType) && nFrameLen_ ) {
				int nRes = fread(videobuffer_.get(),1,nFrameLen_,pfile_);
				if (nRes <= 0)
					fseek(pfile_, 0, SEEK_SET);

				//to do
				DWORD dwtps = timeGetTime();
				if (m_pVideoPublishData) {
					if (evType_I420 == evType_) {
						util::CVideoConv::I420ToRGB24(videobuffer_.get(), bufferTemp.get(),nFrameLen_ * 2,nFrameWidth_,nFrameHeight_);
						util::CVideoConv::VerMirror_RGB24(bufferTemp.get(), bufferTemp1.get(), nFrameWidth_, nFrameHeight_);
						if(m_pVideoPublishData)
							m_pVideoPublishData->onPublishData(dwtps, bufferTemp1, nFrameLen_ * 2, nFrameWidth_, nFrameHeight_);
					}
					else if (evType_RGB24 == evType_) {
						m_pVideoPublishData->onPublishData(dwtps, videobuffer_, nFrameLen_, nFrameWidth_, nFrameHeight_);
					}
				}
			}
		}
	}
}//namespace videofile.