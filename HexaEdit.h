// ==========================================================================
// File : HexaEdit.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 21/02/2000
// By            : Marquet Mike
// ==========================================================================

#if !defined(AFX_HEXAEDIT_H__8D142661_E853_11D3_A111_004005555C30__INCLUDED_)
#define AFX_HEXAEDIT_H__8D142661_E853_11D3_A111_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHexaEdit window

class CHexaEdit : public CEdit
 {
  public :
           CHexaEdit();
           virtual ~CHexaEdit();

           DWORD GetHexaValue();

  public :
	//{{AFX_VIRTUAL(CHexaEdit)
	//}}AFX_VIRTUAL

  protected :
	//{{AFX_MSG(CHexaEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()
 };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXAEDIT_H__8D142661_E853_11D3_A111_004005555C30__INCLUDED_)
