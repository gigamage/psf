#include "stdafx.h"
#include "DCDrawObj.h"
#include "DCPickerDoc.h"
#include "DCPickerView.h"
#include "DCDrawTool.h"

CPtrList CDCDrawTool::c_tools;

CPoint CDCDrawTool::c_down;
UINT CDCDrawTool::c_nDownFlags;
CPoint CDCDrawTool::c_last;
DrawShape CDCDrawTool::c_drawShape = selection;

static CDCSelectTool selectTool;
static CDCRectTool rectTool(rect);

CDCDrawTool::CDCDrawTool(DrawShape nDrawShape)
{
	m_drawShape = nDrawShape;
	c_tools.AddTail(this);
}


CDCDrawTool::~CDCDrawTool(void)
{
}

void CDCDrawTool::OnLButtonDown(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	// deactivate any in-place active item on this view!
	COleClientItem* pActiveItem = pView->GetDocument()->GetInPlaceActiveItem(pView);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
		ASSERT(pView->GetDocument()->GetInPlaceActiveItem(pView) == NULL);
	}

	pView->SetCapture();
	c_nDownFlags = nFlags;
	c_down = point;
	c_last = point;
}

void CDCDrawTool::OnLButtonUp(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	ReleaseCapture();

	if (point == c_down)
		c_drawShape = selection;
}

void CDCDrawTool::OnMouseMove(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	c_last = point;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

CDCDrawTool* CDCDrawTool::FindTool(DrawShape drawShape)
{
	POSITION pos = c_tools.GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawTool* pTool = (CDCDrawTool*)c_tools.GetNext(pos);
		if (pTool->m_drawShape == drawShape)
			return pTool;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////
enum SelectMode
{
	none,
	netSelect,
	move,
	size
};

SelectMode selectMode = none;
int nDragHandle;

CPoint lastPoint;


CDCSelectTool::CDCSelectTool()
	: CDCDrawTool(selection)
{
}

void CDCSelectTool::OnLButtonDown(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	CPoint local = point;
	pView->ClientToDoc(local);

	CDCDrawObj* pObj;
	selectMode = none;

	// Check for resizing (only allowed on single selections)
	if (pView->m_selection.GetCount() == 1)
	{
		pObj = pView->m_selection.GetHead();
		nDragHandle = pObj->HitTest(local, pView, TRUE);
		if (nDragHandle != 0)
			selectMode = size;
	}

	// See if the click was on an object, select and start move if so
	if (selectMode == none)
	{
		pObj = pView->GetDocument()->ObjectAt(local);

		if (pObj != NULL)
		{
			selectMode = move;

			if (!pView->IsSelected(pObj))
				pView->Select(pObj, (nFlags & MK_SHIFT) != 0);

			// Ctrl+Click clones the selection...
			if ((nFlags & MK_CONTROL) != 0)
				pView->CloneSelection();
		}
	}

	// Click on background, start a net-selection
	if (selectMode == none)
	{
		if ((nFlags & MK_SHIFT) == 0)
			pView->Select(NULL);

		selectMode = netSelect;

		CClientDC dc(pView);
		CRect rect(point.x, point.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
	}

	lastPoint = local;
	CDCDrawTool::OnLButtonDown(pView, nFlags, point);

}

void CDCSelectTool::OnLButtonUp(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	if (pView->GetCapture() == pView)
	{
		if (selectMode == netSelect)
		{
			CClientDC dc(pView);
			CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
			rect.NormalizeRect();
			dc.DrawFocusRect(rect);

			pView->SelectWithinRect(rect, TRUE);
		}
		else if (selectMode != none)
		{
			pView->GetDocument()->UpdateAllViews(pView);
		}
	}

	CDCDrawTool::OnLButtonUp(pView, nFlags, point);
}

void CDCSelectTool::OnMouseMove(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	if (pView->GetCapture() != pView)
	{
		if (c_drawShape == selection && pView->m_selection.GetCount() == 1)
		{
			CDCDrawObj* pObj = pView->m_selection.GetHead();
			CPoint local = point;
			pView->ClientToDoc(local);
			int nHandle = pObj->HitTest(local, pView, TRUE);
			if (nHandle != 0)
			{
				SetCursor(pObj->GetHandleCursor(nHandle));
				return; // bypass CDrawTool
			}
		}
		if (c_drawShape == selection)
			CDCDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	if (selectMode == netSelect)
	{
		CClientDC dc(pView);
		CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(c_down.x, c_down.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);

		CDCDrawTool::OnMouseMove(pView, nFlags, point);
		return;
	}

	CPoint local = point;
	pView->ClientToDoc(local);
	CPoint delta = (CPoint)(local - lastPoint);

	POSITION pos = pView->m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDCDrawObj* pObj = pView->m_selection.GetNext(pos);
		CRect position = pObj->m_position;

		if (selectMode == move)
		{
			position += delta;
			pObj->MoveTo(position, pView);
		}
		else if (nDragHandle != 0)
		{
			pObj->MoveHandleTo(nDragHandle, local, pView);
		}
	}

	lastPoint = local;

	if (selectMode == size && c_drawShape == selection)
	{
		c_last = point;
		SetCursor(pView->m_selection.GetHead()->GetHandleCursor(nDragHandle));
		return; // bypass CDrawTool
	}

	c_last = point;

	if (c_drawShape == selection)
		CDCDrawTool::OnMouseMove(pView, nFlags, point);
}



////////////////////////////////////////////////////////////////////////
//	RectTool
CDCRectTool::CDCRectTool(DrawShape drawShape)
	: CDCDrawTool(drawShape)
{

}



void CDCRectTool::OnLButtonDown(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	CDCDrawTool::OnLButtonDown(pView, nFlags, point);

	CPoint local = point;
	pView->ClientToDoc(local);

	CDCDrawRect* pObj = new CDCDrawRect(CRect(local, CSize(0, 0)));
	switch (m_drawShape)
	{
	default:
		ASSERT(FALSE); // unsuported shape!

	case rect:
		pObj->m_nShape = CDCDrawRect::rectangle;
		break;

	case roundRect:
		pObj->m_nShape = CDCDrawRect::roundRectangle;
		break;

	case ellipse:
		pObj->m_nShape = CDCDrawRect::ellipse;
		break;

	case line:
		pObj->m_nShape = CDCDrawRect::line;
		break;
	}
	pView->GetDocument()->Add(pObj);
	pView->Select(pObj);

	selectMode = size;
	nDragHandle = 1;
	lastPoint = local;
}



void CDCRectTool::OnLButtonUp(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	if (point == c_down)
	{
		// Don't create empty objects...
		CDCDrawObj *pObj = pView->m_selection.GetTail();
		pView->GetDocument()->Remove(pObj);
		pObj->Remove();
		selectTool.OnLButtonDown(pView, nFlags, point); // try a select!
	}

	selectTool.OnLButtonUp(pView, nFlags, point);
}

void CDCRectTool::OnMouseMove(CDCPickerView* pView, UINT nFlags, const CPoint& point)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	selectTool.OnMouseMove(pView, nFlags, point);
}