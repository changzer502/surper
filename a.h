// a.h : main header file for the A application
//

#if !defined(AFX_A_H__873C6617_92FE_4081_A1E3_5A01E23A626F__INCLUDED_)
#define AFX_A_H__873C6617_92FE_4081_A1E3_5A01E23A626F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAApp:
// See a.cpp for the implementation of this class
//

class CAApp : public CWinApp
{
public:
	CAApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_A_H__873C6617_92FE_4081_A1E3_5A01E23A626F__INCLUDED_)
