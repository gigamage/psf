// WorkdpadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCPick.h"
#include "WorkpadDlg.h"
#include "afxdialogex.h"


// CWorkdpadDlg dialog

IMPLEMENT_DYNAMIC(CWorkpadDlg, CDialogEx)

CWorkpadDlg::CWorkpadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkpadDlg::IDD, pParent)
{

}

CWorkpadDlg::~CWorkpadDlg()
{
}

void CWorkpadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWorkpadDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWorkdpadDlg message handlers


void CWorkpadDlg::OnPaint()
{
	CDC dcBuffer;
	CBitmap bitmap;
	CBitmap* pOldBitmap = 0;
	CPaintDC dc(this); // device context for painting
	CDC* pDrawDC = &dc;

	CRect client;
	dc.GetClipBox(client);

	CRect rect = client;

	//	DocToClient

	if (dcBuffer.CreateCompatibleDC(&dc))
	{
		if (bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
		{
			//	prepare dc
			pDrawDC = &dcBuffer;
			pOldBitmap = dcBuffer.SelectObject(&bitmap);
			dcBuffer.IntersectClipRect(client);
		}
	}

	//	clear
	CBrush brush;
	if (!brush.CreateSolidBrush(RGB(128, 0, 0)))
	{
		return;
	}

	pDrawDC->FillRect(client, &brush);

	m_kImage.Draw(pDrawDC->GetSafeHdc(), 0, 0);

	if (pDrawDC != (CDC*)&dc)
	{
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);

		dcBuffer.SetViewportOrg(0, 0);
		dcBuffer.SetWindowOrg(0,0);
		dcBuffer.SetMapMode(MM_TEXT);
		dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dcBuffer, 0, 0, SRCCOPY);
		dcBuffer.SelectObject(pOldBitmap);
	}
}

void CWorkpadDlg::SetImageFile(const CString& strImagePath)
{
	m_kImage.Load((LPCTSTR)strImagePath);
}


BOOL CWorkpadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//m_ToolBar.Create(this);
	//m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() 
	//	| CBRS_FLOATING);
	//m_ToolBar.LoadToolBar(IDR_TOOLBAR1);

	//CRect rcClientStart;
	//CRect rcClientNow;
	//GetClientRect(rcClientStart);
	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST,
	//	AFX_IDW_CONTROLBAR_LAST,
	//	0, reposQuery, rcClientNow);

	//CRect rcWindow;
	//GetWindowRect(rcWindow);
	//rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	//rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	//MoveWindow(rcWindow, FALSE);
	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	
	return TRUE;
}
