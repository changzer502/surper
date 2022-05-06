// aView.h : interface of the CAView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIEW_H__14F04361_ADDD_44AD_A360_28974E3DAA85__INCLUDED_)
#define AFX_AVIEW_H__14F04361_ADDD_44AD_A360_28974E3DAA85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAView : public CView
{
protected: // create from serialization only
	CAView();
	DECLARE_DYNCREATE(CAView)

// Attributes
public:
	CADoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL LSharp(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	unsigned char GetMedianNum(unsigned char *b, int num);
	BOOL MedianFilter(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL EhanSmooth(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	HGLOBAL SInterpolaRotate(LPSTR lpDIB, LPBYTE lpDIBBits, LONG lWidth, LONG lHeight, FLOAT RotateAngle);
	HGLOBAL SInterpolaZoom(LPSTR lpDIB, LPBYTE lpDIBBits, LONG lWidth, LONG lHeight, FLOAT ZoomRatio);
	HGLOBAL LjInterpolaRotate(LPSTR lpDIB, LPBYTE lpDIBBits, LONG lWidth, LONG lHeight,FLOAT RotateAngle );
	HGLOBAL LjInterpolaZoom(LPSTR lpDIB, LPBYTE lpDIBBits, LONG lWidth, LONG lHeight, FLOAT ZoomRatio);
	BOOL ThresholdDIB(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL Linertrans(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	HGLOBAL LianTong(LPSTR lpDIB, LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	HGLOBAL LianTongPhone(LPSTR lpDIB, LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);
	void BackForUndo(HGLOBAL h);
	void Undo();
	LRESULT OnDoRealize(WPARAM wParam, LPARAM);
	virtual ~CAView();
	void * HRedo[3];
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditUndo();
	afx_msg void OnLinetrans();
	afx_msg void OnThreshold();
	afx_msg void OnLjinterpolationzoom();
	afx_msg void OnLjInterpolaRotate();
	afx_msg void OnSInterpolaZoom();
	afx_msg void OnSInterpolaRotate();
	afx_msg void OnENhanSmooth();
	afx_msg void OnMedianFilter();
	afx_msg void OnEnhaLsharp();
	afx_msg void OnMorphErosion();
	afx_msg void OnMorphDilation();
	afx_msg void Onliantong();
	afx_msg void OnLianTongPhone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in aView.cpp
inline CADoc* CAView::GetDocument()
   { return (CADoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVIEW_H__14F04361_ADDD_44AD_A360_28974E3DAA85__INCLUDED_)
