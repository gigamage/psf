
// DCPickDlg.h : header file
//

#pragma once


// CDCPickDlg dialog
class CDCPickDlg : public CDialogEx
{
// Construction
public:
	CDCPickDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenfile();
};
