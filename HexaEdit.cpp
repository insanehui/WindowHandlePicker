// ==========================================================================
// File : HexaEdit.cpp
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 21/02/2000
// By            : Marquet Mike
// ==========================================================================

// ==========================================================================
// Les Includes
// ==========================================================================

#include "stdafx.h"
#include <math.h>
#include "HexaEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexaEdit

CHexaEdit::CHexaEdit()
 {
 }

// --------------------------------------------------------------------------

CHexaEdit::~CHexaEdit()
 {
 }

// --------------------------------------------------------------------------

DWORD CHexaEdit::GetHexaValue()
 {
  CString str;

  GetWindowText(str);

  DWORD   dwHexaValue = 0;
  CString strHexa;

  strHexa = "0x" + str;

  char *sz;

  dwHexaValue = (DWORD)strtoul(strHexa, &sz, 16);

  /*
  str.MakeReverse();

  for (int I=0; I<str.GetLength(); I++)
   {
    DWORD dwValue   = (DWORD)pow(16,I);
    int   iValue    = 0;
    char  character = str.GetAt(I);

    if (character >= '0' && character <= '9') iValue = character - '0';

    else if (character >= 'A' && character <= 'F') iValue = character - 'A' + 10;

         else if (character >= 'a' && character <= 'f') iValue = character - 'a' + 10;

    dwHexaValue += (iValue * dwValue);
   }
  */
  
  return dwHexaValue;
 }

// --------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CHexaEdit, CEdit)
	//{{AFX_MSG_MAP(CHexaEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexaEdit message handlers

void CHexaEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
 {
  BOOL bIsIncluded = FALSE;

  if (nChar >= '0' && nChar <= '9') bIsIncluded = TRUE;

  if (nChar >= 'A' && nChar <= 'F') bIsIncluded = TRUE;

  if (nChar >= 'a' && nChar <= 'f') bIsIncluded = TRUE;

  if (nChar == VK_BACK) bIsIncluded = TRUE;
	
  if (!bIsIncluded)
   {
    MessageBeep(-1);
    return;
   }

  CEdit::OnChar(nChar, nRepCnt, nFlags);
 }

// --------------------------------------------------------------------------
