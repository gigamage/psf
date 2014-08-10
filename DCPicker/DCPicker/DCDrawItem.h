
#pragma once

#ifdef _WIN32_WCE
#error "COleClientItem is not supported for Windows CE."
#endif 

// CDCDrawItem command target

class CDCDrawOleObj;

class CDCDrawItem : public COleClientItem
{
	DECLARE_DYNAMIC(CDCDrawItem)

public:
	CDCDrawItem(COleDocument* pContainerDoc = NULL, CDCDrawOleObj* pDrawObj = NULL);
	virtual ~CDCDrawItem();

	// Attributes
	CDCDrawOleObj* m_pDrawObj;    // back pointer to OLE draw object

protected:
	DECLARE_MESSAGE_MAP()
};


