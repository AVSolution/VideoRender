#include <Windows.h>
#include "RenderGDI.h"
#include <timeapi.h>
#pragma comment(lib,"Winmm.lib")

namespace videorender {

	CRenderGDI::CRenderGDI(eRenderType renderType) :
		m_nWidth(0),
		m_nHeight(0),
		m_wnd(nullptr),
		m_bThreadRuning(false),
		m_nFrameCount(0),
		m_bEnableTipInfo(false),
		m_pImageBk(nullptr),
		m_renderType(renderType)
	{
	}

	CRenderGDI::~CRenderGDI()
	{
		m_bThreadRuning = false;
	}

	CRenderGDI::CRenderGDI(void* pHandle, int nWidth, int nHeight) :
		m_nWidth(nWidth),
		m_nHeight(nHeight),
		m_wnd(pHandle)
	{
	}

	CRenderGDI::CRenderGDI(const CRenderGDI& rendergdi) {
		m_nWidth = rendergdi.m_nWidth;
		m_nHeight = rendergdi.m_nHeight;
		m_wnd = rendergdi.m_wnd;
	}

	bool CRenderGDI::showvideo(void* pHandle, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight)
	{
		if (pHandle && ((nBufferLen == nWidth * nHeight * 3 && eRenderType_RGB24 == m_renderType) ||
			(nBufferLen == nWidth * nHeight * 4 && eRenderType_BGRA == m_renderType)) ) {
			m_wnd = pHandle;
			if (m_nWidth != nWidth || m_nHeight != nHeight) {
				m_nWidth = nWidth;
				m_nHeight = nHeight;
				//m_videoBuffer = buffer;
				m_videoBuffer.reset(new uint8_t[nBufferLen]);
				if (!m_bThreadRuning) {
					m_bThreadRuning = true;
					m_threadRender = std::thread(&CRenderGDI::ThreadRender, this);
					m_threadRender.detach();
				}
			}

			memcpy(m_videoBuffer.get(), buffer.get(), nBufferLen);
			if (0 == m_nFrameCount)
				m_dLasttps = timeGetTime();
			m_nFrameCount++;
			DWORD ddNow = timeGetTime();
			if (ddNow - m_dLasttps > 2000) {
				m_nFps = m_nFrameCount / ((ddNow - m_dLasttps) * 1.0 / 1000);
				char szbuffps[256];
				sprintf_s(szbuffps, "handle %p video render fps: %d\n", pHandle, m_nFps);
				OutputDebugStringA(szbuffps);
				m_dLasttps = ddNow;
				m_nFrameCount = 0;
			}

			if(m_nFps)
				

			return true;
		}

		return false;
	}

	bool CRenderGDI::loadBKImage(const WCHAR* pStrPath)
	{
		if (nullptr == m_pImageBk)
			m_pImageBk = new ATL::CImage;

		if (m_pImageBk)
			m_pImageBk->Load(pStrPath);

		return true;
	}

	bool CRenderGDI::unloadBKImage()
	{
		if (m_pImageBk)
			delete m_pImageBk;

		m_pImageBk = nullptr;

		return 0;
	}

	bool CRenderGDI::addtext(const TCHAR* pStr, int nXpos, int nYpos, int nX, int nY)
	{
		wcscpy(m_addTxt, pStr);
		m_addTxtRt.left = nXpos;
		m_addTxtRt.top = nYpos;
		m_addTxtRt.right = nX + m_addTxtRt.left;
		m_addTxtRt.bottom = nY + m_addTxtRt.top;

		return true;
	}

	bool CRenderGDI::addlogo(unsigned char* pLogoBuffer, int nXpos, int nYpos, int nWidthSrc, int nHeightSrc)
	{
		return true;
	}

	void CRenderGDI::ThreadRender()
	{
		while (m_bThreadRuning) {
			std::this_thread::sleep_for(std::chrono::milliseconds((m_nFps > 0) ? (1000 / m_nFps) : 40));
			if (!IsWindow((HWND)m_wnd))
				continue;
			if (nullptr == m_videoBuffer.get())
				continue;

			HWND videownd = (HWND)m_wnd;
			RECT rt;
			GetClientRect(videownd, &rt);
			int nScreenWidth = rt.right - rt.left;
			int nScreenHeight = rt.bottom - rt.top;
			HDC hdc = ::GetDC(videownd);
			HDC hMemDc = CreateCompatibleDC(nullptr);
			HBITMAP hBmp = CreateCompatibleBitmap(hdc, nScreenWidth, nScreenHeight);

			SetStretchBltMode(hMemDc, HALFTONE);
			HBITMAP hOld = (HBITMAP)SelectObject(hMemDc, hBmp);
			if (m_pImageBk) {
				SelectObject(hMemDc, *m_pImageBk);
			}

			BITMAPINFO bmpHdr = { 0 };
			bmpHdr.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmpHdr.bmiHeader.biBitCount = (m_renderType == eRenderType_BGRA ? 32 : 24);
			bmpHdr.bmiHeader.biClrImportant = 0;
			bmpHdr.bmiHeader.biClrUsed = 0;
			bmpHdr.bmiHeader.biPlanes = 1;
			bmpHdr.bmiHeader.biWidth = m_nWidth;
			bmpHdr.bmiHeader.biHeight = -m_nHeight;
			bmpHdr.bmiHeader.biCompression = BI_RGB;
			bmpHdr.bmiHeader.biXPelsPerMeter = 0;
			bmpHdr.bmiHeader.biYPelsPerMeter = 0;
			bmpHdr.bmiHeader.biSizeImage = 0;

			float fXCap = 0, fYCap = 0, fWCap = 0, fHCap = 0;
			float fScreen = nScreenWidth * 1.0 / nScreenHeight;
			float fVideo = m_nWidth * 1.0 / m_nHeight;
			if (fScreen > fVideo) {
				fHCap = nScreenHeight;
				fYCap = 0;
				fWCap = fVideo * fHCap;
				fXCap = (nScreenWidth - fWCap) * 1.0 / 2;
			}
			else {
				fWCap = nScreenWidth;
				fXCap = 0;
				fHCap = fWCap / fVideo;
				fYCap = (nScreenHeight - fHCap) * 1.0 / 2;
			}

			StretchDIBits(hMemDc, fXCap, fYCap, fWCap, fHCap,
				0, 0, m_nWidth, m_nHeight,
				m_videoBuffer.get(), &bmpHdr, DIB_RGB_COLORS, SRCCOPY);

			if (m_bEnableTipInfo) {
				TCHAR szTxt[256];
				swprintf_s(szTxt, L"[%dX%d,Fps:%d]", m_nWidth, m_nHeight, m_nFps);
				RECT rttxt = { fXCap, fHCap - 30, fWCap + fXCap, fHCap };
				SetBkMode(hMemDc, TRANSPARENT);
				SetTextColor(hMemDc, RGB(255, 0, 0));
				DrawText(hMemDc, szTxt, wcslen(szTxt), &rttxt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			if (wcslen(m_addTxt)) {
				SetBkMode(hMemDc, OPAQUE);
				SetTextColor(hMemDc, RGB(0, 0, 0));
				SetBkColor(hMemDc, RGB(0, 255, 0));
				DrawText(hMemDc, m_addTxt, wcslen(m_addTxt), &m_addTxtRt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}

			BitBlt(hdc, 0, 0, nScreenWidth, nScreenHeight, hMemDc, 0, 0, SRCCOPY);

			SelectObject(hMemDc, hOld);
			DeleteObject(hBmp);
			DeleteDC(hMemDc);
			::ReleaseDC(videownd, hdc);
		}
	}

}//namespace videorender