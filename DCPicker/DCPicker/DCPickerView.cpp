
// DCPickerView.cpp : implementation of the CDCPickerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DCPicker.h"
#endif

#include "DCDrawItem.h"
#include "DCDrawObj.h"
#include "DCDrawTool.h"
#include "DCPickerDoc.h"
#include "DCPickerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDCPickerView

IMPLEMENT_DYNCREATE(CDCPickerView, CView)

BEGIN_MESSAGE_MAP(CDCPickerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDCPickerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_SELECT, &CDCPickerView::OnDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SELECT, &CDCPickerView::OnUpdateDrawSelect)
	ON_COMMAND(ID_DRAW_RECT, &CDCPickerView::OnDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, &CDCPickerView::OnUpdateDrawRect)
END_MESSAGE_MAP()

// CDCPickerView construction/destruction

CDCPickerView::CDCPickerView()
{
	// TODO: add construction code here
	m_bActive = FALSE;
	m_gridColor = RGB(0, 0, 128);
	m_bGrid = TRUE;

}

CDCPickerView::~CDCPickerView()
{
}

BOOL CDCPickerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDCPickerView drawing

void CDCPickerView::OnDraw(CDC* pDC)
{
	CDCPickerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = 0;

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);
	CRect rect = client;
	DocToClient(rect);

	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	// paint background
	CBrush brush;
	if (!brush.CreateSolidBrush(pDoc->GetPaperColor()))
		return;

	brush.UnrealizeObject();
	pDrawDC->FillRect(client, &brush);

	if (!pDC->IsPrinting())
	{

		if (m_bGrid)
		{
			DrawGrid(pDrawDC);
		}

	}

		CRect r;
		GetClientRect(&r);
		DrawBackgroundImage(pDrawDC);

	pDoc->Draw(pDrawDC, this);


	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}


// CDCPickerView printing


void CDCPickerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDCPickerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDCPickerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDCPickerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDCPickerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{

	ClientToScreen(&point);
	OnContextMenu(this, point);
	Invalidate();
}

void CDCPickerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDCPickerView diagnostics

#ifdef _DEBUG
void CDCPickerView::AssertValid() const
{
	CView::AssertValid();
}

void CDCPickerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDCPickerDoc* CDCPickerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDCPickerDoc)));
	return (CDCPickerDoc*)m_pDocument;
}
#endif //_DEBUG


// CDCPickerView message handlers


void CDCPickerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	CView::OnPrepareDC(pDC, pInfo);

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches

	pDC->SetMapMode(MM_ANISOTROPIC);
	//pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
		pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(100, 100);

	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	ptOrg.x = GetDocument()->GetSize().cx / 2;
	ptOrg.y = GetDocument()->GetSize().cy / 2;

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(-ptOrg.x,ptOrg.y);

}

void CDCPickerView::SetPageSize(CSize size)
{
	CClientDC dc(NULL);
	size.cx = MulDiv(size.cx, dc.GetDeviceCaps(LOGPIXELSX), 100);
	size.cy = MulDiv(size.cy, dc.GetDeviceCaps(LOGPIXELSY), 100);
	SetScrollSizes(MM_TEXT, size);
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_WINDOW, NULL);
}


void CDCPickerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	//m_kImage.Load(L"z:\\Documents\\GitHub\\psf\\DCPicker\\data\\thumb\\thumb100.jpg");
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	//LoadDefaultImage();

	// invalidate selections when active status changes
	if (m_bActive != bActivate)
	{
		if (bActivate)  // if becoming active update as if active
			m_bActive = bActivate;
		if (!m_selection.IsEmpty())
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_bActive = bActivate;
	}
}


void CDCPickerView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
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
				(CDCDrawObjList*)pHint : &m_selection;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
				InvalObj(pList->GetNext(pos));
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		if (pHint != &m_selection)
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
			CDCPickerDoc* pDoc = GetDocument();
			POSITION pos = pDoc->GetObjects()->GetHeadPosition();
			while (pos != NULL)
			{
				CDCDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
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

BOOL CDCPickerView::IsSelected(const CObject* pDocItem) const
{
	CDCDrawObj* pDrawObj = (CDCDrawObj*)pDocItem;
	if (pDocItem->IsKindOf(RUNTIME_CLASS(CDCDrawItem)))
		pDrawObj = ((CDCDrawItem*)pDocItem)->m_pDrawObj;
	return m_selection.Find(pDrawObj) != NULL;
}

void CDCPickerView::InvalObj(CDCDrawObj* pObj)
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


void CDCPickerView::Remove(CDCDrawObj* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL)
		m_selection.RemoveAt(pos);
}


void CDCPickerView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CDCPickerView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom >= rect.top);
}

void CDCPickerView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CDCPickerView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}


void CDCPickerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDCDrawTool* pTool = CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
	{
		pTool->OnLButtonDown(this, nFlags, point);
	}
	//CScrollView::OnLButtonDown(nFlags, point);
}

void CDCPickerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDCDrawTool* pTool = 
		CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonUp(this, nFlags, point);
}

void CDCPickerView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDCDrawTool* pTool = CDCDrawTool::FindTool(CDCDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnMouseMove(this, nFlags, point);
}
void CDCPickerView::Select(CDCDrawObj* pObj, BOOL bAdd)
{
	if (!bAdd)
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	if (pObj == NULL || IsSelected(pObj))
		return;

	m_selection.AddTail(pObj);
	InvalObj(pObj);
}

// rect is in device coordinates
void CDCPickerView::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
		Select(NULL);

	ClientToDoc(rect);

	CDCDrawObjList* pObList = GetDocument()->GetObjects();
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj != NULL)
	{
		CDCDrawObj* pObj = pObList->GetNext(posObj);
		if (pObj->Intersects(rect))
			Select(pObj, TRUE);
	}
}

void CDCPickerView::CloneSelection()
{
	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = m_selection.GetNext(pos);
		pObj->Clone(pObj->m_pDocument);
			// copies object and adds it to the document
	}
}




void CDCPickerView::OnDrawSelect()
{
	CDCDrawTool::c_drawShape = selection;
}


void CDCPickerView::OnUpdateDrawSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(CDCDrawTool::c_drawShape == selection);
}


void CDCPickerView::OnDrawRect()
{
	CDCDrawTool::c_drawShape = rect;
}


void CDCPickerView::OnUpdateDrawRect(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(CDCDrawTool::c_drawShape == rect);
}



void CDCPickerView::DrawGrid(CDC* pDC)
{
	CDCPickerDoc* pDoc = GetDocument();

	COLORREF oldBkColor = pDC->SetBkColor(pDoc->GetPaperColor());

	CRect rect;
	rect.left = -pDoc->GetSize().cx / 2;
	rect.top = -pDoc->GetSize().cy / 2;
	rect.right = rect.left + pDoc->GetSize().cx;
	rect.bottom = rect.top + pDoc->GetSize().cy;

	// Center lines
	CPen penDash;
	penDash.CreatePen(PS_DASH, 1, m_gridColor);
	CPen* pOldPen = pDC->SelectObject(&penDash);

	pDC->MoveTo(0, rect.top);
	pDC->LineTo(0, rect.bottom);
	pDC->MoveTo(rect.left, 0);
	pDC->LineTo(rect.right, 0);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_DOT, 1, m_gridColor);
	pDC->SelectObject(&penDot);

	for (int x = rect.left / 100 * 100; x < rect.right; x += 100)
	{
		if (x != 0)
		{
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);
		}
	}

	for (int y = rect.top / 100 * 100; y < rect.bottom; y += 100)
	{
		if (y != 0)
		{
			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);
		}
	}

	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 1, m_gridColor);
	pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldPen);
	pDC->SetBkColor(oldBkColor);
}

void CDCPickerView::DrawBackgroundImage(CDC* pDrawDC)
{
	if (!m_kImage.IsNull())
	{
		CPoint ptView = pDrawDC->GetViewportOrg();
		CPoint ptWin = pDrawDC->GetWindowOrg();
		//int iMap = pDrawDC->GetMapMode();
		//pDrawDC->SetViewportOrg(0, 0);
		//pDrawDC->SetWindowOrg(0, 0);
		//pDrawDC->SetMapMode(MM_TEXT);
		//m_kImage.UpdateBitmapInfo(ATL::CImage::DIBOrientation::DIBOR_BOTTOMUP);
		CPoint ptOrg;
		ptOrg.x = GetDocument()->GetSize().cx / 2;
		ptOrg.y = GetDocument()->GetSize().cy / 2;

		m_kImage.Draw(pDrawDC->GetSafeHdc(), -ptOrg.x, ptOrg.y);
		//m_kImage.TransparentBlt(pDrawDC->GetSafeHdc(), 0,0,r.Width(),r.Height(),RGB(255,255,255));
			
		//pDrawDC->SetMapMode(iMap);

		pDrawDC->SetViewportOrg(ptView);
		pDrawDC->SetWindowOrg(ptWin);
	}
}


void CDCPickerView::LoadDefaultImage()
{
	m_kImage.Load(L"..\\data\\thumb\\thumb100.jpg");
}