// ==========================================================================
// File : HandlePickerTitleTip.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 21/02/2000
// By            : Marquet Mike
// ==========================================================================

#if !defined(AFX_HANDLEPICKERTITLETIP_H__8D142662_E853_11D3_A111_004005555C30__INCLUDED_)
#define AFX_HANDLEPICKERTITLETIP_H__8D142662_E853_11D3_A111_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHandlePickerTitleTip window

class CHandlePickerTitleTip : public CWnd
 {
  protected :
              static BOOL m_bRegistered;

              CWnd   *m_pParentWnd;
              CRect   m_rectTitle;
              int     m_iHorzSpace;
              HBRUSH  m_hbrBackground;
              BOOL    m_bIsCreated;

  public :
           CHandlePickerTitleTip();
           virtual ~CHandlePickerTitleTip();

           static BOOL RegisterWindowClass();

           virtual BOOL Create(CWnd *pParentWnd);

           void Hide();

           void Show(HWND hWnd);

           // INLINE
           BOOL IsCreated() { return m_bIsCreated; }

           int GetHorzSpace() { return m_iHorzSpace; }

  public :
	//{{AFX_VIRTUAL(CHandlePickerTitleTip)
	//}}AFX_VIRTUAL

  protected :
	//{{AFX_MSG(CHandlePickerTitleTip)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
  
  DECLARE_MESSAGE_MAP()
 };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLEPICKERTITLETIP_H__8D142662_E853_11D3_A111_004005555C30__INCLUDED_)
