#pragma once
#include "afxwin.h"

//#include "zoomctrl.h"
#include "DCPickZoomCtrl.h"

class CDCDrawObj;
typedef CTypedPtrList<CObList, CDCDrawObj*> CDCDrawObjList;

// CWorkdpadDlg dialog
// Hints for UpdateAllViews/OnUpdate
#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

class CWorkpadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkpadDlg)

public:
	CWorkpadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorkpadDlg();

// Dialog Data
	enum { IDD = IDD_WORKPADDLG };

public:
	void SetImageFile(const CString& strImagePath);
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	void InvalObj(CDCDrawObj* pObj);

	void Select(CDCDrawObj* pObj, BOOL bAdd = FALSE);

	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	CDCDrawObj* ObjectAt(const CPoint& point);
	virtual BOOL IsSelected(const CObject* pDocItem) const;
	void CloneSelection();
	void SelectWithinRect(CRect rect, BOOL bAdd = FALSE);

	void Add(CDCDrawObj* pObj);
	void Remove(CDCDrawObj* pObj);
	void DrawObjects(CDC* pDC);

	CDCDrawObjList* GetObjects() { return &m_objects; }
	void UpdateDlg(DWORD dwFlag, CObject* );


	BOOL m_bActive; // is the view active?
	CImage& GetImage();
protected:
	CImage m_kImage;
	CToolBar m_ToolBar;
	HACCEL  m_hAccelTable;

public:
	CDCDrawObjList* m_pSelection;
	CDCDrawObjList m_objects;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();


public:
	virtual BOOL OnInitDialog();
	//CStatic m_StaticPage;
	CDCPickZoomCtrl m_StaticPage;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawSelect();
	afx_msg void OnUpdateDrawSelect(CCmdUI *pCmdUI);
	afx_msg void OnDrawRect();
	afx_msg void OnUpdateDrawRect(CCmdUI *pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditSelection();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
