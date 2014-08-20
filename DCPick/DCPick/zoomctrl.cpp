//	zoomctrl.cpp
//---------------------------------------------------------------------
/*
	Child control with zoom and scroll

	Jim Dill [jim@jimdill.net]
	v 1.0, Mar 25 2008
*/
//---------------------------------------------------------------------
#include "stdafx.h"
#include "resource.h"
#include "WorkpadDlg.h"
#include "zoomctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
CZoomCtrl::CZoomCtrl()
{
    m_scrollHelper.AttachWnd(this);
	m_zoomFactor = 1.0;
	m_rVirt = CRect(0, 0, 5000, 5000);
}
//---------------------------------------------------------------------
CZoomCtrl::~CZoomCtrl()
{
}
//---------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CZoomCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEACTIVATE()
    ON_WM_MOUSEWHEEL()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//---------------------------------------------------------------------
void CZoomCtrl::Draw(CDC *pDC)
{
	// fill background
	//COLORREF backgrColor = RGB(255,255,192);
	CRect rClient;
	GetClientRect(rClient);
	
	SetScreenSize(rClient);
	//pDC->FillRect(&rClient, &CBrush(backgrColor));

	// define virtual drawing space -- change this to suit
	// if drawing must not distort, then create a rect same shape as rClient
	//CRect rVirt(0, 0, 5000, 5000);
	PrepDC(pDC);//, rVirt, rClient);

	// do your drawing here, in virtual coords

	// define virtual drawing space -- change this to suit
	// if drawing must not distort, then create a rect same shape as rClient
	PrepDC(pDC);//, rVirt, rClient);

	GetWorkpad()->GetImage().Draw(pDC->GetSafeHdc(), rClient.left, rClient.top);
	GetWorkpad()->DrawObjects(pDC);
}

void CZoomCtrl::SetScreenSize(const CRect& rect)
{
	m_rScreen = rect;
}
//---------------------------------------------------------------------
void CZoomCtrl::OnPaint()
{
	CPaintDC dc(this);
	Draw(&dc);
	//__super::OnPaint();
}
//---------------------------------------------------------------------
//void CZoomCtrl::PrepDC(CDC *pDC, const CRect& rVirt, const CRect& rScreen)
//{
//	// maps virtual into client
//	m_rVirt = rVirt;
//	m_rScreen = rScreen;
//
//	pDC->IntersectClipRect(&rScreen);
//
//	//pDC->SetMapMode(MM_ANISOTROPIC);
//	pDC->SetMapMode(MM_TEXT);
//	pDC->SetWindowExt(rVirt.Width(), rVirt.Height());
//
//	LONG wid = (LONG)(m_zoomFactor * (double)rScreen.Width());
//	LONG hgt = (LONG)(m_zoomFactor * (double)rScreen.Height());
//	pDC->SetViewportExt(wid, hgt);
//
//	CSize scrollPos = m_scrollHelper.GetScrollPos();
//	pDC->SetViewportOrg(-scrollPos.cx, -scrollPos.cy);
//}

void CZoomCtrl::PrepDC(CDC* pDC)
{
	// maps virtual into client
	//m_rVirt = rVirt;
	//m_rScreen = rScreen;

	pDC->IntersectClipRect(&m_rScreen);

	//pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetWindowExt(m_rVirt.Width(), m_rVirt.Height());

	LONG wid = (LONG)(m_zoomFactor * (double)m_rScreen.Width());
	LONG hgt = (LONG)(m_zoomFactor * (double)m_rScreen.Height());
	pDC->SetViewportExt(wid, hgt);

	CSize scrollPos = m_scrollHelper.GetScrollPos();
	pDC->SetViewportOrg(-scrollPos.cx, -scrollPos.cy);
}

//---------------------------------------------------------------------
void CZoomCtrl::AdjustScrollbars()
{
	int xMax = (int)((double)m_rScreen.Width() * m_zoomFactor);
	int yMax = (int)((double)m_rScreen.Height() * m_zoomFactor);
	m_scrollHelper.SetPageSize(m_rScreen.Width(), m_rScreen.Height());
	m_scrollHelper.SetDisplaySize(xMax, yMax);
}
//---------------------------------------------------------------------
void CZoomCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scrollHelper.OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
}
//---------------------------------------------------------------------
void CZoomCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scrollHelper.OnVScroll(nSBCode, nPos, pScrollBar);
	if (CWnd* pWnd = GetParent())
		pWnd->Invalidate(FALSE);
	else
		Invalidate();
}
//---------------------------------------------------------------------
BOOL CZoomCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    return m_scrollHelper.OnMouseWheel(nFlags, zDelta, pt);
}
//---------------------------------------------------------------------
LRESULT CZoomCtrl::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam) 
{
	// necessary for getting h/vscroll msgs
    if (/*msg == WM_NCHITTEST || */msg == WM_NCLBUTTONDOWN || msg == WM_NCLBUTTONDBLCLK) 
        return ::DefWindowProc(m_hWnd, msg, wParam, lParam); 
	if (msg == WM_NCHITTEST)
	{
		//CWnd::WindowProc(msg, wParam, lParam); 
		LRESULT lr = ::DefWindowProc(m_hWnd, msg, wParam, lParam); 
		if (lr == HTCLIENT)
			return CWnd::WindowProc(msg, wParam, lParam); 
		return lr;
	}
    return CWnd::WindowProc(msg, wParam, lParam); 
} 
//---------------------------------------------------------------------
int CZoomCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
     // necessary for getting mousewheel messages
	int status = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
    SetFocus();
	return status;
}
//---------------------------------------------------------------------
BOOL CZoomCtrl::OnEraseBkgnd(CDC* pDC)
{
	// prevent erase to reduce flashing
	// for smoothest resize, also set clipchildren style in parent dialog
    return TRUE;
}

CWorkpadDlg* CZoomCtrl::GetWorkpad()
{
	return(CWorkpadDlg*) GetParent();
}

//---------------------------------------------------------------------