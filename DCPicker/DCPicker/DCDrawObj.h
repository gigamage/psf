#pragma once

// CDCDrawObj command target
class CDCPickerView;
class CDCPickerDoc;
class CDCDrawItem;

class CDCDrawObj : public CObject
{
public:
	enum TrackerState { normal, selected, active };

protected:
	DECLARE_SERIAL(CDCDrawObj);
	CDCDrawObj();
public:
	CDCDrawObj(const CRect& position);
	virtual ~CDCDrawObj();

	// Attributes
	CRect m_position;
	CDCPickerDoc* m_pDocument;
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);

	CRect GetHandleRect(int nHandleID, CDCPickerView* pView);

	// Operations
	virtual void Draw(CDC* pDC);
	virtual void DrawTracker(CDC* pDC, TrackerState state);

	virtual int HitTest(CPoint point, CDCPickerView* pView, BOOL bSelected);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDCDrawObj* Clone(CDCPickerDoc* pDoc = NULL);
	virtual void MoveTo(const CRect& positon, CDCPickerView* pView = NULL);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDCPickerView* pView = NULL);

	void Invalidate();
	virtual void Remove();

	// Implementation
public:
	virtual void Serialize(CArchive& ar);


	// implementation data
protected:
	BOOL m_bPen;
	LOGPEN m_logpen;
	BOOL m_bBrush;
	LOGBRUSH m_logbrush;


};

// special 'list' class for this application (requires afxtempl.h)
typedef CTypedPtrList<CObList, CDCDrawObj*> CDCDrawObjList;

///////////////////////////////////////////////////////////////////
//	Draw Objects

class CDCDrawRect : public CDCDrawObj
{
public:
	enum TrackerState {
		normal,
		selected,
		active
	};
	enum Shape { rectangle, roundRectangle, ellipse, line };

protected:
	DECLARE_SERIAL(CDCDrawRect);
	CDCDrawRect();

public:
	CDCDrawRect(const CRect& position);

//	Implementation
public:
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

protected:
	Shape m_nShape;
	CPoint m_roundness; // for roundRect corners
	
	friend class CDCRectTool;

};


class CDCDrawOleObj : public CDCDrawObj
{
public:
	CDCDrawOleObj();
	virtual ~CDCDrawOleObj();

	CDCDrawItem* m_pClientItem;

};