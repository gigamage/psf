
// DCPickerDoc.cpp : implementation of the CDCPickerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DCPicker.h"
#endif

#include "DCDrawObj.h"

#include "DCPickerDoc.h"
#include "DCPickerView.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDCPickerDoc

IMPLEMENT_DYNCREATE(CDCPickerDoc, COleDocument)

BEGIN_MESSAGE_MAP(CDCPickerDoc, COleDocument)
END_MESSAGE_MAP()


// CDCPickerDoc construction/destruction

CDCPickerDoc::CDCPickerDoc()
{
	// TODO: add one-time construction code here
	EnableCompoundFile();
	m_paperColor = RGB(255, 255, 255);
	ComputePageSize();
}

CDCPickerDoc::~CDCPickerDoc()
{
}

BOOL CDCPickerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDCPickerDoc serialization

void CDCPickerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDCPickerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDCPickerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDCPickerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDCPickerDoc diagnostics

#ifdef _DEBUG
void CDCPickerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCPickerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDCPickerDoc commands
void CDCPickerDoc::ComputePageSize()
{
	CSize new_size(850, 1100);  // 8.5" x 11" default

	CPrintDialog dlg(FALSE);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		// GetPrinterDC returns a HDC so attach it
		CDC dc;
		HDC hDC= dlg.CreatePrinterDC();
		ASSERT(hDC != NULL);
		dc.Attach(hDC);

		// Get the size of the page in loenglish
		new_size.cx = MulDiv(dc.GetDeviceCaps(HORZSIZE), 1000, 254);
		new_size.cy = MulDiv(dc.GetDeviceCaps(VERTSIZE), 1000, 254);
	}

	// if size changed then iterate over views and reset
	if (new_size != m_size)
	{
		m_size = new_size;
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
			((CDCPickerView*)GetNextView(pos))->SetPageSize(m_size);
	}

}


CDCDrawObj* CDCPickerDoc::ObjectAt(const CPoint& point)
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

void CDCPickerDoc::Draw(CDC* pDC, CDCPickerView* pView)
{
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = m_objects.GetNext(pos);
		pObj->Draw(pDC);
		if (pView->m_bActive && !pDC->IsPrinting() && pView->IsSelected(pObj))
			pObj->DrawTracker(pDC, CDCDrawObj::selected);
	}
}

void CDCPickerDoc::Add(CDCDrawObj* pObj)
{
	m_objects.AddTail(pObj);
	pObj->m_pDocument = this;
}

void CDCPickerDoc::Remove(CDCDrawObj* pObj)
{
	// Find and remove from document
	POSITION pos = m_objects.Find(pObj);
	if (pos != NULL)
		m_objects.RemoveAt(pos);
	// set document modified flag
	SetModifiedFlag();

	// call remove for each view so that the view can remove from m_selection
	pos = GetFirstViewPosition();
	while (pos != NULL)
		((CDCPickerView*)GetNextView(pos))->Remove(pObj);
}