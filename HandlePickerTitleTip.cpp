// ==========================================================================
// File : HandlePickerTitleTip.cpp
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
#include "HandlePickerTitleTip.h"
#include "TaskBarFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Les Defines
// ==========================================================================

#define HANDLEPICKERTITLETIP_CLASSNAME  "Handle Picker Title Tip"

#define TITLETIP_BORDER_X               5
#define TITLETIP_BORDER_Y               5

// ==========================================================================
// Les Variables Globales
// ==========================================================================

// Register control during class initialisation
BOOL CHandlePickerTitleTip::m_bRegistered = CHandlePickerTitleTip::RegisterWindowClass();

/////////////////////////////////////////////////////////////////////////////
// CHandlePickerTitleTip

CHandlePickerTitleTip::CHandlePickerTitleTip()
 {
  m_pParentWnd    = NULL;
  m_iHorzSpace    = 2;
  m_hbrBackground = (HBRUSH)NULL;
  m_bIsCreated    = FALSE;
 }

// --------------------------------------------------------------------------

CHandlePickerTitleTip::~CHandlePickerTitleTip()
 {
 }

// --------------------------------------------------------------------------

BOOL CHandlePickerTitleTip::Create(CWnd *pParentWnd)
 {
  ASSERT_VALID(pParentWnd);

  DWORD dwStyle   = WS_BORDER | WS_POPUP;
  DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

  m_pParentWnd = pParentWnd;

  m_bIsCreated = CreateEx(dwExStyle,
                          HANDLEPICKERTITLETIP_CLASSNAME,
                          NULL,
                          dwStyle,
                          0,
                          0,
                          0,
                          0,
                          NULL,
                          NULL,
                          NULL);

  return m_bIsCreated;
 }

// --------------------------------------------------------------------------

void CHandlePickerTitleTip::Hide()
 {
  ShowWindow(SW_HIDE);
 }

// --------------------------------------------------------------------------

BOOL CHandlePickerTitleTip::RegisterWindowClass()
 {
  WNDCLASS  stWndClass;

  stWndClass.style         = CS_SAVEBITS | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
  stWndClass.lpfnWndProc   = ::DefWindowProc;
  stWndClass.cbClsExtra    = 0;
  stWndClass.cbWndExtra    = 0;
  stWndClass.hInstance     = NULL;
  stWndClass.hIcon         = NULL;
  stWndClass.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
  stWndClass.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1);
  stWndClass.lpszMenuName  = NULL;
  stWndClass.lpszClassName = _T(HANDLEPICKERTITLETIP_CLASSNAME);

  if (!::RegisterClass(&stWndClass))
   {
    AfxThrowResourceException();
    return FALSE;
   }

  return TRUE;
 }

// --------------------------------------------------------------------------

void CHandlePickerTitleTip::Show(HWND hWnd)
 {
  CString strHandle;

  strHandle.Format("%X", hWnd);

  CClientDC dc(this);
  int       iSaveDC = dc.SaveDC();

  dc.SelectObject(m_pParentWnd->GetFont()); // Use same font as parent window

  // Calculate the window size.
  CSize cTextSize = dc.GetTextExtent(strHandle);
  CSize cWindowSize;

  cWindowSize.cx = cTextSize.cx + 2 * TITLETIP_BORDER_X;
  cWindowSize.cy = cTextSize.cy + 2 * TITLETIP_BORDER_Y;

  // Draw informations in window
  dc.SetBkMode(TRANSPARENT);

  ::FillRect(dc, CRect(0,0,cWindowSize.cx,cWindowSize.cy), CBrush(RGB(255,220,255)));

  dc.SetTextColor(RGB(0,0,0));

  dc.DrawText(strHandle, CRect(0,0,cWindowSize.cx,cWindowSize.cy), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

  dc.RestoreDC(iSaveDC);

  // Get work area rectangle
  RECT stWorkRect;

  SystemParametersInfo(SPI_GETWORKAREA, 0, &stWorkRect, 0);

  // Get rectangle coordinates of the taskbar
  int iPos = GetTaskBarPosition();

  // Calculate window rectangle position on screen
  CRect rectWindow;

  switch(iPos)
   {
    case 0 : // LEFT
             rectWindow.left = stWorkRect.left;
             rectWindow.top  = stWorkRect.bottom - cWindowSize.cy;
             break;

    case 1 : // TOP
             rectWindow.left = stWorkRect.right - cWindowSize.cx;
             rectWindow.top  = stWorkRect.top;
             break;

    case 2 : // RIGHT
             rectWindow.left = stWorkRect.right - cWindowSize.cx;
             rectWindow.top  = stWorkRect.bottom - cWindowSize.cy;
             break;

    case 3 : // BOTTOM
             rectWindow.left = stWorkRect.right - cWindowSize.cx;
             rectWindow.top  = stWorkRect.bottom - cWindowSize.cy;
             break;

    default :
              rectWindow.left = 0;
              rectWindow.top  = 0;
              break;
   }

  rectWindow.right  = rectWindow.left + cWindowSize.cx;
  rectWindow.bottom = rectWindow.top + cWindowSize.cy;

  // Display window
  SetWindowPos(&wndTop,
               rectWindow.left,
               rectWindow.top,
               rectWindow.Width(),
               rectWindow.Height(),
               SWP_SHOWWINDOW | SWP_NOACTIVATE);
 }

// --------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CHandlePickerTitleTip, CWnd)
	//{{AFX_MSG_MAP(CHandlePickerTitleTip)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandlePickerTitleTip message handlers

void CHandlePickerTitleTip::OnMouseMove(UINT nFlags, CPoint point) 
 {
	
  CWnd::OnMouseMove(nFlags, point);
 }
