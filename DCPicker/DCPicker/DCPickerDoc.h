
// DCPickerDoc.h : interface of the CDCPickerDoc class
//


#pragma once


class CDCPickerDoc : public COleDocument
{
protected: // create from serialization only
	CDCPickerDoc();
	DECLARE_DYNCREATE(CDCPickerDoc)

// Attributes
public:
	const CSize& GetSize() const { return m_size; }
	void ComputePageSize();
	CDCDrawObjList* GetObjects() { return &m_objects; }
	COLORREF GetPaperColor() const { return m_paperColor; }

// Operations
public:
	CDCDrawObj* ObjectAt(const CPoint& point);
	void Draw(CDC* pDC, CDCPickerView* pView);

	void Add(CDCDrawObj* pObj);
	void Remove(CDCDrawObj* pObj);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDCPickerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSize m_size;
	int m_nMapMode;
	CDCDrawObjList m_objects;
	COLORREF m_paperColor;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
