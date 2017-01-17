// ==========================================================================
// File : WindowHandlePicker.cpp
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
#include "WindowHandlePickerDlg.h"

#include "External Libraries Files\Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerApp

BEGIN_MESSAGE_MAP(CWindowHandlePickerApp, CWinApp)
	//{{AFX_MSG_MAP(CWindowHandlePickerApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerApp construction

CWindowHandlePickerApp::CWindowHandlePickerApp()
 {
  m_hFileMapping = NULL;
 }

/////////////////////////////////////////////////////////////////////////////
// The one and only CWindowHandlePickerApp object

CWindowHandlePickerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerApp initialization

int CWindowHandlePickerApp::ExitInstance() 
 {
  CloseHandle(m_hFileMapping);
	
  return CWinApp::ExitInstance();
 }

// --------------------------------------------------------------------------

BOOL CWindowHandlePickerApp::InitInstance()
 {
  // Check for preview instance
  if (IsPreviewInstance()) return FALSE;

  #ifdef _AFXDLL
  Enable3dControls();			// Call this when using MFC in a shared DLL
  #else
  Enable3dControlsStatic();	// Call this when linking to MFC statically
  #endif

  free((void*)m_pszAppName);
  m_pszAppName=_tcsdup(_T(APPLICATION_REG_NAME));

  SetRegistryKey(_T(MAIN_REG_KEY)); // Nom qui apparaitra dans (HKEY_CURRENT_USER\Software)

  LoadStdProfileSettings(0);

  if (CreateRegistryKeys() < 0)
   {
    AfxMessageBox(IDS_REGISTRYERROR);
    return FALSE;
   }

  CWindowHandlePickerDlg dlg;
  m_pMainWnd = &dlg;
  
  dlg.DoModal();
  
  return FALSE;
 }

// --------------------------------------------------------------------------

int CWindowHandlePickerApp::CreateRegistryKeys()
 {
  CRegistry cRegistry;

  cRegistry.SetDefaultRootKey(HKEY_CURRENT_USER);
  cRegistry.UseDefaultSubKey(TRUE);
  cRegistry.SetDefaultSubKey(DEFAULT_SUBKEY);

  if (cRegistry.IsNameValid("", REG_TRAYONSTART) < 0)
   {
    if (cRegistry.CreateValue("", REG_TRAYONSTART, CRegistry::regDWORD) < 0) return -1;
   }

  return 0;
 }

// --------------------------------------------------------------------------

BOOL CWindowHandlePickerApp::IsPreviewInstance()
 {
  CString str;

  str.FormatMessage(IDS_APPLICATIONNAME);

  m_hFileMapping = CreateFileMapping( (HANDLE) 0xFFFFFFFF,
                                      NULL,
                                      PAGE_READONLY,
                                      0,
                                      32,
                                      str );

  if (m_hFileMapping)
   {
    if (GetLastError() == ERROR_ALREADY_EXISTS)
     {
      MessageBox(NULL, "Application is already running.", str, MB_OK);
      return TRUE;
     }
   }
  else {
        MessageBox(NULL, "Error creating file mapping", str, MB_OK);
        return TRUE;
       }

  return FALSE;
 }

// --------------------------------------------------------------------------
