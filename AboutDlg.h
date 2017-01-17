// ==========================================================================
// File : AboutDlg.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 16/02/2000
// By            : Marquet Mike
// ==========================================================================

#if !defined(AFX_ABOUTDLG_H__45D6A4F1_E46D_11D3_A10D_004005555C30__INCLUDED_)
#define AFX_ABOUTDLG_H__45D6A4F1_E46D_11D3_A10D_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
 {
  public :
           CAboutDlg(CWnd* pParent = NULL);

  public :
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

  protected :
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
  
  DECLARE_MESSAGE_MAP()
 };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__45D6A4F1_E46D_11D3_A10D_004005555C30__INCLUDED_)
