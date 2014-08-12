#pragma once
#include "afxcmn.h"


class CDCDrawObj;

// CRectEditDlg dialog

class CRectEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRectEditDlg)

public:
	CRectEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRectEditDlg();

// Dialog Data
	enum { IDD = IDD_RECTDLG };
	void SetRectObj(CDCDrawObj* pRect);

	CRect GetRectData();
protected:
	CDCDrawObj* m_pRect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CSpinButtonCtrl m_spinLeft;
	CSpinButtonCtrl m_spinRight;
	CSpinButtonCtrl m_spinTop;
	CSpinButtonCtrl m_spinBottom;
public:

	int m_iLeft;
	int m_iBottom;
	int m_iRight;
	int m_iTop;
};
