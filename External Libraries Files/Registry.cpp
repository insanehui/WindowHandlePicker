// ==========================================================================
// Registry.cpp
// 
// Author : Marquet Mike
//
// Last Modified : 28/01/2000
// by            : MMA
// ==========================================================================

// ==========================================================================
// Les Includes
// ==========================================================================

#include "stdafx.h"
#include "Registry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

void REG_TRACE(const char * lpszMsg, ...)
 {
  va_list Args;
  char    Buffer[512];

  va_start(Args,lpszMsg);
  vsprintf(Buffer,lpszMsg,Args);
  va_end(Args);                 

  TRACE(Buffer);
 }

/////////////////////////////////////////////////////////////////////////////
// CRegistry class

CRegistry::CRegistry()
 {
  m_strDefaultSubKey.Empty();

  m_RootKey           = HKEY_CLASSES_ROOT;
  m_EmptyCar[0]       = ' ';
  m_EmptyCar[1]       = 0;
  m_bUseDefaultSubKey = FALSE;
 } 

// --------------------------------------------------------------------------

CRegistry::~CRegistry()
 {
 }

// --------------------------------------------------------------------------
//                     PROTECTED FUNCTIONS
// --------------------------------------------------------------------------

BOOL CRegistry::ByteToStringArray(BYTE *pByteData, DWORD dwSize, CStringArray &strArray)
 {
  strArray.RemoveAll();

  if (dwSize == 1 && pByteData[0] == '\0') return TRUE;

  for (DWORD I=0; I<dwSize; I++) 
   if (pByteData[I] == '\0') pByteData[I] = '\255';

  char *token = strtok((char *)pByteData,"\255");
  while (token != NULL)
   {
    // While there are tokens in "pByteData"
    REG_TRACE("(CRegistry) ByteToStringArray(...) : %s\n", token);
    strArray.Add(token);
    // Get next token :
    token = strtok(NULL,"\255");
   }

  return TRUE;
 }

// --------------------------------------------------------------------------

int CRegistry::DeleteKeyNT(const char *szSubKey)
 {
  HKEY    hKey;
  LONG    Error;
  char    szSubKeyName[MAX_KEYLENGTH];
  CString strSubKey;
  
  Error = RegOpenKeyEx(m_RootKey, szSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -2;
   }

  if (EmptyKey(szSubKey) == 0)
   {
    Error = RegDeleteKey(hKey,szSubKey);
    if (Error != ERROR_SUCCESS)
     {
      ErrorString(Error);
      RegCloseKey(hKey);
      return -3;
     }

    return 1;
   }

  do
    {
     Error = RegEnumKey(hKey, 0, szSubKeyName, MAX_KEYLENGTH);
     if (Error != ERROR_SUCCESS)
      {
       ErrorString(Error);
       RegCloseKey(hKey);
       return -4;
      }

     if (Error != ERROR_NO_MORE_ITEMS)
      {
       strSubKey.Format("%s\\%s", szSubKey, szSubKeyName);
       DeleteKeyNT(szSubKey);
      }
    }
  while (Error != ERROR_NO_MORE_ITEMS);

  Error = RegDeleteKey(m_RootKey,szSubKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    return -5;
   }

  RegCloseKey(hKey);

  return 1;
 }

// --------------------------------------------------------------------------

int CRegistry::GetDataSize(HKEY hKey, const char * szName, DWORD &dwSize)
 {
  LONG  Error;
  DWORD dwType;
  //BYTE  ByteData[1];

  dwSize = 0;

  //Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, NULL, &dwSize);
  if (Error != ERROR_SUCCESS && Error != ERROR_MORE_DATA)
   {
    ErrorString(Error);
    //delete [] pByteData;
    return -1;
   }

  return 1;
 }

// --------------------------------------------------------------------------

CString CRegistry::GetFullSubKey(const char *szSubKey)
 {
  CString strFullSubKey;

  if (m_bUseDefaultSubKey && m_strDefaultSubKey.GetLength() > 0)
   {
    strFullSubKey = m_strDefaultSubKey;

    if (strFullSubKey[strFullSubKey.GetLength() - 1] != '\\' && szSubKey[0] != '\\' && szSubKey[0] != 0)
     {
      strFullSubKey += "\\";
     }

    strFullSubKey += szSubKey;
   }
  else strFullSubKey = szSubKey;

  return strFullSubKey;
 }

// --------------------------------------------------------------------------
//                        PUBLIC FUNCTIONS
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// CreateKey : Crée une nouvelle clé dans la base des registres
//             dans la root définit par la fonction 'SetDefaultRootKey'
// --------------------------------------------------------------------------

int CRegistry::CreateKey(const char *szSubKey)
 {
  HKEY    hKey;
  LONG    Error;
  DWORD   dwDisposition;
  CString strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegCreateKeyEx(m_RootKey,
                         strFullSubKey,
                         0,
                         "",
                         REG_OPTION_NON_VOLATILE,
                         KEY_CREATE_SUB_KEY,
                         NULL,
                         &hKey,
                         &dwDisposition);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }
    
  RegCloseKey(hKey);

  if (dwDisposition == REG_OPENED_EXISTING_KEY) return 0;

  return 1;
 }

// --------------------------------------------------------------------------
// CreateValue : Crée une nouvelle valeur
//
//  szSubKey = Clé dans laquel il faut ajouter la nouvelle valeur
//  szName   = Nom de la valeur
//  dwType   = Type de la valeur
//  Size     = Taille de la valeur
// --------------------------------------------------------------------------

int CRegistry::CreateValue(const char * szSubKey, const char * szName, DWORD dwType, SHORT Size)
 {
  HKEY    hKey;
  LONG    Error;
  BYTE    ByteData[1024];
  CString strFullSubKey = GetFullSubKey(szSubKey);

  memset((char *)ByteData, 0, sizeof(ByteData));

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  switch(dwType)
   {
    case regSTRING      :
                          Error = RegSetValueEx(hKey,szName,0,dwType,ByteData,1);
                          if (Error != ERROR_SUCCESS)
                           {
                            ErrorString(Error);
                            RegCloseKey(hKey);
                            return -2;
                           }
                          break;

    case regDWORD       :
                          Error = RegSetValueEx(hKey,szName,0,dwType,ByteData,sizeof(DWORD));
                          if (Error != ERROR_SUCCESS)
                           {
                            ErrorString(Error);
                            RegCloseKey(hKey);
                            return -3;
                           }
                          break;

    case regBINARY      :
                          Error = RegSetValueEx(hKey,szName,0,dwType,ByteData,Size);
                          if (Error != ERROR_SUCCESS)
                           {
                            ErrorString(Error);
                            RegCloseKey(hKey);
                            return -4;
                           }
                          break;

    case regMULTISTRING :
                          Error = RegSetValueEx(hKey,szName,0,dwType,ByteData,1);
                          if (Error != ERROR_SUCCESS)
                           {
                            ErrorString(Error);
                            RegCloseKey(hKey);
                            return -5;
                           }
                          break;

    default             :
                          Error = RegSetValueEx(hKey,szName,0,REG_NONE,ByteData,1);
                          if (Error != ERROR_SUCCESS)
                           {
                            ErrorString(Error);
                            RegCloseKey(hKey);
                            return -6;
                           }
                          break;
   }

  RegCloseKey(hKey);

  return 1;
 }

// --------------------------------------------------------------------------
// DeleteKey : Supprime le clé définit par 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::DeleteKey(const char *szSubKey)
 {
  LONG    Error;
  CString strFullSubKey = GetFullSubKey(szSubKey);

  if (HIWORD(GetVersion()) & 0x8000 == 0x8000) return DeleteKeyNT(strFullSubKey);

  Error = RegDeleteKey(m_RootKey,strFullSubKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  return 1;
 }

// --------------------------------------------------------------------------
// DeleteValue : Supprime la valeur 'szValue' de la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::DeleteValue(const char * szSubKey, const char * szValue)
 {
  HKEY    hKey;
  LONG    Error;
  CString strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  Error = RegDeleteValue(hKey, szValue);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    return -2;
   }

  RegCloseKey(hKey);

  return 1;
 }

// --------------------------------------------------------------------------
// AreValuesInKey : Permet de savoir si une clé n'a pas de valeurs
// --------------------------------------------------------------------------

int CRegistry::AreValuesInKey(const char * szSubKey)
 {
  HKEY    hKey;
  LONG    Error;
  char    szBuf[1024];
  DWORD   dwSize = sizeof(szBuf);
  CString strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  Error = RegEnumValue(hKey, 0, szBuf, &dwSize, NULL, NULL, NULL, NULL);
  if (Error != ERROR_SUCCESS && Error != ERROR_NO_MORE_ITEMS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    return -2;
   }

  RegCloseKey(hKey);

  if (Error == ERROR_NO_MORE_ITEMS) return 0;

  return 1;
 }

// --------------------------------------------------------------------------
// EmptyKey : Permet de savoir si une clé n'a pas de sous clé
// --------------------------------------------------------------------------

int CRegistry::EmptyKey(const char * szSubKey)
 {
  HKEY    hKey;
  LONG    Error;
  char    szSubKeyName[MAX_KEYLENGTH];
  CString strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  Error = RegEnumKey(hKey, 0, szSubKeyName, MAX_KEYLENGTH);
  if (Error != ERROR_SUCCESS && Error != ERROR_NO_MORE_ITEMS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    return -2;
   }

  RegCloseKey(hKey);

  if (Error == ERROR_NO_MORE_ITEMS) return 0;

  return 1;
 }

// --------------------------------------------------------------------------
// ErrorString : Retourne un message d'erreur pour un N° d'erreur spécifique
// --------------------------------------------------------------------------

CString CRegistry::ErrorString(LONG Error)
 {
  CString str;
  LPSTR   szMsgBuf;

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                Error,
                MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                (LPSTR)&szMsgBuf,
                0,
                NULL);
  str = szMsgBuf;
  LocalFree(szMsgBuf);

  REG_TRACE("(CRegistry) ErrorString(%lu) = %s\n", Error, str);

  return str;
 }

// --------------------------------------------------------------------------
// GetValue : Récupère la valeur boleenne pour la valeur 'szName' dans la clé
//            'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::GetBolean(const char * szSubKey, const char * szName, BOOL &bValue)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  BYTE    *pByteData;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  if (GetDataSize(hKey,szName,dwSize) < 0)
   {
    RegCloseKey(hKey);
    return -2;
   }

  pByteData = new BYTE[dwSize];

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -3;
   }

  RegCloseKey(hKey);

  if (dwType != REG_DWORD) return -4;

  DWORD dwValue;

  memcpy((char *)&dwValue, (char *)pByteData, sizeof(DWORD));

  delete [] pByteData;

  if (!dwValue) bValue = FALSE; else bValue = TRUE;

  return 1;
 }

// --------------------------------------------------------------------------
// GetData : Permet de récupérer les données dans la clé 'szSubKey', de la 
//           valeur 'szName'
// 
//  szSubKey = Nom de la clé
//  szName   = Nom de la valeur dont il faut récupérer les données
//  pData    = Pointeur sur le buffer qui doit recevoir les données
//  Size     = Taille de la zone de réception
// --------------------------------------------------------------------------

int CRegistry::GetData(const char * szSubKey, const char * szName, void *pData, DWORD &Size)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  BYTE    *pByteData;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  if (GetDataSize(hKey,szName,dwSize) < 0)
   {
    RegCloseKey(hKey);
    return -2;
   }

  pByteData = new BYTE[dwSize];

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -3;
   }

  RegCloseKey(hKey);

  if (dwSize > Size)
   {
    Size = dwSize;
    delete [] pByteData;
    return 0;
   }

  if (dwType != REG_BINARY) 
   {
    delete [] pByteData;
    return -4;
   }

  Size = dwSize;
  memcpy((BYTE *)pData, (BYTE *)pByteData, dwSize);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// GetLastIndex : Permet de récupérer le dernier index dans la clé
// --------------------------------------------------------------------------

int CRegistry::GetLastIndex(const char * szSubKey)
 {
  HKEY     hKey;
  LONG     Error;
  int      I = 0;
  int      MaxIndex = -1;
  int      Index;
  char     szSubKeyName[MAX_KEYLENGTH];
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  do
    {
     Error = RegEnumKey(hKey, I, szSubKeyName, MAX_KEYLENGTH);
     if (Error != ERROR_SUCCESS && Error != ERROR_NO_MORE_ITEMS)
      {
       ErrorString(Error);
       RegCloseKey(hKey);
       return -2;
      }

     if (Error != ERROR_NO_MORE_ITEMS)
      {
       if (szSubKeyName[0] >= '0' && szSubKeyName[0] <= '9')
        {
         Index = atoi(szSubKeyName);
         if (MaxIndex < Index) MaxIndex = Index;
        }

       I++;
      }
    }
  while (Error != ERROR_NO_MORE_ITEMS);

  RegCloseKey(hKey);

  return MaxIndex;
 }

// --------------------------------------------------------------------------
// GetMaxKey : Retourne le nombre de valeur dans la clé
// --------------------------------------------------------------------------

int CRegistry::GetMaxKey(const char * szSubKey)
 {
  HKEY     hKey;
  LONG     Error;
  int      I = 0;
  char     szSubKeyName[MAX_KEYLENGTH];
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  do
    {
     Error = RegEnumKey(hKey, I, szSubKeyName, MAX_KEYLENGTH);
     if (Error != ERROR_SUCCESS && Error != ERROR_NO_MORE_ITEMS)
      {
       ErrorString(Error);
       RegCloseKey(hKey);
       return -2;
      }

     if (Error != ERROR_NO_MORE_ITEMS) I++;
    }
  while (Error != ERROR_NO_MORE_ITEMS);

  RegCloseKey(hKey);

  return I;
 }

// --------------------------------------------------------------------------
// GetNetmeetingFilename : Retourne le chemin complet vers l'exécutable de 
//                         Netmeeting
// --------------------------------------------------------------------------

BOOL CRegistry::GetNetmeetingFilename(CString &strFilename)
 {
  CString strPath;
  CString strFile;
  HKEY    hPreviewKey = SetDefaultRootKey(HKEY_LOCAL_MACHINE);

  int rValue1 = GetString("SOFTWARE\\Microsoft\\Conferencing", "InstallationDirectory", strPath);
  int rValue2 = GetString("SOFTWARE\\Microsoft\\Conferencing", "NodeControllerName", strFile);

  SetDefaultRootKey(hPreviewKey);

  if (rValue1 == 1 && rValue2 == 1)
   {
    strPath += "\\" + strFile;
    strFilename = strPath;

    // Test path v& file validity
    if (GetFileAttributes(strFilename) == 0xFFFFFFFF) return FALSE;
   }
  
  return FALSE;
 }

// --------------------------------------------------------------------------
// GetNewIndex : Retourne un nouvelle index pour la clé spécifié
// --------------------------------------------------------------------------

int CRegistry::GetNewIndex(const char * szSubKey)
 {
  HKEY     hKey;
  LONG     Error;
  int      I = 0;
  int      MaxIndex = -1;
  int      Index;
  char     szSubKeyName[MAX_KEYLENGTH];
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  do
    {
     Error = RegEnumKey(hKey, I, szSubKeyName, MAX_KEYLENGTH);
     if (Error != ERROR_SUCCESS && Error != ERROR_NO_MORE_ITEMS)
      {
       ErrorString(Error);
       RegCloseKey(hKey);
       return -2;
      }

     if (Error != ERROR_NO_MORE_ITEMS)
      {
       if (szSubKeyName[0] >= '0' && szSubKeyName[0] <= '9')
        {
         Index = atoi(szSubKeyName);
         if (MaxIndex < Index) MaxIndex = Index;
        }

       I++;
      }
    }
  while (Error != ERROR_NO_MORE_ITEMS);

  RegCloseKey(hKey);

  return MaxIndex+1;
 }

// --------------------------------------------------------------------------
// GetString : Récupère la chaine de caractère pour la valeur 'szName' dans
//             la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::GetString(const char * szSubKey, const char * szName, CString &strValue)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  BYTE    *pByteData;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  if (GetDataSize(hKey,szName,dwSize) < 0)
   {
    RegCloseKey(hKey);
    return -2;
   }

  pByteData = new BYTE[dwSize];

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -3;
   }

  RegCloseKey(hKey);

  if (dwType != REG_SZ)
   {
    delete [] pByteData;
    return -4;
   }

  strValue = pByteData;

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// GetStringArray : Récupère le tableau de caractère pour la valeur 'szName'
// --------------------------------------------------------------------------

int CRegistry::GetStringArray(const char * szSubKey, const char * szName, CStringArray &strArray)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  BYTE    *pByteData;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  if (GetDataSize(hKey,szName,dwSize) < 0)
   {
    RegCloseKey(hKey);
    return -2;
   }

  pByteData = new BYTE[dwSize];

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -3;
   }

  RegCloseKey(hKey);

  if (dwType != REG_MULTI_SZ) 
   {
    delete [] pByteData;
    return -4;
   }

  if (ByteToStringArray(pByteData,dwSize-2,strArray) == FALSE)
   {
    delete [] pByteData;
    strArray.RemoveAll();
    return -5;
   }

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// GetStringFromIndex : Récupère la chaine de caractère pour la valeur
//                      'szName' se trouvant à l'index N°<Index>
// --------------------------------------------------------------------------

int CRegistry::GetStringFromIndex(const char * szSubKey, const char * szName, CString &strValue, int Index)
 {
  CStringArray strArray;
  int          rValue;
  CString      strFullSubKey = GetFullSubKey(szSubKey);

  rValue = GetStringArray(strFullSubKey,szName,strArray);
  if (rValue < 0) return rValue;

  if (strArray.GetSize() == 0) return 0;
  if (Index >= strArray.GetSize()) return -6;
  
  strValue = strArray[Index];
  
  return 1;
 }

// --------------------------------------------------------------------------
// GetValue : Récupère la valeur pour la valeur 'szName' dans la clé
//            'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::GetValue(const char * szSubKey, const char * szName, DWORD &dwValue)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  BYTE    *pByteData;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  if (GetDataSize(hKey,szName,dwSize) < 0)
   {
    RegCloseKey(hKey);
    return -2;
   }

  pByteData = new BYTE[dwSize];

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -3;
   }

  RegCloseKey(hKey);

  if (dwType != REG_DWORD) return -4;

  memcpy((char *)&dwValue, (char *)pByteData, sizeof(DWORD));

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// GetValueArray : Récupère le tableau de valeurs pour la valeur 'szName'
// --------------------------------------------------------------------------

int CRegistry::GetValueArray(const char * szSubKey, const char * szName, CUIntArray &dwArray)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  BYTE    *pByteData;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  dwArray.RemoveAll();

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  if (GetDataSize(hKey,szName,dwSize) < 0)
   {
    RegCloseKey(hKey);
    return -2;
   }

  pByteData = new BYTE[dwSize];

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, pByteData, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -3;
   }

  RegCloseKey(hKey);

  if (dwType != REG_BINARY) 
   {
    delete [] pByteData;
    return -4;
   }

  if (dwSize % sizeof(DWORD))
   {
    delete [] pByteData;
    return -5;
   }

  for (int I=0, J=0; I<(int)(dwSize / sizeof(DWORD)); I++, J += sizeof(DWORD))
   {
    DWORD dwValue;
    memcpy((BYTE *)&dwValue, (BYTE *)(pByteData + J), sizeof(DWORD));
    dwArray.Add(dwValue);
   }

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// GetValueFromIndex : Récupère la valeur pour la valeur 'szName' se trouvant
//                     à l'index N°<Index>
// --------------------------------------------------------------------------

int CRegistry::GetValueFromIndex(const char * szSubKey, const char * szName, DWORD &dwValue, int Index)
 {
  CUIntArray dwArray;
  int        rValue;
  CString    strFullSubKey = GetFullSubKey(szSubKey);

  rValue = GetValueArray(strFullSubKey,szName,dwArray);
  if (rValue < 0) return rValue;

  if (dwArray.GetSize() == 0) return 0;
  if (Index >= dwArray.GetSize()) return -6;
  
  dwValue = dwArray[Index];
  
  return 1;
 }

// --------------------------------------------------------------------------
// GetWindowPlacement : Permet de sauvegarder la position et la taille de la
//                      fenétre passée en argument
// --------------------------------------------------------------------------

int CRegistry::GetWindowPlacement(const char * szSubKey, const char * szName, CWnd *pWindow)
 {
  WINDOWPLACEMENT stWP;
  HWND            hWnd = pWindow->GetSafeHwnd();
  CString         strFullSubKey = GetFullSubKey(szSubKey);

  if (hWnd == NULL) return -3;

  stWP.length = sizeof(WINDOWPLACEMENT);
  if (::GetWindowPlacement(hWnd,&stWP) == FALSE) return -4;

  return SetData(strFullSubKey,szName,&stWP,sizeof(WINDOWPLACEMENT));
 }

// --------------------------------------------------------------------------
// NetmeetingUI : NE PAS UTILISER (Ne fonctionne qu'avec Intel Proshare Video
//                Conferencing
// --------------------------------------------------------------------------

BOOL CRegistry::NetmeetingUI(BOOL ShowIt)
 {
  DWORD dwValue = (ShowIt) ? 0 : 1;
  HKEY  hPreviewKey = SetDefaultRootKey(HKEY_LOCAL_MACHINE);

  int rValue = SetValue("SOFTWARE\\Intel\\ProShare\\3.0\\PDK\\CM", "SuppressNmUI", dwValue);

  SetDefaultRootKey(hPreviewKey);

  if (rValue < 0) return FALSE;

  return TRUE;
 }

// --------------------------------------------------------------------------
// IsKeyValid : Détermine si la clé exite ou non
// --------------------------------------------------------------------------

int CRegistry::IsKeyValid(const char * szSubKey)
 {
  HKEY     hKey;
  LONG     Error;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  RegCloseKey(hKey);

  return 1;
 }

// --------------------------------------------------------------------------
// IsNameValid : Détermine si la valeur existe dans la clé spécifiée
// --------------------------------------------------------------------------

int CRegistry::IsNameValid(const char * szSubKey, const char * szName)
 {
  HKEY     hKey;
  LONG     Error;
  DWORD    dwType;
  DWORD    dwSize;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_READ, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    return -1;
   }

  Error = RegQueryValueEx(hKey, szName, NULL, &dwType, NULL, &dwSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    return -2;
   }

  RegCloseKey(hKey);

  return 1;
 }

// --------------------------------------------------------------------------
// SetValue : Permet de stocker une donnée numérique dans la valeur 'szName'
//            se trouvant dans la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::SetBolean(const char * szSubKey, const char * szName, BOOL bValue)
 {
  HKEY     hKey;
  LONG     Error;
  BYTE    *pByteData = new BYTE[sizeof(DWORD)];
  DWORD    dwValue   = 0;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  if (bValue) dwValue = 1;

  memcpy((char *)pByteData, (char *)&dwValue, sizeof(DWORD));

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    delete [] pByteData;
    return -1;
   }

  Error = RegSetValueEx(hKey, szName, NULL, REG_DWORD, pByteData, sizeof(DWORD));
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -2;
   }

  RegCloseKey(hKey);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// SetData : Permet de sauvegarder les données dans la clé 'szSubKey', dans
//           la valeur 'szName'
// 
//  szSubKey = Nom de la clé
//  szName   = Nom de la valeur où il faut stocker les données
//  pData    = Pointeur sur le buffer qui contient les données
//  Size     = Taille des données
// --------------------------------------------------------------------------
 
int CRegistry::SetData(const char * szSubKey, const char * szName, void *pData, DWORD Size)
 {
  HKEY     hKey;
  LONG     Error;
  BYTE    *pByteData;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  pByteData = new BYTE[Size];

  memcpy((BYTE *)pByteData, (BYTE *)pData, Size);

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    delete [] pByteData;
    return -1;
   }

  Error = RegSetValueEx(hKey, szName, NULL, REG_BINARY, pByteData, Size);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -2;
   }

  RegCloseKey(hKey);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// SetDefaultRootKey : Permet de définir sur qu'elle clé racine les fonctions
//                     de la classe vont travailler
// --------------------------------------------------------------------------

HKEY CRegistry::SetDefaultRootKey(HKEY RootKey)
 {
  HKEY PreviewRoot = m_RootKey;
  m_RootKey = RootKey;
  return PreviewRoot;
 }

// --------------------------------------------------------------------------
// SetString : Permet de stocker une chaine de caractère dans la valeur
//             'szName' se trouvant dans la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::SetString(const char * szSubKey, const char * szName, CString strValue)
 {
  HKEY     hKey;
  LONG     Error;
  BYTE    *pByteData;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  if (strValue.IsEmpty()) strValue = m_EmptyCar;

  pByteData = new BYTE[strValue.GetLength() + 1];

  for (int I=0; I<strValue.GetLength(); I++) pByteData[I] = strValue[I];
  pByteData[I] = 0;

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    delete [] pByteData;
    return -1;
   }

  Error = RegSetValueEx(hKey, szName, NULL, REG_SZ, pByteData, strValue.GetLength() + 1);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -2;
   }

  RegCloseKey(hKey);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// SetStringArray : Permet de stocker un tableau de chaine de caractère dans
//                  la valeur 'szName' se trouvant  dans la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::SetStringArray(const char * szSubKey, const char * szName, CStringArray &strArray)
 {
  HKEY     hKey;
  LONG     Error;
  BYTE    *pByteData;
  int      I, J, K = 0;
  DWORD    dwDataSize = 0;
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  for (I=0; I<strArray.GetSize(); I++) 
   {
    if (strArray[I].IsEmpty()) strArray[I] = m_EmptyCar;
    dwDataSize += strArray[I].GetLength() + 1;
   }
  dwDataSize += 2;

  pByteData = new BYTE[dwDataSize];

  for (I=0; I<strArray.GetSize(); I++)
   {
    for (J=0; J<strArray[I].GetLength(); J++) pByteData[K++] = strArray[I][J];
    pByteData[K++] = 0;
   }
  pByteData[K++] = 0;
  pByteData[K++] = 0;

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    delete [] pByteData;
    return -1;
   }

  Error = RegSetValueEx(hKey, szName, NULL, REG_MULTI_SZ, pByteData, dwDataSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -2;
   }

  RegCloseKey(hKey);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// SetStringAtIndex : Permet de stocker une chaine de caractère dans la
//                    valeur 'szName' se trouvant dans la clé 'szSubKey' à
//                    l'index N°<Index>
// --------------------------------------------------------------------------

int CRegistry::SetStringAtIndex(const char * szSubKey, const char * szName, CString strValue, int Index)
 {
  CStringArray strArray;
  int          rValue;
  CString      strFullSubKey = GetFullSubKey(szSubKey);

  if (strValue.IsEmpty()) strValue = m_EmptyCar;

  rValue = GetStringArray(strFullSubKey,szName,strArray);
  if (rValue < 0) return rValue;
  
  if (Index < strArray.GetSize()) strArray[Index] = strValue;
  else {
        for (int I=strArray.GetSize(); I<Index; I++) strArray.Add(m_EmptyCar);
        strArray.Add(strValue);
       }

  return SetStringArray(strFullSubKey,szName,strArray);
 }

// --------------------------------------------------------------------------
// SetValue : Permet de stocker une donnée numérique dans la valeur 'szName'
//            se trouvant dans la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::SetValue(const char * szSubKey, const char * szName, DWORD dwValue)
 {
  HKEY     hKey;
  LONG     Error;
  BYTE    *pByteData = new BYTE[sizeof(DWORD)];
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  memcpy((char *)pByteData, (char *)&dwValue, sizeof(DWORD));

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    delete [] pByteData;
    return -1;
   }

  Error = RegSetValueEx(hKey, szName, NULL, REG_DWORD, pByteData, sizeof(DWORD));
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -2;
   }

  RegCloseKey(hKey);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// SetValueArray : Permet de stocker un tableau de données numériques dans la
//                 valeur 'szName' se trouvant dans la clé 'szSubKey'
// --------------------------------------------------------------------------

int CRegistry::SetValueArray(const char * szSubKey, const char * szName, CUIntArray &dwArray)
 {
  HKEY     hKey;
  LONG     Error;
  BYTE    *pByteData;
  int      I, J;
  DWORD    dwDataSize = dwArray.GetSize() * sizeof(DWORD);
  CString  strFullSubKey = GetFullSubKey(szSubKey);

  pByteData = new BYTE[dwDataSize];

  for (I=0, J=0; I<dwArray.GetSize(); I++)
   {
    memcpy((BYTE *)(pByteData + J), (BYTE *)&dwArray[I], sizeof(DWORD));
    J += sizeof(DWORD);
   }

  Error = RegOpenKeyEx(m_RootKey, strFullSubKey, 0, KEY_ALL_ACCESS, &hKey);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    delete [] pByteData;
    return -1;
   }

  Error = RegSetValueEx(hKey, szName, NULL, REG_BINARY, pByteData, dwDataSize);
  if (Error != ERROR_SUCCESS)
   {
    ErrorString(Error);
    RegCloseKey(hKey);
    delete [] pByteData;
    return -2;
   }

  RegCloseKey(hKey);

  delete [] pByteData;

  return 1;
 }

// --------------------------------------------------------------------------
// SetValueAtIndex : Permet de stocker une donnée numérique dans la valeur
//                   'szName' se trouvant dans la clé 'szSubKey'
//                   à l'index N°<Index>
// --------------------------------------------------------------------------

int CRegistry::SetValueAtIndex(const char * szSubKey, const char * szName, DWORD dwValue, int Index)
 {
  CUIntArray dwArray;
  int        rValue;
  CString    strFullSubKey = GetFullSubKey(szSubKey);

  rValue = GetValueArray(strFullSubKey,szName,dwArray);
  if (rValue < 0) return rValue;
  
  if (Index < dwArray.GetSize()) dwArray[Index] = dwValue;
  else {
        for (int I=dwArray.GetSize(); I<Index; I++) dwArray.Add(0);
        dwArray.Add(dwValue);
       }

  return SetValueArray(strFullSubKey,szName,dwArray);
 }

// --------------------------------------------------------------------------
// SetWindowPlacement : Permet de restorer la position et la taille de la
//                      fenêtre passée en en argument
// --------------------------------------------------------------------------

int CRegistry::SetWindowPlacement(const char * szSubKey, const char * szName, CWnd *pWindow)
 {
  WINDOWPLACEMENT stWP;
  HWND            hWnd = pWindow->GetSafeHwnd();
  int             rValue;
  DWORD           dwSize = sizeof(WINDOWPLACEMENT);
  CString         strFullSubKey = GetFullSubKey(szSubKey);

  if (hWnd == NULL) return -5;

  rValue = GetData(strFullSubKey,szName,&stWP,dwSize);
  if (rValue < 0) return rValue;

  if (::SetWindowPlacement(hWnd,&stWP) == FALSE) return -6;

  return 1;
 }

// --------------------------------------------------------------------------
