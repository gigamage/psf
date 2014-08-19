#include "stdafx.h"
#include "resource.h"
#include "WorkpadDlg.h"
#include "DCPickZoomCtrl.h"


CDCPickZoomCtrl::CDCPickZoomCtrl(void)
{
}


CDCPickZoomCtrl::~CDCPickZoomCtrl(void)
{
}

void CDCPickZoomCtrl::SetScreenSize(const CRect& rect)
{
	m_rScreen = rect;
}

CDC* CDCPickZoomCtrl::GetVirtualDC()
{
	return NULL;
}

void CDCPickZoomCtrl::Draw(CDC *pDC)
{
	CRect rClient;
	GetClientRect(rClient);
	pDC->FillRect(&rClient, &CBrush(RGB(128, 0, 0)));

	// define virtual drawing space -- change this to suit
	// if drawing must not distort, then create a rect same shape as rClient
	CRect rVirt(0, 0, 1000, 1000);
	PrepDC(pDC, rVirt, rClient);

	GetWorkpad()->GetImage().Draw(pDC->GetSafeHdc(), rClient.left, rClient.top);
	GetWorkpad()->DrawObjects(pDC);

}

CWorkpadDlg* CDCPickZoomCtrl::GetWorkpad()
{
	return (CWorkpadDlg*)GetParent();
}
