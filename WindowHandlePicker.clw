; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWindowHandlePickerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WindowHandlePicker.h"

ClassCount=6
Class1=CWindowHandlePickerApp
Class2=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_WINDOWHANDLEPICKER_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Class3=CWindowHandlePickerDlg
Class4=CHexaEdit
Class5=CHandlePickerTitleTip
Resource5=IDD_OPTIONSDLG
Resource6=IDD_WINDOWHANDLEPICKER_DIALOG (English (U.S.))
Class6=COptionsDlg
Resource7=IDM_TASKBARICON (Neutral (Default))

[CLS:CWindowHandlePickerApp]
Type=0
HeaderFile=WindowHandlePicker.h
ImplementationFile=WindowHandlePicker.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CWindowHandlePickerApp

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_WINDOWHANDLEPICKER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CWindowHandlePickerDlg

[DLG:IDD_WINDOWHANDLEPICKER_DIALOG (English (U.S.))]
Type=1
Class=CWindowHandlePickerDlg
ControlCount=19
Control1=IDED_HANDLE,edit,1350631424
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342177280
Control4=IDC_STATIC,static,1342177280
Control5=IDPIC_WINDOWFINDER,static,1342177294
Control6=IDC_STATIC,static,1342177792
Control7=IDC_STATIC,static,1342177792
Control8=IDST_CAPTION,static,1350566400
Control9=IDC_STATIC,static,1342177792
Control10=IDST_CLASS,static,1350566400
Control11=IDC_STATIC,static,1342177792
Control12=IDST_STYLE,static,1350566400
Control13=IDC_STATIC,static,1342177792
Control14=IDST_EXSTYLE,static,1350566400
Control15=IDC_STATIC,static,1342177792
Control16=IDST_RECT,static,1350566400
Control17=IDC_STATIC,static,1342177792
Control18=IDST_ID,static,1350566401
Control19=IDCB_ALWAYSONTOP,button,1342242819

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342177408
Control3=IDC_STATIC,static,1342177280
Control4=IDOK,button,1342373889

[CLS:CAboutDlg]
Type=0
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAboutDlg

[CLS:CWindowHandlePickerDlg]
Type=0
HeaderFile=WindowHandlePickerDlg.h
ImplementationFile=WindowHandlePickerDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CWindowHandlePickerDlg
VirtualFilter=dWC

[CLS:CHexaEdit]
Type=0
HeaderFile=HexaEdit.h
ImplementationFile=HexaEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=CHexaEdit
VirtualFilter=WC

[CLS:CHandlePickerTitleTip]
Type=0
HeaderFile=HandlePickerTitleTip.h
ImplementationFile=HandlePickerTitleTip.cpp
BaseClass=CWnd
Filter=W
LastObject=CHandlePickerTitleTip
VirtualFilter=WC

[MNU:IDM_TASKBARICON (Neutral (Default))]
Type=1
Class=?
Command1=IDM_SHOWINTERFACE
Command2=IDM_OPTIONSDLG
Command3=IDM_ABOUTBOX
Command4=ID_APP_EXIT
CommandCount=4

[DLG:IDD_OPTIONSDLG]
Type=1
Class=COptionsDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCB_STARTTRAYICON,button,1342242819

[CLS:COptionsDlg]
Type=0
HeaderFile=OptionsDlg.h
ImplementationFile=OptionsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=COptionsDlg
VirtualFilter=dWC

