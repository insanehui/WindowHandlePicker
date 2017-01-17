// ==========================================================================
// File : TaskBarFunctions.cpp
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
#include "TaskBarFunctions.h"

// ==========================================================================
// Les Fonctions
// ==========================================================================

// --------------------------------------------------------------------------
// AddTaskBarIcon : adds an icon to the taskbar status area
//
// Returns TRUE if successful, or FALSE otherwise
//
// hwnd    : handle to the window to receive callback messages
// uID     : identifier of the icon
// hicon   : handle to the icon to add
// lpszTip : tooltip text
// --------------------------------------------------------------------------

BOOL AddTaskBarIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip)
 { 
  BOOL           res;
  NOTIFYICONDATA tnid;

  tnid.cbSize           = sizeof(NOTIFYICONDATA);
  tnid.hWnd             = hwnd;
  tnid.uID              = uID;
  tnid.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
  tnid.uCallbackMessage = WM_NOTIFYTASKBARICON;
  tnid.hIcon            = hicon;
  
  if (lpszTip) lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip));
  else tnid.szTip[0] = '\0';

  res = Shell_NotifyIcon(NIM_ADD, &tnid);

  if (hicon) DestroyIcon(hicon);

  return res;
 }

// --------------------------------------------------------------------------
// DeleteTaskBarIcon : deletes an icon from the taskbar status area
//
// Returns TRUE if successful, or FALSE otherwise
//
// hwnd : handle to the window that added the icon
// uID  : identifier of the icon to delete
// --------------------------------------------------------------------------

BOOL DeleteTaskBarIcon(HWND hwnd, UINT uID)
 { 
  BOOL           res;
  NOTIFYICONDATA tnid;

  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd   = hwnd;
  tnid.uID    = uID;
     
  res = Shell_NotifyIcon(NIM_DELETE, &tnid);

  return res;
 } 

// --------------------------------------------------------------------------
// GetTaskBarPosition : retrieve the rectangle coordinates of the taskbar
//                      window.
//
// Returns -1 : if en error occured
//          0 : if taskbar is at the left of the screen
//          1 : if taskbar is at the top of the screen
//          2 : if taskbar is at the right of the screen
//          3 : if taskbar is at the bottom of the screen
// --------------------------------------------------------------------------

int GetTaskBarPosition(RECT *pstRect)
 {
  // Get rectangle coordinates of the taskbar
  APPBARDATA stABD;

  stABD.cbSize = sizeof(stABD);
  stABD.hWnd   = 0;

  BOOL bRet = (BOOL)SHAppBarMessage(ABM_GETTASKBARPOS, &stABD);

  if (bRet == FALSE) return -1;

  int iRet = 1;

  // Get position of status bar icon.
  if ((stABD.rc.left < GetSystemMetrics(SM_CXSCREEN) / 2) && (stABD.rc.top > GetSystemMetrics(SM_CYSCREEN) / 2)) iRet = 3;

  else if ((stABD.rc.left > GetSystemMetrics(SM_CXSCREEN) / 2) && (stABD.rc.top < GetSystemMetrics(SM_CYSCREEN) / 2)) iRet = 2;
     
       else if ((stABD.rc.left < GetSystemMetrics(SM_CXSCREEN) / 2) && (stABD.rc.top < GetSystemMetrics(SM_CYSCREEN) / 2) && (stABD.rc.right < GetSystemMetrics(SM_CXSCREEN) / 2)) iRet = 0;

            else iRet = 1;

  if (pstRect) memcpy(pstRect, &stABD.rc, sizeof(RECT));

  return iRet;
 }

// ==========================================================================
// ==========================================================================
