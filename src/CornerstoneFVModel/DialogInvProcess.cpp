// DialogInvProcess.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogInvProcess.h"

#include "DialogSelectCompany.h"
#include "SQLInvProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInvProcess dialog


CDialogInvProcess::CDialogInvProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInvProcess::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInvProcess)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogInvProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInvProcess)
	DDX_Control(pDX, IDC_LIST_REDUCE, m_listRW);
	DDX_Control(pDX, IDC_LIST_SD, m_listSD);
	DDX_Control(pDX, IDC_LIST_TRUCK, m_listTruck);
	DDX_Control(pDX, IDC_LIST_WIP, m_listWIP);
	DDX_Control(pDX, IDC_LIST_PORT, m_listPort);
	DDX_Control(pDX, IDC_LIST_ADDWEIGHT, m_listAW);
	DDX_Control(pDX, IDC_LIST_QC, m_listQC);
	DDX_Control(pDX, IDC_LIST_RAW, m_listRaw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInvProcess, CDialog)
	//{{AFX_MSG_MAP(CDialogInvProcess)
	ON_BN_CLICKED(IDC_BUTTON_RAW_ADD, OnButtonRawAdd)
	ON_BN_CLICKED(IDC_BUTTON_RAW_DEL, OnButtonRawDel)
	ON_BN_CLICKED(IDC_BUTTON_WIP_ADD, OnButtonWipAdd)
	ON_BN_CLICKED(IDC_BUTTON_WIP_DEL, OnButtonWipDel)
	ON_BN_CLICKED(IDC_BUTTON_QC_ADD, OnButtonQcAdd)
	ON_BN_CLICKED(IDC_BUTTON_QC_DEL, OnButtonQcDel)
	ON_BN_CLICKED(IDC_BUTTON_PORT_ADD, OnButtonPortAdd)
	ON_BN_CLICKED(IDC_BUTTON_PORT_DEL, OnButtonPortDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInvProcess message handlers

BOOL CDialogInvProcess::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CArray<int,int>	arIds;
	CStringArray	arTickers;

	// Raw Materials
	CSQLInvProcess::Get(1, arIds, arTickers);
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nInd = m_listRaw.AddString(arTickers[i]);
		m_listRaw.SetItemData(nInd, arIds[i]);
	}

	// WIP
	arIds.RemoveAll();
	arTickers.RemoveAll();
	CSQLInvProcess::Get(2, arIds, arTickers);
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nInd = m_listWIP.AddString(arTickers[i]);
		m_listWIP.SetItemData(nInd, arIds[i]);
	}

	// QC
	arIds.RemoveAll();
	arTickers.RemoveAll();
	CSQLInvProcess::Get(3, arIds, arTickers);
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nInd = m_listQC.AddString(arTickers[i]);
		m_listQC.SetItemData(nInd, arIds[i]);
	}

	// Portfolio
	arIds.RemoveAll();
	arTickers.RemoveAll();
	CSQLInvProcess::Get(4, arIds, arTickers);
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nInd = m_listPort.AddString(arTickers[i]);
		m_listPort.SetItemData(nInd, arIds[i]);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogInvProcess::OnButtonRawAdd() 
{
	CDialogSelectCompany dlg;
	if(dlg.DoModal() != IDOK)
		return;

	int nStage = 1;
	int nId = dlg.m_nId;
	CString sTicker = dlg.m_sTicker;

	int nRet = CSQLInvProcess::Add(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to add. Error with Database connection. Please try again.");
		return;
	}

	int nIndex = m_listRaw.AddString(sTicker);
	m_listRaw.SetItemData(nIndex, nId);
}

void CDialogInvProcess::OnButtonRawDel() 
{
	int nSel = m_listRaw.GetCurSel();
	if(nSel < 0)
	{
		AfxMessageBox("No company is selected. Please select an item and try again.");
		return;
	}

	int n = AfxMessageBox("Are you sure you want to delete the selected item?", MB_YESNO|MB_ICONQUESTION);
	if(n != 6)
		return;

	int nStage = 1;
	int nId = m_listRaw.GetItemData(nSel);

	int nRet = CSQLInvProcess::Delete(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to delete. Error with Database connection. Please try again.");
		return;
	}

	m_listRaw.DeleteString(nSel);
}

void CDialogInvProcess::OnButtonWipAdd() 
{
	CDialogSelectCompany dlg;
	if(dlg.DoModal() != IDOK)
		return;

	int nStage = 2;
	int nId = dlg.m_nId;
	CString sTicker = dlg.m_sTicker;

	int nRet = CSQLInvProcess::Add(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to add. Error with Database connection. Please try again.");
		return;
	}

	int nIndex = m_listWIP.AddString(sTicker);
	m_listWIP.SetItemData(nIndex, nId);
}

void CDialogInvProcess::OnButtonWipDel() 
{
	int nSel = m_listWIP.GetCurSel();
	if(nSel < 0)
	{
		AfxMessageBox("No company is selected. Please select an item and try again.");
		return;
	}

	int n = AfxMessageBox("Are you sure you want to delete the selected item?", MB_YESNO|MB_ICONQUESTION);
	if(n != 6)
		return;

	int nStage = 2;
	int nId = m_listWIP.GetItemData(nSel);

	int nRet = CSQLInvProcess::Delete(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to delete. Error with Database connection. Please try again.");
		return;
	}

	m_listWIP.DeleteString(nSel);	
}

void CDialogInvProcess::OnButtonQcAdd() 
{
	CDialogSelectCompany dlg;
	if(dlg.DoModal() != IDOK)
		return;

	int nStage = 3;
	int nId = dlg.m_nId;
	CString sTicker = dlg.m_sTicker;

	int nRet = CSQLInvProcess::Add(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to add. Error with Database connection. Please try again.");
		return;
	}

	int nIndex = m_listQC.AddString(sTicker);
	m_listQC.SetItemData(nIndex, nId);
}

void CDialogInvProcess::OnButtonQcDel() 
{
	int nSel = m_listQC.GetCurSel();
	if(nSel < 0)
	{
		AfxMessageBox("No company is selected. Please select an item and try again.");
		return;
	}

	int n = AfxMessageBox("Are you sure you want to delete the selected item?", MB_YESNO|MB_ICONQUESTION);
	if(n != 6)
		return;

	int nStage = 3;
	int nId = m_listQC.GetItemData(nSel);

	int nRet = CSQLInvProcess::Delete(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to delete. Error with Database connection. Please try again.");
		return;
	}

	m_listQC.DeleteString(nSel);
}

void CDialogInvProcess::OnButtonPortAdd() 
{
	CDialogSelectCompany dlg;
	if(dlg.DoModal() != IDOK)
		return;

	int nStage = 4;
	int nId = dlg.m_nId;
	CString sTicker = dlg.m_sTicker;

	int nRet = CSQLInvProcess::Add(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to add. Error with Database connection. Please try again.");
		return;
	}

	int nIndex = m_listPort.AddString(sTicker);
	m_listPort.SetItemData(nIndex, nId);
}

void CDialogInvProcess::OnButtonPortDel() 
{
	int nSel = m_listPort.GetCurSel();
	if(nSel < 0)
	{
		AfxMessageBox("No company is selected. Please select an item and try again.");
		return;
	}

	int n = AfxMessageBox("Are you sure you want to delete the selected item?", MB_YESNO|MB_ICONQUESTION);
	if(n != 6)
		return;

	int nStage = 4;
	int nId = m_listPort.GetItemData(nSel);

	int nRet = CSQLInvProcess::Delete(nId, nStage);	
	if(!nRet)
	{
		AfxMessageBox("Unable to delete. Error with Database connection. Please try again.");
		return;
	}

	m_listPort.DeleteString(nSel);
}
