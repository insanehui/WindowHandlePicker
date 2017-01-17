// ==========================================================================
// File : AboutDlg.cpp
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 16/02/2000
// By            : Marquet Mike
// ==========================================================================

// ==========================================================================
// Les Includes
// ==========================================================================

#include "stdafx.h"
#include "WindowHandlePicker.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

CAboutDlg::CAboutDlg(CWnd* pParent) : CDialog(CAboutDlg::IDD, pParent)
 {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
 }

// --------------------------------------------------------------------------

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
 {
  CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
 }

// --------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers
