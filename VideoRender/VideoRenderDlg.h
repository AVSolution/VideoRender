
// VideoRenderDlg.h : header file
//

#pragma once

#include "videoroute.h"
using namespace videoroute;
#include "RenderGDI.h"
using namespace videorender;
#include "videoFile.h"
using namespace  videofile;

#include <map>
#include <mutex>

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

	virtual void onNotifySubscribe() override;
	virtual void onSubscribeData(unsigned long ulTps, const char* publishStreamId, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) override;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

// 	CRenderGDI m_rendergdi;
// 	std::string m_strPublish;
// 	std::string m_strPublish1;
// 	std::unique_ptr<CVideoFile> m_upVideoFile;
// 	std::unique_ptr<CVideoFile> m_upVideoFile1;

	struct videofileItem {
		std::unique_ptr<CVideoFile> videofile;
		CRenderGDI	rendergdi;
		videofileItem(std::unique_ptr<CVideoFile> videofile_, CRenderGDI rendergdi_) {
		}
	};

	std::mutex	m_mutex;
	std::map<std::string, videofileItem> m_mapVideoFile;
	
public:
	CStatic m_st_1_1;
};
