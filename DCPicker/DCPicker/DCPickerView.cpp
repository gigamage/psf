
// DCPickerView.cpp : implementation of the CDCPickerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DCPicker.h"
#endif

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
END_MESSAGE_MAP()

// CDCPickerView construction/destruction

CDCPickerView::CDCPickerView()
{
	// TODO: add construction code here

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

void CDCPickerView::OnDraw(CDC* /*pDC*/)
{
	CDCPickerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
		pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(100, -100);

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
