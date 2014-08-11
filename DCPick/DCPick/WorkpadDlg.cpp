// WorkdpadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCPick.h"
#include "WorkpadDlg.h"
#include "DCDrawObj.h"
#include "DCDrawItem.h"
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

void CWorkpadDlg::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CWorkpadDlg::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom >= rect.top);
}

void CWorkpadDlg::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CWorkpadDlg::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CWorkpadDlg::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo )
{
	pDC->SetMapMode(MM_ANISOTROPIC);
	//pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
		pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(100, 100);

	// set the origin of the coordinate system to the center of the page
	//CPoint ptOrg;
	//ptOrg.x = GetDocument()->GetSize().cx / 2;
	//ptOrg.y = GetDocument()->GetSize().cy / 2;

	//// ptOrg is in logical coordinates
	//pDC->OffsetWindowOrg(-ptOrg.x,ptOrg.y);
}


void CWorkpadDlg::CloneSelection()
{
	POSITION pos = m_pSelection->GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = m_pSelection->GetNext(pos);
		pObj->Clone(pObj->m_pDocument);
		// copies object and adds it to the document
	}
}

// rect is in device coordinates
void CWorkpadDlg::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
		Select(NULL);

	ClientToDoc(rect);

	CDCDrawObjList* pObList = GetObjects();
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj != NULL)
	{
		CDCDrawObj* pObj = pObList->GetNext(posObj);
		if (pObj->Intersects(rect))
			Select(pObj, TRUE);
	}
}


void CWorkpadDlg::Add(CDCDrawObj* pObj)
{
	m_objects.AddTail(pObj);
	pObj->m_pDocument = this;
}

void CWorkpadDlg::Remove(CDCDrawObj* pObj)
{
	// Find and remove from document
	POSITION pos = m_objects.Find(pObj);
	if (pos != NULL)
		m_objects.RemoveAt(pos);
	// set document modified flag
	//SetModifiedFlag();

	// call remove for each view so that the view can remove from m_selection
	//pos = GetFirstViewPosition();
	//while (pos != NULL)
	//	((CDCPickerView*)GetNextView(pos))->Remove(pObj);
}

BOOL CWorkpadDlg::IsSelected(const CObject* pDocItem) const
{
	CDCDrawObj* pDrawObj = (CDCDrawObj*)pDocItem;
	if (pDocItem->IsKindOf(RUNTIME_CLASS(CDCDrawItem)))
		pDrawObj = ((CDCDrawItem*)pDocItem)->m_pDrawObj;
	return m_pSelection->Find(pDrawObj) != NULL;
}

void CWorkpadDlg::UpdateDlg(DWORD dwFlag, CObject* )
{


}

void CWorkpadDlg::InvalObj(CDCDrawObj* pObj)
{
	CRect rect = pObj->m_position;
	DocToClient(rect);
	if (m_bActive && IsSelected(pObj))
	{
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;
	}
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	InvalidateRect(rect, FALSE);
}

void CWorkpadDlg::Select(CDCDrawObj* pObj, BOOL bAdd)
{
	if (!bAdd)
	{
		UpdateDlg(HINT_UPDATE_SELECTION, NULL);
		m_pSelection->RemoveAll();
	}

	if (pObj == NULL || IsSelected(pObj))
		return;

	m_pSelection->AddTail(pObj);
	InvalObj(pObj);
}

CDCDrawObj* CWorkpadDlg::ObjectAt(const CPoint& point)
{
	CRect rect(point, CSize(1, 1));
	POSITION pos = m_objects.GetTailPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = m_objects.GetPrev(pos);
		if (pObj->Intersects(rect))
			return pObj;
	}

	return NULL;
}