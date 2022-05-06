// aDoc.h : interface of the CADoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOC_H__3FEA04EA_2AE3_4FC3_9E47_935C30072A2A__INCLUDED_)
#define AFX_ADOC_H__3FEA04EA_2AE3_4FC3_9E47_935C30072A2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dibapi.h" 

class CADoc : public CDocument
{
protected: // create from serialization only
	CADoc();
	DECLARE_DYNCREATE(CADoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitDIBData();
	void ReplaceHDIB(HDIB hDIB);
	virtual ~CADoc();
	COLORREF m_refColorBKG;		// ±³¾°É«
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CADoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
		HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADOC_H__3FEA04EA_2AE3_4FC3_9E47_935C30072A2A__INCLUDED_)
