// WorkdpadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCPick.h"
#include "WorkpadDlg.h"
#include "DCDrawObj.h"
#include "DCDrawItem.h"
#include "DCDrawTool.h"
#include "afxdialogex.h"


// CWorkdpadDlg dialog

IMPLEMENT_DYNAMIC(CWorkpadDlg, CDialogEx)

CWorkpadDlg::CWorkpadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkpadDlg::IDD, pParent)
{
	m_pSelection = new CDCDrawObjList;
	m_hAccelTable
		= LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CWorkpadDlg::~CWorkpadDlg()
{
	delete m_pSelection;
}

void CWorkpadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PAGE, m_StaticPage);
}


BEGIN_MESSAGE_MAP(CWorkpadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_SELECT, &CWorkpadDlg::OnDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SELECT, &CWorkpadDlg::OnUpdateDrawSelect)
	ON_COMMAND(ID_DRAW_RECT, &CWorkpadDlg::OnDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, &CWorkpadDlg::OnUpdateDrawRect)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_CLEAR, &CWorkpadDlg::OnDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CWorkpadDlg::OnUpdateDelete)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_SELECTION, &CWorkpadDlg::OnEditSelection)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CWorkdpadDlg message handlers


void CWorkpadDlg::OnPaint()
{
	//CDC dcBuffer;
	//CBitmap bitmap;
	//CBitmap* pOldBitmap = 0;
	//CPaintDC dc(&m_StaticPage);

	//CRect rcClient;
	//m_StaticPage.GetClientRect(rcClient);
	//CRect rVirt(0, 0, 5000, 5000);
	//m_StaticPage.PrepDC(&dc, rVirt, rcClient);
	//CDC* pDrawDC = &dc;

	//CRect client, clientStatic;

	//m_StaticPage.GetClientRect(client);
	//clientStatic = client;
	//DocToClient(clientStatic);

	//CRect rect = clientStatic;

	//if (dcBuffer.CreateCompatibleDC(&dc))
	//{
	//	if (bitmap.CreateCompatibleBitmap(&dc, client.Width(), client.Height()))
	//	{
	//		//	prepare dc
	//		pDrawDC = &dcBuffer;
	//		pOldBitmap = dcBuffer.SelectObject(&bitmap);
	//		dcBuffer.IntersectClipRect(client);
	//	}
	//}

	////	clear
	//CBrush brush;
	//if (!brush.CreateSolidBrush(RGB(128, 0, 0)))
	//{
	//	return;
	//}
	//brush.UnrealizeObject();

	//pDrawDC->FillRect(clientStatic, &brush);

	//m_kImage.Draw(pDrawDC->GetSafeHdc(), client.left, client.top);

	//DrawObjects(pDrawDC);

	//if (pDrawDC != (CDC*)&dc)
	//{
	//	dc.SetViewportOrg(0, 0);
	//	dc.SetWindowOrg(0,0);
	//	dc.SetMapMode(MM_TEXT);

	//	dcBuffer.SetViewportOrg(0, 0);
	//	dcBuffer.SetWindowOrg(0,0);
	//	dcBuffer.SetMapMode(MM_TEXT);
	//	dc.BitBlt(client.left, client.top, client.Width(), client.Height(),
	//		&dcBuffer, 0, 0, SRCCOPY);
	//	dcBuffer.SelectObject(pOldBitmap);
	//}
	__super::OnPaint();
}

void CWorkpadDlg::SetImageFile(const CString& strImagePath)
{
	m_kImage.Load((LPCTSTR)strImagePath);
}


BOOL CWorkpadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_ToolBar.Create(this);
	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() 
		| CBRS_FLOATING);
	m_ToolBar.LoadToolBar(IDR_TOOLBAR1);

	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,
		AFX_IDW_CONTROLBAR_LAST,
		0, reposQuery, rcClientNow);

	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	
	m_StaticPage.SetZoomFactor(1.2f);
	m_StaticPage.GetWindowRect(rcWindow);
	m_StaticPage.SetScreenSize(rcWindow);
	//m_StaticPage.set
	m_StaticPage.AdjustScrollbars();
	m_StaticPage.Invalidate();
	return TRUE;
}

void CWorkpadDlg::ClientToDoc(CPoint& point)
{
	CClientDC dc(&m_StaticPage);


	m_StaticPage.PrepDC(&dc);//, rVirt, rClient);


	//OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);

	CRect clientStatic;
	m_StaticPage.GetWindowRect(clientStatic);
	ScreenToClient(clientStatic);
	point -= clientStatic.TopLeft();


}

void CWorkpadDlg::ClientToDoc(CRect& rect)
{
	CClientDC dc(&m_StaticPage);
	m_StaticPage.PrepDC(&dc);//, rVirt, rClient);

	//OnPrepareDC(&dc, NULL);
	CRect clientStatic;
	m_StaticPage.GetWindowRect(clientStatic);
	ScreenToClient(clientStatic);
	rect-= clientStatic.TopLeft();

	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom >= rect.top);


}

void CWorkpadDlg::DocToClient(CPoint& point)
{
	CClientDC dc(&m_StaticPage);

	m_StaticPage.PrepDC(&dc);//, rVirt, rClient);

	//OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
	CRect clientStatic;
	m_StaticPage.GetWindowRect(clientStatic);
	ScreenToClient(clientStatic);
	point += clientStatic.TopLeft();


}

void CWorkpadDlg::DocToClient(CRect& rect)
{
	CClientDC dc(&m_StaticPage);

	m_StaticPage.PrepDC(&dc);//, rVirt, rClient);

	//OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
	CRect clientStatic;
	m_StaticPage.GetWindowRect(clientStatic);
	ScreenToClient(clientStatic);
	rect += clientStatic.TopLeft();


}

void CWorkpadDlg::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo )
{
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
		pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(100, 100);
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

	pos = m_pSelection->Find(pObj);
	if (pos != NULL)
		m_pSelection->RemoveAt(pos);
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

void CWorkpadDlg::UpdateDlg(DWORD lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		InvalObj((CDCDrawObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			CDCDrawObjList* pList = pHint != NULL ?
				(CDCDrawObjList*)pHint : m_pSelection;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
				InvalObj(pList->GetNext(pos));
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		if (pHint != m_pSelection)
		{
			CDCDrawObjList* pList = (CDCDrawObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDCDrawObj* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);   // remove it from this view's selection
			}
		}
		break;

	case HINT_UPDATE_OLE_ITEMS:
		{
			POSITION pos = GetObjects()->GetHeadPosition();
			while (pos != NULL)
			{
				CDCDrawObj* pObj = GetObjects()->GetNext(pos);
				if (pObj->IsKindOf(RUNTIME_CLASS(CDCDrawOleObj)))
					InvalObj(pObj);
			}
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}

}

void CWorkpadDlg::InvalObj(CDCDrawObj* pObj)
{
	CRect rect = pObj->m_position;
	DocToClient(rect);
	if (m_bActive && IsSelected(pObj))
	{
		rect.left -= 4 * 2;
		rect.top -= 5 * 2;
		rect.right += 5 * 2;
		rect.bottom += 4;
	}
	rect.InflateRect(2, 2); // handles CDrawOleObj objects

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

void CWorkpadDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDCDrawTool* pTool = CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
	{
		pTool->OnLButtonDown(this, nFlags, point);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CWorkpadDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDCDrawTool* pTool = 
		CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonUp(this, nFlags, point);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CWorkpadDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CWorkpadDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDCDrawTool* pTool = CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnMouseMove(this, nFlags, point);
	//Invalidate(FALSE);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CWorkpadDlg::OnDrawSelect()
{
	CDCDrawTool::c_drawShape = selection;
}


void CWorkpadDlg::OnUpdateDrawSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(CDCDrawTool::c_drawShape == selection);
}


void CWorkpadDlg::OnDrawRect()
{
	CDCDrawTool::c_drawShape = rect;
}


void CWorkpadDlg::OnUpdateDrawRect(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(CDCDrawTool::c_drawShape == rect);
}

void CWorkpadDlg::DrawObjects(CDC* pDC)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = m_objects.GetNext(pos);
		pObj->Draw(pDC);
		if (m_bActive && !pDC->IsPrinting() && IsSelected(pObj))
			pObj->DrawTracker(pDC, CDCDrawObj::selected);
	}
}

void CWorkpadDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	//if (!m_bActive)
	//	return;

	//CDCDrawTool* pTool = 
	//	CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	//if (pTool != NULL)
	//	pTool->OnRButtonUp(this, nFlags, point);
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CWorkpadDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// make sure window is active
	
	//GetParentFrame()->ActivateFrame();

	CPoint local = point;
	ScreenToClient(&local);
	ClientToDoc(local);

	CDCDrawObj* pObj;
	pObj = ObjectAt(local);
	if(pObj != NULL)
	{
		if(!IsSelected(pObj))
			Select( pObj, FALSE );          // reselect item if appropriate
		UpdateWindow();

		CMenu menu;
		if (menu.LoadMenu(IDR_POP_MENU))
		{
			CMenu* pPopup = menu.GetSubMenu(0);
			ENSURE(pPopup != NULL);

			int nCmd = pPopup->TrackPopupMenuEx(TPM_RIGHTBUTTON | TPM_LEFTALIGN| TPM_VERPOSANIMATION | TPM_RETURNCMD | TPM_NONOTIFY,
				point.x, point.y,
				AfxGetMainWnd(), NULL); // route commands through main window
			if (nCmd)
				SendMessage(WM_COMMAND, nCmd);
		}
	}
}


void CWorkpadDlg::OnDelete()
{
	// update all the views before the selection goes away
	UpdateDlg(HINT_DELETE_SELECTION, m_pSelection);
	UpdateDlg(HINT_UPDATE_SELECTION, NULL);

	// now remove the selection from the document
	POSITION pos = m_pSelection->GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = m_pSelection->GetNext(pos);
		Remove(pObj);
		pObj->Remove();
	}
	//Cleanup Tool members such as CPolyTool::m_pDrawObj, that should be NULL at this point.
	CDCDrawTool* pTool = CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
	{
		pTool->OnCancel();
	}
	m_pSelection->RemoveAll();
}


void CWorkpadDlg::OnUpdateDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pSelection->IsEmpty());
}


BOOL CWorkpadDlg::PreTranslateMessage(MSG* pMsg)
{

	if (m_hAccelTable) {
		if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
			return(TRUE);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CWorkpadDlg::OnEditSelection()
{
	if (m_pSelection->GetCount() == 1)
	{
		CDCDrawObj* pObj = m_pSelection->GetHead();
		if (pObj)
		{
			pObj->OnEditProperties();
			InvalObj(pObj);
		}
	}
}


void CWorkpadDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

CImage& CWorkpadDlg::GetImage()
{
	return m_kImage;
}
