#ifndef __VIDEOFILE_H__
#define __VIDEOFILE_H__

#include <stdio.h>
#include <thread>
#include <memory>

namespace videofile {

	enum eVideoFileType {
		evType_NULL = -1,
		evType_RGB24,
		evType_I420,
		evType_PNG,
		evType_BMP,

		evType_UNKNOWN = 0xff,
	};

	class CVideoFile {
	public:
		explicit CVideoFile();
		explicit CVideoFile(const char* pStrFilePath,eVideoFileType evType);
		~CVideoFile();

		void startRead(int nWidth,int nHeight);
		void stopRead();

	protected:
		void ThreadRead();

	private:
		FILE* pfile_;
		eVideoFileType evType_;

		bool		bRunning_;
		std::thread threadReadFile_;
		std::shared_ptr<uint8_t> videobuffer_;
		int	nFrameLen_;
	};
	
}//namespace videofile

#endif
