// DCDrawObj.cpp : implementation file
//

#include "stdafx.h"
#include "DCDrawObj.h"

#include "DCPickerView.h"
#include "DCPickerDoc.h"

// CDCDrawObj
IMPLEMENT_SERIAL(CDCDrawObj, CObject, 0)

CDCDrawObj::CDCDrawObj()
{
}

CDCDrawObj::CDCDrawObj(const CRect& position)
{
	m_position = position;
	m_pDocument = NULL;
	m_bPen = TRUE;
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	m_logpen.lopnColor = RGB(0,0,0);
	m_bBrush = FALSE;
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = RGB(192, 192, 192);
	m_logbrush.lbHatch = HS_HORIZONTAL;
}

HCURSOR CDCDrawObj::GetHandleCursor(int nHandle)
{
	ASSERT_VALID(this);

	LPCTSTR id;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

CDCDrawObj::~CDCDrawObj()
{
}

void CDCDrawObj::Remove()
{
	delete this;
}
void CDCDrawObj::Draw(CDC*)
{

}


void CDCDrawObj::DrawTracker(CDC* pDC, TrackerState state)
{
	ASSERT_VALID(this);

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			int nHandleCount = GetHandleCount();
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CPoint handle = GetHandle(nHandle);
				pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
			}
		}
		break;
	}
}


int CDCDrawObj::GetHandleCount()
{
	ASSERT_VALID(this);
	return 8;
}

// returns logical coords of center of handle
CPoint CDCDrawObj::GetHandle(int nHandle)
{
	ASSERT_VALID(this);
	int x, y, xCenter, yCenter;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = m_position.left + m_position.Width() / 2;
	yCenter = m_position.top + m_position.Height() / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = m_position.left;
		y = m_position.top;
		break;

	case 2:
		x = xCenter;
		y = m_position.top;
		break;

	case 3:
		x = m_position.right;
		y = m_position.top;
		break;

	case 4:
		x = m_position.right;
		y = yCenter;
		break;

	case 5:
		x = m_position.right;
		y = m_position.bottom;
		break;

	case 6:
		x = xCenter;
		y = m_position.bottom;
		break;

	case 7:
		x = m_position.left;
		y = m_position.bottom;
		break;

	case 8:
		x = m_position.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

void CDCDrawObj::Invalidate()
{
	ASSERT_VALID(this);
	m_pDocument->UpdateAllViews(NULL, HINT_UPDATE_DRAWOBJ, this);
}


// position is in logical
void CDCDrawObj::MoveTo(const CRect& position, CDCPickDlg* pView)
{
	ASSERT_VALID(this);

	if (position == m_position)
		return;

	if (pView == NULL)
	{
		Invalidate();
		m_position = position;
		Invalidate();
	}
	else
	{
		pView->InvalObj(this);
		m_position = position;
		pView->InvalObj(this);
	}
	m_pDocument->SetModifiedFlag();
}


// point must be in logical
void CDCDrawObj::MoveHandleTo(int nHandle, CPoint point, CDCPickDlg* pView)
{
	ASSERT_VALID(this);

	CRect position = m_position;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;
	}

	MoveTo(position, pView);
}

// return rectange of handle in logical coords
CRect CDCDrawObj::GetHandleRect(int nHandleID, CDCPickDlg* pView)
{
	ASSERT_VALID(this);
	ENSURE(pView != NULL);

	CRect rect;
	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
	pView->DocToClient(point);
	// return CRect of handle in device coords
	rect.SetRect(point.x-3, point.y-3, point.x+3, point.y+3);
	pView->ClientToDoc(rect);

	return rect;
}


int CDCDrawObj::HitTest(CPoint point, CDCPickDlg* pView, BOOL bSelected)
{
	ASSERT_VALID(this);
	ASSERT(pView != NULL);

	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc = GetHandleRect(nHandle,pView);
			if (point.x >= rc.left && point.x < rc.right &&
				point.y <= rc.top && point.y > rc.bottom)
				return nHandle;
		}
	}
	else
	{
		if (point.x >= m_position.left && point.x < m_position.right &&
			point.y <= m_position.top && point.y > m_position.bottom)
			return 1;
	}
	return 0;
}

// rect must be in logical coordinates
BOOL CDCDrawObj::Intersects(const CRect& rect)
{
	ASSERT_VALID(this);

	CRect fixed = m_position;
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
}

CDCDrawObj* CDCDrawObj::Clone(CDCPickerDoc* pDoc)
{
	ASSERT_VALID(this);

	CDCDrawObj* pClone = new CDCDrawObj(m_position);
	pClone->m_bPen = m_bPen;
	pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	pClone->m_logbrush = m_logbrush;
	ASSERT_VALID(pClone);

	if (pDoc != NULL)
		pDoc->Add(pClone);

	return pClone;
}

void CDCDrawObj::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_position;
		ar << (WORD)m_bPen;
		ar.Write(&m_logpen, sizeof(LOGPEN));
		ar << (WORD)m_bBrush;
		ar.Write(&m_logbrush, sizeof(LOGBRUSH));
	}
	else
	{
		// get the document back pointer from the archive
		m_pDocument = (CDCPickerDoc*)ar.m_pDocument;
		ASSERT_VALID(m_pDocument);
		ASSERT_KINDOF(CDCPickerDoc, m_pDocument);

		WORD wTemp;
		ar >> m_position;
		ar >> wTemp; m_bPen = (BOOL)wTemp;
		ar.Read(&m_logpen,sizeof(LOGPEN));
		ar >> wTemp; m_bBrush = (BOOL)wTemp;
		ar.Read(&m_logbrush, sizeof(LOGBRUSH));
	}
}

// CDCDrawRect member functions

IMPLEMENT_SERIAL(CDCDrawRect, CDCDrawObj, 0)

CDCDrawRect::CDCDrawRect()
{
}

CDCDrawRect::CDCDrawRect(const CRect& position)
	: CDCDrawObj(position)
{
	ASSERT_VALID(this);

	m_nShape = rectangle;
	m_roundness.x = 16;
	m_roundness.y = 16;
}

void CDCDrawRect::Draw(CDC* pDC)
{
	ASSERT_VALID(this);

	CBrush brush;
	if (!brush.CreateBrushIndirect(&m_logbrush))
		return;
	CPen pen;
	if (!pen.CreatePenIndirect(&m_logpen))
		return;

	CBrush* pOldBrush;
	CPen* pOldPen;

	if (m_bBrush)
		pOldBrush = pDC->SelectObject(&brush);
	else
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	if (m_bPen)
		pOldPen = pDC->SelectObject(&pen);
	else
		pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

	CRect rect = m_position;
	switch (m_nShape)
	{
	case rectangle:
		pDC->Rectangle(rect);
		break;

	//case roundRectangle:
	//	pDC->RoundRect(rect, m_roundness);
	//	break;

	//case ellipse:
	//	pDC->Ellipse(rect);
	//	break;

	//case line:
	//	if (rect.top > rect.bottom)
	//	{
	//		rect.top -= m_logpen.lopnWidth.y / 2;
	//		rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
	//	}
	//	else
	//	{
	//		rect.top += (m_logpen.lopnWidth.y + 1) / 2;
	//		rect.bottom -= m_logpen.lopnWidth.y / 2;
	//	}

	//	if (rect.left > rect.right)
	//	{
	//		rect.left -= m_logpen.lopnWidth.x / 2;
	//		rect.right += (m_logpen.lopnWidth.x + 1) / 2;
	//	}
	//	else
	//	{
	//		rect.left += (m_logpen.lopnWidth.x + 1) / 2;
	//		rect.right -= m_logpen.lopnWidth.x / 2;
	//	}

	//	pDC->MoveTo(rect.TopLeft());
	//	pDC->LineTo(rect.BottomRight());
	//	break;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}



// CDCDrawOleObj member functions


CDCDrawOleObj::CDCDrawOleObj()
{
	m_pClientItem = NULL;
}

CDCDrawOleObj::~CDCDrawOleObj()
{

}

void CDCDrawRect::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CDCDrawObj::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << (WORD) m_nShape;
		ar << m_roundness;
	}
	else
	{
		WORD wTemp;
		ar >> wTemp; m_nShape = (Shape)wTemp;
		ar >> m_roundness;
	}
}