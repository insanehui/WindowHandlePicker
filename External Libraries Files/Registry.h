// ==========================================================================
// Registry.h
// 
// Author : Marquet Mike
//
// Last Modified : 28/01/2000
// by            : MMA
// ==========================================================================

#if !defined(AFX_REGISTRY_H__C78F37EE_BCFF_11D2_BAC1_444553540000__INCLUDED_)
#define AFX_REGISTRY_H__C78F37EE_BCFF_11D2_BAC1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define MAX_KEYLENGTH   255

class CRegistry  
 {
  protected :
              HKEY    m_RootKey;
              CString m_strDefaultSubKey;
              char    m_EmptyCar[2];
              BOOL    m_bUseDefaultSubKey;

              BOOL    ByteToStringArray(BYTE *pByteData, DWORD dwSize, CStringArray &strArray);
              int     DeleteKeyNT(const char * szSubKey);
              int     GetDataSize(HKEY hKey, const char * szName, DWORD &dwSize);
              CString GetFullSubKey(const char *szSubKey);
  public :
           CRegistry();
           ~CRegistry();

           int     AreValuesInKey(const char * szSubKey);
           int     CreateKey(const char * szSubKey);
           int     CreateValue(const char * szSubKey, const char * szName, DWORD dwType, SHORT Size = 0);
           int     DeleteKey(const char * szSubKey);
           int     DeleteValue(const char * szSubKey, const char * szValue);
           int     EmptyKey(const char * szSubKey);
           CString ErrorString(LONG Error);
           int     GetBolean(const char * szSubKey, const char * szName, BOOL &bValue);
           int     GetData(const char * szSubKey, const char * szName, void *pData, DWORD &Size);
           int     GetLastIndex(const char * szSubKey);
           int     GetMaxKey(const char * szSubKey);
           BOOL    GetNetmeetingFilename(CString &strFilename);
           int     GetNewIndex(const char * szSubKey);
           int     GetString(const char * szSubKey, const char * szName, CString &strValue);
           int     GetStringArray(const char * szSubKey, const char * szName, CStringArray &strArray);
           int     GetStringFromIndex(const char * szSubKey, const char * szName, CString &strValue, int Index);
           int     GetValue(const char * szSubKey, const char * szName, DWORD &dwValue);
           int     GetValueArray(const char * szSubKey, const char * szName, CUIntArray &dwArray);
           int     GetValueFromIndex(const char * szSubKey, const char * szName, DWORD &dwValue, int Index);
           int     GetWindowPlacement(const char * szSubKey, const char * szName, CWnd *pWindow);
           int     IsKeyValid(const char * szSubKey);
           int     IsNameValid(const char * szSubKey, const char * szName);
           BOOL    NetmeetingUI(BOOL ShowIt = FALSE);
           int     SetBolean(const char * szSubKey, const char * szName, BOOL bValue);
           int     SetData(const char * szSubKey, const char * szName, void *pData, DWORD Size);
           HKEY    SetDefaultRootKey(HKEY RootKey);
           int     SetString(const char * szSubKey, const char * szName, CString strValue);
           int     SetStringArray(const char * szSubKey, const char * szName, CStringArray &strArray);
           int     SetStringAtIndex(const char * szSubKey, const char * szName, CString strValue, int Index);
           int     SetValue(const char * szSubKey, const char * szName, DWORD dwValue);
           int     SetValueArray(const char * szSubKey, const char * szName, CUIntArray &dwValue);
           int     SetValueAtIndex(const char * szSubKey, const char * szName, DWORD dwValue, int Index);
           int     SetWindowPlacement(const char * szSubKey, const char * szName, CWnd *pWindow);

           // Inline
           void SetDefaultSubKey(CString strDefaultSubKey = "") { m_strDefaultSubKey = strDefaultSubKey; }
           void SetEmptyCar(char EmptyCar)                      { m_EmptyCar[0] = EmptyCar; }
           void UseDefaultSubKey(BOOL bUseDefaultSubKey)        { m_bUseDefaultSubKey = bUseDefaultSubKey; }

  enum
   {
    regSTRING      = REG_SZ,
    regDWORD       = REG_DWORD,
    regBINARY      = REG_BINARY,
    regMULTISTRING = REG_MULTI_SZ,
   };
 };

#endif // !defined(AFX_REGISTRY_H__C78F37EE_BCFF_11D2_BAC1_444553540000__INCLUDED_)
