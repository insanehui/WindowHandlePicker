// ==========================================================================
// File : OptionsDlg.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 22/02/2000
// By            : Marquet Mike
// ==========================================================================

#if !defined(AFX_OPTIONSDLG_H__807F82A3_E9FA_11D3_A113_004005555C30__INCLUDED_)
#define AFX_OPTIONSDLG_H__807F82A3_E9FA_11D3_A113_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
 {
  public :
           COptionsDlg(CWnd *pParent = NULL);

  public :
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONSDLG };
	BOOL	m_bTrayOnStart;
	//}}AFX_DATA

  protected :
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  protected :
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
  
  DECLARE_MESSAGE_MAP()
 };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__807F82A3_E9FA_11D3_A113_004005555C30__INCLUDED_)
