#pragma once


// CWorkdpadDlg dialog

class CWorkdpadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkdpadDlg)

public:
	CWorkdpadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorkdpadDlg();

// Dialog Data
	enum { IDD = IDD_WORKDPADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
