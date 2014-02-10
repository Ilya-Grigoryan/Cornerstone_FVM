; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=3
Class1=CCip_coreApp
LastClass=CDialogProgressEx
NewFileInclude2=#include "cip_core.h"
ResourceCount=1
NewFileInclude1=#include "stdafx.h"
Class2=CViewHTML
LastTemplate=CHtmlView
Class3=CDialogProgressEx
Resource1=CG_IDD_PROGRESS

[CLS:CCip_coreApp]
Type=0
HeaderFile=cip_core.h
ImplementationFile=cip_core.cpp
Filter=N

[CLS:CViewHTML]
Type=0
HeaderFile=ViewHTML.h
ImplementationFile=ViewHTML.cpp
BaseClass=CHtmlView
Filter=C

[DLG:CG_IDD_PROGRESS]
Type=1
Class=CDialogProgressEx
ControlCount=4
Control1=IDCANCEL,button,1342242817
Control2=CG_IDC_PROGDLG_PROGRESS,msctls_progress32,1350565888
Control3=CG_IDC_PROGDLG_PERCENT,static,1342308352
Control4=CG_IDC_PROGDLG_STATUS,static,1342308352

[CLS:CDialogProgressEx]
Type=0
HeaderFile=DialogProgressEx.h
ImplementationFile=DialogProgressEx.cpp
BaseClass=CDialog

