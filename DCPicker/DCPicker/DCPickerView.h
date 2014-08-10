
// DCPickerView.h : interface of the CDCPickerView class
//

#pragma once

// Hints for UpdateAllViews/OnUpdate
#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

class DCDrawObj;
class CDCPickerDoc;

class CDCPickerView : public CScrollView
{
protected: // create from serialization only
	CDCPickerView();
	DECLARE_DYNCREATE(CDCPickerView)

// Attributes
public:
	CDCPickerDoc* GetDocument() const;
	void SetPageSize(CSize size);

	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);

	BOOL m_bActive; // is the view active?

	CDCDrawObjList m_selection;
	BOOL m_bGrid;
	COLORREF m_gridColor;

protected:
	CImage	m_kImage;

// Operations
public:
	void InvalObj(CDCDrawObj* pObj);
	void Select(CDCDrawObj* pObj, BOOL bAdd = FALSE);
	void SelectWithinRect(CRect rect, BOOL bAdd = FALSE);

	void CloneSelection();

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual BOOL IsSelected(const CObject* pDocItem) const;

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void DrawGrid(CDC* pDC);
	void DrawBackgroundImage(CDC* pDC);
public:
	void LoadDefaultImage();
// Implementation
public:
	virtual ~CDCPickerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	void Remove(CDCDrawObj* pObj);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnDrawSelect();
	afx_msg void OnUpdateDrawSelect(CCmdUI *pCmdUI);
	afx_msg void OnDrawRect();
	afx_msg void OnUpdateDrawRect(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in DCPickerView.cpp
inline CDCPickerDoc* CDCPickerView::GetDocument() const
   { return reinterpret_cast<CDCPickerDoc*>(m_pDocument); }
#endif

