// DialogSectors.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogSectors.h"
#include "DialogName.h"

#include "SQLCompany.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSectors dialog


CDialogSectors::CDialogSectors(int bIndustryOrSector, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSectors::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSectors)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIndustryOrSector = bIndustryOrSector;
}


void CDialogSectors::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSectors)
	DDX_Control(pDX, IDC_LIST_MAIN, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSectors, CDialog)
	//{{AFX_MSG_MAP(CDialogSectors)
	ON_BN_CLICKED(ID_ADD, OnAdd)
	ON_BN_CLICKED(ID_EDIT, OnEdit)
	ON_BN_CLICKED(ID_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSectors message handlers

void CDialogSectors::OnAdd() 
{
	CDialogName dlg;
	if(dlg.DoModal() != IDOK)
		return;

	if(m_bIndustryOrSector == 0)
		CSQLCompany::AddIndustry(dlg.m_sName, "");
	else
		CSQLCompany::AddSector(dlg.m_sName, "");
	Load();
}

void CDialogSectors::OnEdit() 
{
	int nSel = m_List.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("Please select an Item.");
		return;
	}
	
	CString sName; 
	m_List.GetText(nSel, sName);
	int nID = m_List.GetItemData(nSel);

	CDialogName dlg;
	dlg.m_sName = sName;
	if(dlg.DoModal() != IDOK)
		return;

	sName = dlg.m_sName;

	if(m_bIndustryOrSector == 0)
		CSQLCompany::SetIndustryName(nID, sName);
	else
		CSQLCompany::SetSectorName(nID, sName);
	Load();
}

void CDialogSectors::OnDelete() 
{
	int nSel = m_List.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("Please select an Item.");
		return;
	}

	int nRet;
	if(m_bIndustryOrSector == 0)
		nRet = AfxMessageBox("Are you sure you want to delete the Sector?  You cannot undo this operation.", MB_ICONQUESTION|MB_YESNO);
	else
		nRet = AfxMessageBox("Are you sure you want to delete the Industry?  You cannot undo this operation.", MB_ICONQUESTION|MB_YESNO);
	if(nRet != IDYES)
		return;

	int nID = m_List.GetItemData(nSel);

	if(m_bIndustryOrSector == 0)
		CSQLCompany::DeleteIndustry(nID);
	else
		CSQLCompany::DeleteSector(nID);
	Load();
}

BOOL CDialogSectors::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	strcpy(lf.lfFaceName, "Arial");
	lf.lfHeight = 14;
	m_Font.CreateFontIndirect(&lf);

	m_List.SetFont(&m_Font);

	if(m_bIndustryOrSector == 0)
		SetWindowText("Sectors");
	else
		SetWindowText("Industries");

	Load();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSectors::Load()
{
	m_List.ResetContent();

	CArray<int,int> arIds;
	CStringArray arNames;

	int nCount;
	if(m_bIndustryOrSector == 0)
		nCount = CSQLCompany::GetIndustryIdsAndNames(arIds, arNames);
	else
		nCount = CSQLCompany::GetSectorIdsAndNames(arIds, arNames);

	for(int i=0; i<nCount; i++)
	{
		int nIndex = m_List.AddString(arNames[i]);
		m_List.SetItemData(nIndex, arIds[i]);
	}

}
