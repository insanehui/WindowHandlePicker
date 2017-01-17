// ==========================================================================
// File : WindowHandlePickerDlg.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 21/02/2000
// By            : Marquet Mike
// ==========================================================================

#if !defined(AFX_WINDOWHANDLEPICKERDLG_H__45D6A4E7_E46D_11D3_A10D_004005555C30__INCLUDED_)
#define AFX_WINDOWHANDLEPICKERDLG_H__45D6A4E7_E46D_11D3_A10D_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ==========================================================================
// Les Includes
// ==========================================================================

#include "HexaEdit.h"
#include "HandlePickerTitleTip.h"

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerDlg dialog

class CWindowHandlePickerDlg : public CDialog
 {
  public :
           CWindowHandlePickerDlg(CWnd *pParent = NULL);

  protected :
              HICON                 m_hIcon;
              HCURSOR               m_hWindowFinderCursor;
              HWND                  m_hLastHWND;
              CBitmap               m_WindowFinder1Bitmap;
              CBitmap               m_WindowFinder2Bitmap;
              CHandlePickerTitleTip m_HandlePickerTitleTip;

              void DisplayWindowInfo(HWND hWnd = NULL, BOOL bKeyboardInput = FALSE);
              void InvertTracker(HWND hWnd);
              void StartTracking(BOOL bFromTrayIcon, CPoint point);
              void StopTracking();

  public :
	//{{AFX_DATA(CWindowHandlePickerDlg)
	enum { IDD = IDD_WINDOWHANDLEPICKER_DIALOG };
	CHexaEdit	m_EDHandle;
	BOOL	m_CBAlwaysOnTop;
	//}}AFX_DATA

  protected :
	//{{AFX_VIRTUAL(CWindowHandlePickerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWindowHandlePickerDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChange_Handle();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTrayMenu_ShowInterface();
	afx_msg void OnTrayMenu_OptionsDlg();
	afx_msg void OnTrayMenu_AboutBox();
	afx_msg void OnCB_AlwaysOnTop();
	//}}AFX_MSG
  afx_msg LRESULT OnNotifyTaskBarIcon(WPARAM wParam, LPARAM lParam);
  
  DECLARE_MESSAGE_MAP()
 };

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWHANDLEPICKERDLG_H__45D6A4E7_E46D_11D3_A10D_004005555C30__INCLUDED_)
