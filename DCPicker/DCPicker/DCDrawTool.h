#pragma once

enum DrawShape
{
	selection,
	line,
	rect,
	roundRect,
	ellipse,
	poly
};

class CDCPickerView;

class CDCDrawTool
{
public:
	CDCDrawTool(DrawShape nDrawShape);
	~CDCDrawTool(void);

//	overridables
	virtual void OnLButtonDown(CDCPickerView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CDCPickerView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CDCPickerView* pView, UINT nFlags, const CPoint& point);

//	Attributes
	DrawShape	m_drawShape;

	static CDCDrawTool* FindTool(DrawShape drawShape);
	
	static CPtrList c_tools;
	static CPoint c_down;
	static UINT c_nDownFlags;
	static CPoint c_last;
	static DrawShape c_drawShape;

};

class CDCSelectTool : public CDCDrawTool
{
public:
	CDCSelectTool();

	//	Implementation
	virtual void OnLButtonDown(CDCPickerView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CDCPickerView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CDCPickerView* pView, UINT nFlags, const CPoint& point);

};

class CDCRectTool : public CDCDrawTool
{
public:
	CDCRectTool(DrawShape drawShape);

	// Implementation
	virtual void OnLButtonDown(CDCPickerView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CDCPickerView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CDCPickerView* pView, UINT nFlags, const CPoint& point);

};

