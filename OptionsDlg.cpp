// ==========================================================================
// File : OptionsDlg.cpp
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 22/02/2000
// By            : Marquet Mike
// ==========================================================================

// ==========================================================================
// Les Includes
// ==========================================================================

#include "stdafx.h"
#include "WindowHandlePicker.h"
#include "OptionsDlg.h"

#include "External Libraries Files\Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

COptionsDlg::COptionsDlg(CWnd *pParent) : CDialog(COptionsDlg::IDD, pParent)
 {
	//{{AFX_DATA_INIT(COptionsDlg)
	m_bTrayOnStart = FALSE;
	//}}AFX_DATA_INIT
 }

// --------------------------------------------------------------------------

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
 {
  CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Check(pDX, IDCB_STARTTRAYICON, m_bTrayOnStart);
	//}}AFX_DATA_MAP
 }

// --------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
 {
  CDialog::OnInitDialog();
	
  CRegistry cRegistry;
  DWORD     dwValue;

  cRegistry.SetDefaultRootKey(HKEY_CURRENT_USER);
  cRegistry.UseDefaultSubKey(TRUE);
  cRegistry.SetDefaultSubKey(DEFAULT_SUBKEY);

  cRegistry.GetValue("", REG_TRAYONSTART, dwValue);

  m_bTrayOnStart = dwValue ? TRUE : FALSE;

  UpdateData(FALSE);

  return TRUE;
 }

// --------------------------------------------------------------------------

void COptionsDlg::OnOK() 
 {
  UpdateData(TRUE);

  CRegistry cRegistry;

  cRegistry.SetDefaultRootKey(HKEY_CURRENT_USER);
  cRegistry.UseDefaultSubKey(TRUE);
  cRegistry.SetDefaultSubKey(DEFAULT_SUBKEY);

  cRegistry.SetValue("", REG_TRAYONSTART, (DWORD)m_bTrayOnStart);
	
  CDialog::OnOK();
 }

// --------------------------------------------------------------------------
