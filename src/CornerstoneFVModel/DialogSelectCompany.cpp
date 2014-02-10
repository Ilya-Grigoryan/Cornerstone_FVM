// DialogSelectCompany.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogSelectCompany.h"

#include <SQLUser.h>
#include <SQLDailySum.h>

extern CApp theApp ;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectCompany dialog


CDialogSelectCompany::CDialogSelectCompany(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSelectCompany::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSelectCompany)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nId = 0;
	m_sTicker = "";
}


void CDialogSelectCompany::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSelectCompany)
	DDX_Control(pDX, IDC_COMBO_COMPANY, m_comboCompany);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSelectCompany, CDialog)
	//{{AFX_MSG_MAP(CDialogSelectCompany)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectCompany message handlers

BOOL CDialogSelectCompany::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Load companies 
	CArray<int,int> arIds;
	CStringArray arTickers;

	m_comboCompany.AddString("- Select Ticker -");
	CSQLCompany::GetCompanyIDAndTickers(arIds, arTickers, theApp.m_Universe.GetDate());
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nID = arIds[i];
		CString sTicker = arTickers[i];

		int nIndex = m_comboCompany.AddString(sTicker);
		m_comboCompany.SetItemData(nIndex, nID);
	}
	m_comboCompany.SetCurSel(0);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSelectCompany::OnOK() 
{
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		AfxMessageBox("Please select a company from the list.");
		return;
	}

	m_nId = m_comboCompany.GetItemData(nSel);
	m_comboCompany.GetLBText(nSel, m_sTicker);

	CDialog::OnOK();
}
