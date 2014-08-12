// RectEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCPick.h"
#include "RectEditDlg.h"
#include "DCDrawObj.h"
#include "afxdialogex.h"


// CRectEditDlg dialog

IMPLEMENT_DYNAMIC(CRectEditDlg, CDialogEx)

CRectEditDlg::CRectEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRectEditDlg::IDD, pParent)
	, m_pRect(NULL)

	, m_iLeft(0)
	, m_iBottom(0)
	, m_iRight(0)
	, m_iTop(0)
{

}

CRectEditDlg::~CRectEditDlg()
{
}

void CRectEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, m_spinLeft);
	DDX_Control(pDX, IDC_SPIN2, m_spinRight);
	DDX_Control(pDX, IDC_SPIN3, m_spinTop);
	DDX_Control(pDX, IDC_SPIN4, m_spinBottom);
	DDX_Text(pDX, IDC_EDIT_RECT_LEFT, m_iLeft);
	DDX_Text(pDX, IDC_EDIT_RECT_BOTTOM, m_iBottom);
	DDX_Text(pDX, IDC_EDIT_RECT_RIGHT, m_iRight);
	DDX_Text(pDX, IDC_EDIT_RECT_TOP, m_iTop);
}


BEGIN_MESSAGE_MAP(CRectEditDlg, CDialogEx)
END_MESSAGE_MAP()

void CRectEditDlg::SetRectObj(CDCDrawObj* pRect)
{
	m_pRect = pRect;
}


// CRectEditDlg message handlers


BOOL CRectEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spinLeft.SetRange(0, 10000);
	m_spinRight.SetRange(0, 10000);
	m_spinTop.SetRange(0, 10000);
	m_spinBottom.SetRange(0, 10000);

	if (m_pRect)
	{
		CRect rect = m_pRect->m_position;

		m_spinLeft.SetPos(rect.left);
		m_spinRight.SetPos(rect.right);
		m_spinTop.SetPos(rect.top);
		m_spinBottom.SetPos(rect.bottom);

	}
	return TRUE;
}

CRect CRectEditDlg::GetRectData()
{
	CRect rect;
	rect.left = m_iLeft;
	rect.right = m_iRight;
	rect.top = m_iTop;
	rect.bottom = m_iBottom;
	return rect;
}
