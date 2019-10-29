
// VideoRenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoRender.h"
#include "VideoRenderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVideoRenderDlg dialog
CVideoRenderDlg::CVideoRenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEORENDER_DIALOG, pParent),
	IVideoSubscribeObserver(nullptr, nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CVideoRenderDlg::CVideoRenderDlg(const char* pPublishStreamId /*= nullptr*/,const char* pSubscribeStreamId /*=nullptr*/,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEORENDER_DIALOG, pParent),
	IVideoSubscribeObserver(pPublishStreamId,pSubscribeStreamId)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1_1, m_st_1_1);
}

void CVideoRenderDlg::onNotifySubscribe() {
	
}

void CVideoRenderDlg::onSubscribeData(unsigned long ulTps, const char* publishStreamId, std::shared_ptr<uint8_t> buffer, int nBufferLen, int nWidth, int nHeight) {
	//m_mapVideoFile[publishStreamId].rendergdi.showvideo(m_st_1_1, buffer, nBufferLen, nWidth, nHeight);
}

BEGIN_MESSAGE_MAP(CVideoRenderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CVideoRenderDlg message handlers

BOOL CVideoRenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	videofileItem vfi;
	std::string publishstreamId = "path/yuv/capture";
	vfi.videofile = std::make_unique<CVideoFile>(publishstreamId.c_str(), "..\\Debug\\capture.yuv", evType_I420);
	//vfi.rendergdi.enableTipInfo(true);
	//vfi.rendergdi.addtext(CString(publishstreamId.c_str()), 0, 0, 200, 30);
	m_mapVideoFile.insert(make_pair(publishstreamId,vfi));
	add_subscribe(this);
	add_publish(vfi.videofile.get());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoRenderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CVideoRenderDlg::OnClose() {
	remove_subscribe(this);
	for(auto &it : m_mapVideoFile)
		remove_publish(it.second.videofile.get());

	CDialogEx::OnClose();
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoRenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

