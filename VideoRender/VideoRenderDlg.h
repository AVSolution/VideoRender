
// VideoRenderDlg.h : header file
//

#pragma once

#include "videoroute.h"
using namespace videoroute;
#include "RenderGDI.h"
using namespace videorender;
#include "videoFile.h"
using namespace  videofile;

// CVideoRenderDlg dialog
class CVideoRenderDlg : public CDialogEx, public IVideoSubscribeObserver
{
// Construction
public:
	CVideoRenderDlg(CWnd* pParent = nullptr);
	CVideoRenderDlg(const char* pPublishStreamId = nullptr,const char* pSubscribeStreamId =  nullptr,CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEORENDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void onSubscribeData(unsigned long ulTps, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) override;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CRenderGDI m_rendergdi;
	std::unique_ptr<CVideoFile> m_upVideoFile;
	
public:
	CStatic m_st_1_1;
};
