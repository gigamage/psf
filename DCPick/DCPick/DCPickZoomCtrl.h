#pragma once
#include "zoomctrl.h"

class CWorkpadDlg;

class CDCPickZoomCtrl :
	public CZoomCtrl
{
public:
	CDCPickZoomCtrl(void);
	~CDCPickZoomCtrl(void);

	void SetScreenSize(const CRect& rect);
	CDC* GetVirtualDC();
	virtual void Draw(CDC *pDC);

	CWorkpadDlg* GetWorkpad();

protected:
	CImage m_kImage;
};

