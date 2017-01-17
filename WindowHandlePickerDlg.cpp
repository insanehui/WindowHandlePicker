// ==========================================================================
// File : WindowHandlePickerDlg.cpp
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
#include "WindowHandlePickerDlg.h"
#include "AboutDlg.h"
#include "TaskBarFunctions.h"
#include "OptionsDlg.h"

#include "External Libraries Files\Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Les Defines
// ==========================================================================

#define USE_TITLETIP

#define ID_TRAYICON   1234

// ==========================================================================
// Les Fonctions
// ==========================================================================

HWND RealChildWindowFromPoint(HWND hWndParent, POINT stPoint)
 {
  ::ClientToScreen(hWndParent, &stPoint);

  HWND hWndPoint = ::WindowFromPoint(stPoint);

  if (!hWndPoint) return NULL;

  ::ScreenToClient(hWndPoint, &stPoint);

  HWND hWndChild = ::ChildWindowFromPointEx(hWndPoint, stPoint, CWP_ALL);

  ::ClientToScreen(hWndPoint, &stPoint);

  if (!hWndChild) return hWndPoint;

  if (::IsChild(::GetParent(hWndChild),hWndChild) == FALSE) 
   {
    return hWndChild;
   }

  HWND  hFirstChildWnd = ::GetWindow(hWndChild, GW_HWNDFIRST);
  HWND  hNextChildWnd  = hFirstChildWnd;
  BOOL  bPointIncluded;
  RECT  stRect;
  int   iChildListSize = 0;

  struct CHILD_LIST
   {
    HWND hChildWnd;
    RECT stChildRect;
   } *pstChildList = NULL;

  while (hNextChildWnd)
   {
    bPointIncluded = TRUE;

    ::GetWindowRect(hNextChildWnd, &stRect);

    if (stPoint.x < stRect.left || stPoint.x > stRect.right) bPointIncluded = FALSE;

    if (stPoint.y < stRect.top || stPoint.y > stRect.bottom) bPointIncluded = FALSE;

    if (bPointIncluded)
     {
      iChildListSize++;

      pstChildList = (CHILD_LIST *)realloc(pstChildList, iChildListSize * sizeof(CHILD_LIST));

      ::GetWindowRect(hNextChildWnd, &stRect);

      pstChildList[iChildListSize - 1].hChildWnd = hNextChildWnd;

      pstChildList[iChildListSize - 1].stChildRect = stRect;
     }

    hNextChildWnd = ::GetWindow(hNextChildWnd, GW_HWNDNEXT);
   }

  // 'hChildList' contient une liste de toutes les fenêtres contenant le
  // point considéré. La nombre de handle contenus dans le liste est 'iChildListSize'

  // Parcourir la liste pour prendre l'index de la fenêtre qui à la dimension
  // la plus petite.
  int iMinNbrPixel = ::GetSystemMetrics(SM_CXFULLSCREEN) * ::GetSystemMetrics(SM_CYFULLSCREEN);
  int iIndex       = -1;

  for (int I=0; I<iChildListSize; I++)
   {
    int iNbrPixel = (pstChildList[I].stChildRect.right - pstChildList[I].stChildRect.left) * (pstChildList[I].stChildRect.bottom - pstChildList[I].stChildRect.top);

    if (iNbrPixel < iMinNbrPixel)
     {
      iMinNbrPixel = iNbrPixel;
      iIndex       = I;
     }
   }

  HWND hRealWnd = (iIndex != -1) ? pstChildList[iIndex].hChildWnd : NULL;

  if (!hRealWnd && pstChildList) hRealWnd = pstChildList[0].hChildWnd;

  if (pstChildList) free(pstChildList);

  iChildListSize = 0;

  return hRealWnd;
 }

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerDlg dialog

CWindowHandlePickerDlg::CWindowHandlePickerDlg(CWnd *pParent)
 : CDialog(CWindowHandlePickerDlg::IDD, pParent)
 {
	//{{AFX_DATA_INIT(CWindowHandlePickerDlg)
	m_CBAlwaysOnTop = FALSE;
	//}}AFX_DATA_INIT

  m_hIcon               = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_hWindowFinderCursor = AfxGetApp()->LoadCursor(IDC_WINDOWFINDER);
  m_hLastHWND           = NULL;

  m_WindowFinder1Bitmap.LoadBitmap(IDB_WINDOWFINDER1);
  m_WindowFinder2Bitmap.LoadBitmap(IDB_WINDOWFINDER2);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::DisplayWindowInfo(HWND hWnd, BOOL bKeyboardInput)
 {
  if (!hWnd)
   {
    m_hLastHWND = NULL;

    if (!bKeyboardInput) SetDlgItemText(IDED_HANDLE, "");

    SetDlgItemText(IDST_ID     , "");
    SetDlgItemText(IDST_CAPTION, "");
    SetDlgItemText(IDST_CLASS  , "");
    SetDlgItemText(IDST_STYLE  , "");
    SetDlgItemText(IDST_EXSTYLE, "");
    SetDlgItemText(IDST_RECT   , "");

    #ifdef USE_TITLETIP
    m_HandlePickerTitleTip.Hide();
    #endif

    return;
   }

  CString str;
  char    szBuf[255];
  RECT    stRect;
  LONG    lStyle;
  LONG    lExStyle;
  int     iID;

  str.Format("%08X", hWnd);

  if (!bKeyboardInput) SetDlgItemText(IDED_HANDLE, str);

  iID = ::GetDlgCtrlID(hWnd);

  if (iID == 0) str.Format("");
  else str.Format("%d", iID);

  SetDlgItemText(IDST_ID, str);

  ::GetWindowText(hWnd, szBuf, sizeof(szBuf));

  str.Format("\"%s\"", szBuf);

  SetDlgItemText(IDST_CAPTION, str);

  ::GetClassName(hWnd, szBuf, sizeof(szBuf));

  SetDlgItemText(IDST_CLASS, szBuf);

  lStyle = GetWindowLong(hWnd, GWL_STYLE);

  str.Format("%X", lStyle);

  SetDlgItemText(IDST_STYLE, str);

  lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

  str.Format("%X", lExStyle);

  SetDlgItemText(IDST_EXSTYLE, str);

  ::GetWindowRect(hWnd, &stRect);

  str.Format("(%d,%d)-(%d,%d) %dx%d", stRect.left,
                                      stRect.top,
                                      stRect.right,
                                      stRect.bottom,
                                      stRect.right - stRect.left,
                                      stRect.bottom - stRect.top);

  SetDlgItemText(IDST_RECT, str);

  #ifdef USE_TITLETIP
  if (GetCapture()) m_HandlePickerTitleTip.Show(hWnd);
  #endif
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::InvertTracker(HWND hWnd)
 {
  RECT stRect;

  // Get the coordinates of the window on the screen
  ::GetWindowRect(hWnd, &stRect);

  // Get a handle to the window's device context
  HDC hDC = ::GetWindowDC(hWnd);

  // Create an inverse pen that is the size of the window border
  SetROP2(hDC, R2_NOT);
  
  HPEN hPen = CreatePen(PS_INSIDEFRAME, 3 * GetSystemMetrics(SM_CXBORDER), RGB(0,0,0));

  // Draw the rectangle around the window
  HPEN   hOldPen   = (HPEN)SelectObject(hDC, hPen);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

  Rectangle(hDC, 0, 0, stRect.right - stRect.left, stRect.bottom - stRect.top);

  SelectObject(hDC, hOldBrush);
  SelectObject(hDC, hOldPen);

  //Give the window its device context back, and destroy our pen
  ::ReleaseDC(hWnd, hDC);

  DeleteObject(hPen);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::StartTracking(BOOL bFromTrayIcon, CPoint point)
 {
  CStatic *pstWindowFinder = (CStatic *)GetDlgItem(IDPIC_WINDOWFINDER); 
  CRect    rect; 
  CRect    rectDlg; 

  if (bFromTrayIcon == FALSE)
   {
    GetClientRect(&rectDlg); 

    pstWindowFinder->GetWindowRect(&rect); 

    ScreenToClient(rect); 

    if (point.x < rect.left || rect.right < point.x) return; 

    if (point.y < rect.top || rect.bottom < point.y) return; 
   }

  ((CStatic *)GetDlgItem(IDPIC_WINDOWFINDER))->SetBitmap(m_WindowFinder2Bitmap); 

  DisplayWindowInfo(); 

  SetCursor(m_hWindowFinderCursor); 

  SetCapture();
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::StopTracking()
 {
  if (GetCapture() != this) return;

  #ifdef USE_TITLETIP
  m_HandlePickerTitleTip.Hide();
  #endif

  if (m_hLastHWND) InvertTracker(m_hLastHWND);

  m_hLastHWND = NULL; 

  ::InvalidateRect(NULL, NULL, TRUE);

  ReleaseCapture(); 

  ((CStatic *)GetDlgItem(IDPIC_WINDOWFINDER))->SetBitmap(m_WindowFinder1Bitmap); 
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::DoDataExchange(CDataExchange *pDX)
 {
  CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowHandlePickerDlg)
	DDX_Control(pDX, IDED_HANDLE, m_EDHandle);
	DDX_Check(pDX, IDCB_ALWAYSONTOP, m_CBAlwaysOnTop);
	//}}AFX_DATA_MAP
 }

// --------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CWindowHandlePickerDlg, CDialog)
	//{{AFX_MSG_MAP(CWindowHandlePickerDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDED_HANDLE, OnChange_Handle)
	ON_WM_SIZE()
	ON_COMMAND(IDM_SHOWINTERFACE, OnTrayMenu_ShowInterface)
	ON_COMMAND(IDM_OPTIONSDLG, OnTrayMenu_OptionsDlg)
	ON_COMMAND(IDM_ABOUTBOX, OnTrayMenu_AboutBox)
	ON_BN_CLICKED(IDCB_ALWAYSONTOP, OnCB_AlwaysOnTop)
	//}}AFX_MSG_MAP
  ON_MESSAGE(WM_NOTIFYTASKBARICON, OnNotifyTaskBarIcon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowHandlePickerDlg message handlers

BOOL CWindowHandlePickerDlg::DestroyWindow() 
 {
  m_HandlePickerTitleTip.DestroyWindow();
	
  DeleteTaskBarIcon(GetSafeHwnd(), ID_TRAYICON);

  return CDialog::DestroyWindow();
 }

// --------------------------------------------------------------------------

BOOL CWindowHandlePickerDlg::OnInitDialog() 
 {
  CDialog::OnInitDialog();

  CString str;

  str.FormatMessage(IDS_APPLICATIONNAME);

  SetWindowText(str);
	
  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu *pSysMenu = GetSystemMenu(FALSE);
  
  if (pSysMenu != NULL)
   {
    CString str;

    // Add "Options" menu item to system menu
    str.LoadString(IDS_OPTIONS);

    if (!str.IsEmpty())
     {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_OPTIONSDLG, str);
     }

    // Add "About..." menu item to sytem menu
    str.LoadString(IDS_ABOUTBOX);

    if (!str.IsEmpty())
     {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, str);
     }
   }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon

  m_HandlePickerTitleTip.Create(this);

  // Apply options from registry
  CRegistry cRegistry;
  DWORD     dwTrayOnStart;

  cRegistry.SetDefaultRootKey(HKEY_CURRENT_USER);
  cRegistry.UseDefaultSubKey(TRUE);
  cRegistry.SetDefaultSubKey(DEFAULT_SUBKEY);

  cRegistry.GetValue("", REG_TRAYONSTART, dwTrayOnStart);

  if (dwTrayOnStart)
   {
    PostMessage(WM_SIZE, (WPARAM)SIZE_MINIMIZED, MAKELPARAM(0,0));
    //ModifyStyle(0, WS_MINIMIZE);
   }

  return TRUE;
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
 {
  CDialog::OnLButtonDown(nFlags, point);

  StartTracking(FALSE, point);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
 {
  CDialog::OnLButtonUp(nFlags, point);

  StopTracking();
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnMouseMove(UINT nFlags, CPoint point) 
 {
  CDialog::OnMouseMove(nFlags, point);

  if (GetCapture() != this) return;

  HWND hWnd = RealChildWindowFromPoint(GetSafeHwnd(), point);

  ClientToScreen(&point);

  #ifdef USE_TITLETIP
  if (m_HandlePickerTitleTip.GetSafeHwnd() == hWnd) return;
  #endif

  if (hWnd == GetSafeHwnd() || ::GetParent(hWnd) == GetSafeHwnd())
   {
    DisplayWindowInfo();
    m_hLastHWND = NULL;
    return;
   }

  if (hWnd != m_hLastHWND)
   { 
    if (m_hLastHWND)
     {
      InvertTracker(m_hLastHWND);
      
      //::InvalidateRect(m_hLastHWND, NULL, TRUE);
     }

    m_hLastHWND = hWnd; 

    DisplayWindowInfo(m_hLastHWND, FALSE); 

    InvertTracker(m_hLastHWND);
   } 
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnPaint() 
 {
  if (IsIconic())
   {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
   }
  else CDialog::OnPaint();
 }

// --------------------------------------------------------------------------

HCURSOR CWindowHandlePickerDlg::OnQueryDragIcon()
 {
  return (HCURSOR)m_hIcon;
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
 {
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
   {
    OnTrayMenu_AboutBox();
    return;
   }
  
  if ((nID & 0xFFF0) == SC_CLOSE)
   {
    if (AfxMessageBox("close application ?", MB_YESNO) == IDNO) return;
   }

  if (nID == IDM_OPTIONSDLG)
   {
    OnTrayMenu_OptionsDlg();
    return;
   }

  CDialog::OnSysCommand(nID, lParam);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnSize(UINT nType, int cx, int cy) 
 {
  if (nType == SIZE_MINIMIZED)
   {
    AddTaskBarIcon(GetSafeHwnd(), ID_TRAYICON, AfxGetApp()->LoadIcon(IDR_MAINFRAME), "Window Handle Picker");
    ShowWindow(SW_HIDE);

    DisplayWindowInfo();
    m_hLastHWND = NULL;
   }

  CDialog::OnSize(nType, cx, cy);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnChange_Handle() 
 {
  HWND hWnd = (HWND)m_EDHandle.GetHexaValue();

  if ( !IsWindow(hWnd) )
   {
    DisplayWindowInfo(NULL, TRUE);
    return;
   }

  DisplayWindowInfo(hWnd, TRUE);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnCB_AlwaysOnTop() 
 {
  UpdateData(TRUE);

  if (m_CBAlwaysOnTop)
   {
    SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   }
  else {
        SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
       }
 }

// --------------------------------------------------------------------------

LRESULT CWindowHandlePickerDlg::OnNotifyTaskBarIcon(WPARAM wParam, LPARAM lParam)
 {
  UINT uID       = (UINT)wParam;
  UINT uMouseMsg = (UINT)lParam;

  if (uID != ID_TRAYICON) return 0L;

  switch(uMouseMsg)
   {
    case WM_LBUTTONDOWN :
                          //StartTracking(TRUE, CPoint(0,0));
                          break;

    case WM_LBUTTONDBLCLK :
                            OnTrayMenu_ShowInterface();
                            break;

    case WM_RBUTTONDOWN :
                          {
                           CMenu cTaskBarDummyMenu;

                           if (!cTaskBarDummyMenu.LoadMenu(IDM_TASKBARICON)) return 0L;

                           CMenu *pcTaskBarMenu = cTaskBarDummyMenu.GetSubMenu(0);

                           CPoint point;
                           
                           GetCursorPos(&point);

                           SetForegroundWindow(); 

                           if (pcTaskBarMenu)
                            {
                             //pcTaskBarMenu->ModifyMenu(MF_BYCOMMAND, );
                             pcTaskBarMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this, NULL);
                            }

                           PostMessage(NULL,0,0);

                           cTaskBarDummyMenu.DestroyMenu();
                          }
                          break;
   }

  return 0L;
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnTrayMenu_ShowInterface() 
 {
  ShowWindow(SW_NORMAL);

  DeleteTaskBarIcon(GetSafeHwnd(), ID_TRAYICON);
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnTrayMenu_OptionsDlg() 
 {
  COptionsDlg cOptionsDlg;
  cOptionsDlg.DoModal();
 }

// --------------------------------------------------------------------------

void CWindowHandlePickerDlg::OnTrayMenu_AboutBox() 
 {
  CAboutDlg dlgAbout;
  dlgAbout.DoModal();
 }

// --------------------------------------------------------------------------
