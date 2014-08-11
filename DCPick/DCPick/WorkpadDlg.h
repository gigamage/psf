#pragma once


// CWorkdpadDlg dialog

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
protected:
	CImage m_kImage;
	CToolBar m_ToolBar;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();


public:
	virtual BOOL OnInitDialog();
};
