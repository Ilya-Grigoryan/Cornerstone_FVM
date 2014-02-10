// DialogCompanies.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogCompanies.h"
#include "DialogNameAndTicker.h"

#include "Universe.h"
#include "DialogDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogCompanies dialog


CDialogCompanies::CDialogCompanies(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCompanies::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogCompanies)
	//}}AFX_DATA_INIT
	m_bAdding = false;
	m_nIndexAdding = -1;
}


void CDialogCompanies::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCompanies)
	DDX_Control(pDX, IDC_CHECK_LOAD_ROCROE, m_checkLoadROCROE);
	DDX_Control(pDX, IDC_EDIT_NAME_LOOKUP, m_editLookup);
	DDX_Control(pDX, IDC_EDIT_LAST_REPORT_YEAR, m_editLastYear);
	DDX_Control(pDX, IDC_EDIT_TICKER, m_editTicker);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_CIQTICKER, m_editCIQTicker);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_SECTOR, m_comboSector);
	DDX_Control(pDX, IDC_COMBO_INDUSTRY, m_comboIndustry);
	DDX_Control(pDX, IDC_CHECK_USE_DEBT_ADJUST, m_checkDebtAdjust);
	DDX_Control(pDX, IDC_CHECK_LOAD_EPS, m_checkLoadEPS);
	DDX_Control(pDX, IDC_CHECK_LOAD_BS, m_checkLoadBS);
	DDX_Control(pDX, IDC_LIST_MAIN, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCompanies, CDialog)
	//{{AFX_MSG_MAP(CDialogCompanies)
	ON_LBN_SELCHANGE(IDC_LIST_MAIN, OnSelchangeListMain)
	ON_BN_CLICKED(ID_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(ID_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(ID_BUTTON_SAVE, OnButtonSave)
	ON_EN_CHANGE(IDC_EDIT_NAME_LOOKUP, OnChangeEditNameLookup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCompanies message handlers

BOOL CDialogCompanies::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	strcpy(lf.lfFaceName, "Arial");
	lf.lfHeight = 14;
	m_Font.CreateFontIndirect(&lf);

	//m_editTicker.EnableWindow(false);
	
	// Get Companies
	for(int i=0; i<theApp.m_Universe.GetSize(); i++)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompany(i);

		CString sItem;
		sItem.Format("%s - %s", pCompany->GetTicker(), pCompany->GetName());
		int nIndex = m_List.AddString(sItem);
		m_List.SetItemData(nIndex, (DWORD)pCompany);
	}

	CArray<int,int> arIds;
	CStringArray arNames;
	int nCount;

	// Get Industries (really Sectors from DB)
	nCount = CSQLCompany::GetSectorIdsAndNames(arIds, arNames);
	for(int i=0; i<nCount; i++)
	{
		int nIndex = m_comboIndustry.AddString(arNames[i]);
		m_comboIndustry.SetItemData(nIndex, arIds[i]);
	}

	// Get Sectors (really Industries from DB)
	arIds.RemoveAll(); arNames.RemoveAll();
	nCount = CSQLCompany::GetIndustryIdsAndNames(arIds, arNames);
	for(int i=0; i<nCount; i++)
	{
		int nIndex = m_comboSector.AddString(arNames[i]);
		m_comboSector.SetItemData(nIndex, arIds[i]);
	}

	m_arEditWeights[0] = GetDlgItem(IDC_EDIT_W1);
	m_arEditWeights[1] = GetDlgItem(IDC_EDIT_W2);
	m_arEditWeights[2] = GetDlgItem(IDC_EDIT_W3);
	m_arEditWeights[3] = GetDlgItem(IDC_EDIT_W4);
	m_arEditWeights[4] = GetDlgItem(IDC_EDIT_W5);
	m_arEditWeights[5] = GetDlgItem(IDC_EDIT_W6);
	m_arEditWeights[6] = GetDlgItem(IDC_EDIT_W7);
	m_arEditWeights[7] = GetDlgItem(IDC_EDIT_W8);
	m_arEditWeights[8] = GetDlgItem(IDC_EDIT_W9);
	
	m_List.SetCurSel(0);
	SetValues(0);
	return TRUE;
}

void CDialogCompanies::OnSelchangeListMain() 
{
	int nIndex = m_List.GetCurSel();

	if(m_bAdding && m_nIndexAdding != -1)
	{
		int nRet = AfxMessageBox("You haven't saved the newly added Company. Press 'Yes' to go back and save it. Press 'No' to discard it.", MB_ICONQUESTION|MB_YESNO);
		if(nRet == IDYES)
		{
			m_List.SetCurSel(m_nIndexAdding);
			return;
		} 
		else if(nRet == IDNO)
		{
			CCompany* pCompany = (CCompany*)m_List.GetItemData(m_nIndexAdding);
			delete pCompany;
			m_List.DeleteString(m_nIndexAdding);
			m_bAdding = false;
			m_nIndexAdding = -1;
			GetDlgItem(ID_BUTTON_NEW)->EnableWindow(true);
			GetDlgItem(ID_BUTTON_DELETE)->EnableWindow(true);
			GetDlgItem(ID_BUTTON_SAVE)->SetWindowText("Save");
		}	
	}

	SetValues(nIndex);
}

void CDialogCompanies::SetValues(int nIndex)
{
	if(nIndex == -1)
		return;

	CCompany* pCompany = (CCompany*)m_List.GetItemData(nIndex);

	m_editTicker.SetWindowText(pCompany->GetTicker());
	m_editName.SetWindowText(pCompany->GetName());

	// industry
	m_comboIndustry.SetCurSel(-1);
	CString sIndustry = pCompany->GetIndustry();
	for(int i=0; i<m_comboIndustry.GetCount(); i++)
	{
		CString s;
		m_comboIndustry.GetLBText(i, s);
		if(s==sIndustry)
		{
			m_comboIndustry.SetCurSel(i);
			break;
		}
	}

	// sector
	m_comboSector.SetCurSel(-1);
	CString sSector = pCompany->GetSector();
	for(int i=0; i<m_comboSector.GetCount(); i++)
	{
		CString s;
		m_comboSector.GetLBText(i, s);
		if(s==sSector)
		{
			m_comboSector.SetCurSel(i);
			break;
		}
	}

	m_editCIQTicker.SetWindowText(pCompany->GetCIQTicker());
	m_comboType.SetCurSel(pCompany->GetType());

	CString sYear; sYear.Format("%u", pCompany->GetYear());
	m_editLastYear.SetWindowText(sYear);

	m_checkLoadBS.SetCheck(pCompany->GetLoadBS());
	m_checkLoadEPS.SetCheck(pCompany->GetLoadEPS());
	m_checkLoadROCROE.SetCheck(pCompany->GetLoadROCROE());
	m_checkDebtAdjust.SetCheck(pCompany->GetUseDebtAdjust());

	for(int i=0; i<9; i++)
	{
		CString sWeight;
		sWeight.Format("%.2f", pCompany->GetWeightInIndex(i)*100.0);
		m_arEditWeights[i]->SetWindowText(sWeight);
	}
}

void CDialogCompanies::OnButtonNew() 
{
	CDialogNameAndTicker dlg;
	if(dlg.DoModal() != IDOK)
		return;

	CCompany* pCompany = new CCompany();
	pCompany->SetTicker(dlg.m_sTicker);
	pCompany->SetName(dlg.m_sName);

	COleDateTime now = COleDateTime::GetCurrentTime();
	pCompany->SetYear(now.GetYear());

	CString sItem;
	sItem.Format("%s - %s", pCompany->GetTicker(), pCompany->GetName());
	m_nIndexAdding = m_List.AddString(sItem);
	m_List.SetItemData(m_nIndexAdding, (DWORD)pCompany);
	m_List.SetCurSel(m_nIndexAdding);
	SetValues(m_nIndexAdding);

	m_bAdding = true;
	
	GetDlgItem(ID_BUTTON_NEW)->EnableWindow(false);
	GetDlgItem(ID_BUTTON_DELETE)->EnableWindow(false);
	GetDlgItem(ID_BUTTON_SAVE)->SetWindowText("Add");
}

void CDialogCompanies::OnButtonDelete() 
{
	int nSel = m_List.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Company is selected.");
		return;
	}
	CCompany* pCompany = (CCompany*)m_List.GetItemData(nSel);
	CString sTicker = pCompany->GetTicker();

	CString sMessage;
	sMessage.Format("Are you sure you want to delete the company '%s'?  You will not be able to undo this operation.", sTicker);
	int nResult = AfxMessageBox(sMessage, MB_ICONQUESTION|MB_YESNO);
	if(nResult != IDYES)
		return;

	CDialogDate dlg;
	dlg.m_Date = COleDateTime::GetCurrentTime();
	if(dlg.DoModal() != IDOK)
		return;

	BOOL bRet = CSQLCompany::DeleteCompany(sTicker, dlg.m_Date);
	if(!bRet)
		AfxMessageBox("Unable to delete the company from the Data-Base");

	m_List.DeleteString(nSel);
	theApp.m_Universe.RemoveCompany(pCompany);
	delete pCompany;
	AfxMessageBox("The company is deleted from the Data-Base");
}

void CDialogCompanies::OnButtonSave() 
{
	int nSel = m_List.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Company is selected.");
		return;
	}
	CCompany* pCompany = (CCompany*)m_List.GetItemData(nSel);
	CString sTicker = pCompany->GetTicker();

	CString sNewTicker;
	m_editTicker.GetWindowText(sNewTicker);	
	if(sNewTicker.IsEmpty())
	{
		AfxMessageBox("Ticker cannot be empty.");
		return;
	}

	CString sName;
	m_editName.GetWindowText(sName);
	if(sName.IsEmpty())
	{
		AfxMessageBox("Name cannot be empty.");
		return;
	}

	nSel = m_comboIndustry.GetCurSel();  
	if(nSel == -1)
	{
		AfxMessageBox("Please specify the Industry.");
		return;
	}
	int nIndustryID = m_comboIndustry.GetItemData(nSel);

	nSel = m_comboSector.GetCurSel();  
	if(nSel == -1)
	{
		AfxMessageBox("Please specify the Sector.");
		return;
	}
	int nSectorID = m_comboSector.GetItemData(nSel);

	CString sCIQTicker;
	m_editCIQTicker.GetWindowText(sCIQTicker);	
	if(sCIQTicker.IsEmpty())
	{
		AfxMessageBox("Please specify CapitalIQ Ticker.");
		return;
	}

	int nType = m_comboType.GetCurSel();  
	if(nType == -1)
	{
		AfxMessageBox("Please specify the Company Type.");
		return;
	}

	CString sYear;
	m_editLastYear.GetWindowText(sYear);
	int nYear = atoi(sYear) - 1;
	if(nYear < 1949 || nYear > 2049)
	{
		AfxMessageBox("Please specify Current Fiscal Year between 1950 and 2050.");
		return;
	}

	BOOL bLoadBS = m_checkLoadBS.GetCheck();
	BOOL bLoadEPS = m_checkLoadEPS.GetCheck();
	BOOL bLoadROCROE = m_checkLoadROCROE.GetCheck();
	BOOL bUseDebt = m_checkDebtAdjust.GetCheck();

	double weights[9];
	for(int i=0; i<9; i++)
	{
		CString s; m_arEditWeights[i]->GetWindowText(s);
		double fW = atof(s) / 100.0;
		weights[i] = fW;
	}

	if(!m_bAdding)
	{
		BOOL bResult = CSQLCompany::UpdateCompanyBasicInfo(sTicker, sNewTicker, sName, 
														nSectorID, nIndustryID, // switched
														sCIQTicker, nType, nYear, bLoadBS, bLoadEPS, bLoadROCROE, bUseDebt, weights);

		if(!bResult)
		{
			AfxMessageBox("Unable to save the information into the Data-Base. Plase try again.");
			return;
		}
		AfxMessageBox("The Company information is successfully saved into the Data-Base.");
	}
	else
	{
		COleDateTime now = COleDateTime::GetCurrentTime();
		BOOL bResult = CSQLCompany::AddCompany(sNewTicker, sName, 
												nSectorID, nIndustryID, // switched
												sCIQTicker, nType, nYear, bLoadBS, bLoadEPS, bLoadROCROE, bUseDebt, weights, now);

		if(!bResult)
		{
			AfxMessageBox("Unable to add the company into the Data-Base. Plase check the Ticker; it must be unique.");
			return;
		}
		AfxMessageBox("The new Company is successfully added into the Data-Base.");
		
		GetDlgItem(ID_BUTTON_NEW)->EnableWindow(true);
		GetDlgItem(ID_BUTTON_DELETE)->EnableWindow(true);
		GetDlgItem(ID_BUTTON_SAVE)->SetWindowText("Save");
	}
	
	SCompanyRawData CompanyData;
	CSQLCompany::GetCompanyRawData(nYear, sNewTicker, pCompany->GetReportDateForPriceAndReturn(), CompanyData);
	pCompany->CreateFromRawData(CompanyData);

	if(m_bAdding)
		theApp.m_Universe.AddCompany(pCompany);

	m_bAdding = false;
	m_nIndexAdding = -1;
}

void CDialogCompanies::OnChangeEditNameLookup() 
{
	if(!m_editLookup.m_hWnd)
		return;

	CString sTicker;
	m_editLookup.GetWindowText(sTicker);

	int nSel = m_List.FindString(0, sTicker);
	if(nSel != -1)
	{
		m_List.SetCurSel(nSel);
		SetValues(nSel);
	}
}

void CDialogCompanies::OnOK() 
{	
}

void CDialogCompanies::OnCancel() 
{	
	CDialog::OnCancel();
}
