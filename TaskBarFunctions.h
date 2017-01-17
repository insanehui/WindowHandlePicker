// ==========================================================================
// File : TaskBarFunctions.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 22/02/2000
// By            : Marquet Mike
// ==========================================================================

#ifndef TASKBARFUNCTIONS_H
#define TASKBARFUNCTIONS_H

// ==========================================================================
// Les Defines
// ==========================================================================

#define WM_NOTIFYTASKBARICON (WM_USER + 244)

// ==========================================================================
// Les Variables Globales
// ==========================================================================

BOOL AddTaskBarIcon(HWND hwnd, UINT uID, HICON hicon, LPSTR lpszTip);
BOOL DeleteTaskBarIcon(HWND hwnd, UINT uID);
int  GetTaskBarPosition(RECT *pstRect = NULL);

// ==========================================================================
// ==========================================================================

#endif // TASKBARFUNCTIONS_H
