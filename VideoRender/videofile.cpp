#include "videoFile.h"

namespace videofile {

	CVideoFile::CVideoFile():pfile_(nullptr),
	evType_(evType_NULL),
	bRunning_(false) {
	}

	CVideoFile::CVideoFile(const char* pStrFilePath, eVideoFileType evType):
	evType_(evType),
	pfile_(nullptr),
	bRunning_(false) {
		pfile_ = fopen(pStrFilePath, "rb");
	}

	CVideoFile::~CVideoFile() {
		bRunning_ = false;
		if (pfile_)
			fclose(pfile_);
		pfile_ = nullptr;
	}

	void CVideoFile::startRead(int nWidth, int nHeight) {
		if (pfile_) {
			bRunning_ = true;
			threadReadFile_ = std::thread(&CVideoFile::ThreadRead, this);
		}
		
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

		if (nFrameLen_ && bRunning_) {
			videobuffer_ = std::shared_ptr<uint8_t>(new uint8_t[nFrameLen_]);
		}
	}

	void CVideoFile::stopRead() {
		bRunning_ = false;
	}

	void CVideoFile::ThreadRead() {
		
		while (bRunning_) {
			if (pfile_) {
				int nRes = fread(videobuffer_.get(),1,nFrameLen_,pfile_);
				if (nRes <= 0)
					fseek(pfile_, 0, SEEK_SET);

				//to do.
			}
		}
	}
}//namespace videofile.