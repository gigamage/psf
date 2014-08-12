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

class CWorkpadDlg;

class CDCDrawTool
{
public:
	CDCDrawTool(DrawShape nDrawShape);
	~CDCDrawTool(void);

//	overridables
	virtual void OnLButtonDown(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnRButtonUp(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

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
	virtual void OnLButtonDown(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnRButtonUp(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
};

class CDCRectTool : public CDCDrawTool
{
public:
	CDCRectTool(DrawShape drawShape);

	// Implementation
	virtual void OnLButtonDown(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);
	virtual void OnRButtonUp(CWorkpadDlg* pView, UINT nFlags, const CPoint& point);

};

