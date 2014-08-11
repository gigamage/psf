// DCDrawItem.cpp : implementation file
//

#include "stdafx.h"
#include "DCPicker.h"
#include "DCDrawObj.h"
#include "DCDrawItem.h"


// CDCDrawItem

IMPLEMENT_DYNAMIC(CDCDrawItem, COleClientItem)

CDCDrawItem::CDCDrawItem(COleDocument* pContainerDoc /*= NULL*/,
					CDCDrawOleObj* pDrawObj)
	: COleClientItem(pContainerDoc)
{
	m_pDrawObj = pDrawObj;
}

CDCDrawItem::~CDCDrawItem()
{
	if (m_pDrawObj != NULL)
		m_pDrawObj->m_pClientItem = NULL;
}


BEGIN_MESSAGE_MAP(CDCDrawItem, COleClientItem)
END_MESSAGE_MAP()



// CDCDrawItem message handlers
