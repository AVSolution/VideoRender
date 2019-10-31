#ifndef __VIDEOCONV_H__
#define __VIDEOCONV_H__

#include <algorithm>
#include <iostream>
#include <string>

#include "..\\..\\libyuv\libyuv.h"
#ifdef _M_IX86
#ifdef _DEBUG
#pragma comment(lib,"..\\..\\libyuv\\debug_x86\\yuv.lib")
#else
#pragma comment(lib,"..\\..\\libyuv\\release_x86\\yuv.lib")
#endif
#else
#pragma comment(lib,"..\\..\\libyuv\\release_x64\\yuv.lib")
#endif

namespace util {
	
	class CVideoConv
	{
	public:
		CVideoConv();
		~CVideoConv();

		static bool I420ToRGB24(const uint8_t* lpYUVBuffer, uint8_t* lpBGR24Buffer, int  nRGBBufSize, int nWidth, int nHeight);

		static bool RGB24ToI420(const uint8_t* lpBGR24Buffer, uint8_t* lpYUVBuffer, int nYUVBufSize, int nWidth, int nHeight);

		static bool YUY2ToI420(const uint8_t* lpYUY2Buffer, uint8_t* lpYUVBuffer, int nYUVBufSize, int nWidth, int nHeight);

		static bool UYVYToI420(const uint8_t* lpUYVYBuffer, uint8_t* lpYUVBuffer, int nYUVBufSize, int nWidth, int nHeight);

		static bool CONVERT_ENDIAN_RGB24(uint8_t* const lpRGB24Buffer, int nWidth, int nHeight);

		static bool CONVERT_BIG_RGB24(uint8_t* lpRGB24Buffer, int nWidth, int nHeight);
		
		static void VerMirror_RGB24(uint8_t* pSrc, uint8_t* pDest, int nWidth, int nHeight);

	private:
		static const int m_nfv1Table[256];
		static const int m_nfv2Table[256];
		static const int m_nfu1Table[256];
		static const int m_nfu2Table[256];
	};

	class CTransVideo {
	public:
		//static bool 
	};

}//namespace util

#endif
