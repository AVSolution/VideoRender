#ifndef __RENDERGDI_H__
#define __RENDERGDI_H__

#include <memory>
#include <thread>
#include <atlimage.h>

namespace videorender {

	enum eRenderType {
		eRenderType_RGB24,
		eRenderType_BGRA,
	};

	class CRenderGDI {
	public:
		CRenderGDI(eRenderType renderType = eRenderType_RGB24);
		CRenderGDI(void* pHandle, int nWidth, int nHeight);
		CRenderGDI(const CRenderGDI& rendergdi);
		~CRenderGDI();

		void enableTipInfo(bool bEnable) { m_bEnableTipInfo = bEnable; }
		bool addtext(const TCHAR* pStr, int nXpos, int nYpos, int nX, int nY);
		bool addlogo(unsigned char* pLogoBuffer, int nXpos, int nYpos, int nWidthSrc, int nHeightSrc);
		bool showvideo(void* pHandle, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight);

		bool loadBKImage(const TCHAR* pStrPath);
		bool unloadBKImage();

	protected:
		void ThreadRender();

	private:

		int		m_nWidth;
		int		m_nHeight;
		std::shared_ptr<uint8_t> m_videoBuffer;
		std::thread		m_threadRender;
		bool				m_bThreadRuning;
		void*				m_wnd;
		int					m_nFps;
		int					m_nFrameCount;
		DWORD		m_dLasttps;

		bool				m_bEnableTipInfo;
		TCHAR			m_addTxt[256];
		RECT			m_addTxtRt;
		std::unique_ptr<BYTE>	m_logoBuffer;
		RECT			m_addLogoRt;
		eRenderType		m_renderType;

		ATL::CImage* m_pImageBk;
	};

}//namespace videorender

#endif