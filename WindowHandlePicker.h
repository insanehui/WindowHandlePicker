// ==========================================================================
// File : WindowHandlePicker.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 16/02/2000
// By            : Marquet Mike
// ==========================================================================

#if !defined(AFX_WINDOWHANDLEPICKER_H__45D6A4E5_E46D_11D3_A10D_004005555C30__INCLUDED_)
#define AFX_WINDOWHANDLEPICKER_H__45D6A4E5_E46D_11D3_A10D_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ==========================================================================
// Les Includes
// ==========================================================================

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// ==========================================================================
// Les Defines
// ==========================================================================

#define MAIN_REG_KEY          "Mik Sessions Software"
#define APPLICATION_REG_NAME  "Window Handle Picker"

#define DEFAULT_SUBKEY        "Software\\" MAIN_REG_KEY "\\" APPLICATION_REG_NAME
//#define DEFAULT_SUBKEY        "Software\\Mik Sessions Software\\Window Handle Picker"
#define REG_TRAYONSTART       "tray on start"

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerApp:
// See WindowHandlePicker.cpp for the implementation of this class
//

class CWindowHandlePickerApp : public CWinApp
 {
  protected :
              HANDLE m_hFileMapping; // For preview instance checking

              int  CreateRegistryKeys();
              BOOL IsPreviewInstance();

  public :
           CWindowHandlePickerApp();

  public :
	//{{AFX_VIRTUAL(CWindowHandlePickerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWindowHandlePickerApp)
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()
 };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWHANDLEPICKER_H__45D6A4E5_E46D_11D3_A10D_004005555C30__INCLUDED_)
