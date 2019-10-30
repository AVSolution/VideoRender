
// VideoRenderDlg.h : header file
//

#pragma once

#include "videoroute.h"
using namespace videoroute;
#include "RenderGDI.h"
using namespace videorender;
#include "videoFile.h"
using namespace videofile;

#include <map>
#include <mutex>

class c1 {
public:
	c1() { ; }
	c1(int nNum) :m_nNum(nNum) { ; }
	//c1(const c1& c1_) {
	//	m_nNum = c1_.getNum();
	//}
	~c1() { ; }

	int getNum() const { return m_nNum; } 

private:
	int m_nNum;
};

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

	virtual void onSubscribeStatus(const char* publishSteamID, eVideoRouteType evrType) override;
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

	struct videofileItem {
		std::shared_ptr<CVideoFile> videofile;
		std::shared_ptr<CRenderGDI>	rendergdi;
		HWND wnd;
	};

	std::mutex	m_mutex;
	std::map<std::string, videofileItem> m_mapVideoFile;
	
public:
	CStatic m_st_1_1;
	afx_msg void OnBnClickedButtonOk();
	CStatic m_s_1_2;
};
