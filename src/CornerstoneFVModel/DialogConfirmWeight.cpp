// DialogConfirmWeight.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogConfirmWeight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogConfirmWeight dialog


CDialogConfirmWeight::CDialogConfirmWeight(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConfirmWeight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConfirmWeight)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogConfirmWeight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConfirmWeight)
	DDX_Control(pDX, IDC_COMBO_BIAS, m_comboBias);
	DDX_Control(pDX, IDC_COMBO_TARGET_WEIGHT, m_comboTarget);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogConfirmWeight, CDialog)
	//{{AFX_MSG_MAP(CDialogConfirmWeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConfirmWeight message handlers

void CDialogConfirmWeight::OnOK() 
{
	// Target weight
	CString sWeight;
	m_comboTarget.GetWindowText(sWeight);
	
	// check for special cases (WIP, QC, Shipping Dock, Truck, Pass)
	if(sWeight == "WIP")				m_fWeight = -101; else
	if(sWeight == "QC")					m_fWeight = -102; else
	if(sWeight == "Shipping Dock")		m_fWeight = -103; else
	if(sWeight == "Truck")				m_fWeight = -104; else
	if(sWeight == "Pass")				m_fWeight = -105; 
	else
		m_fWeight = atof(sWeight);

	// Bias
	int nSel = m_comboBias.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the 'Bias' from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	m_comboBias.GetLBText(nSel, m_sBias);

	CDialog::OnOK();
}

BOOL CDialogConfirmWeight::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Weight
	if(m_fWeight <= -100)
		m_comboTarget.SetCurSel(-(100+(int)m_fWeight));
	else
	{
		CString sWeight;
		sWeight.Format("%g", m_fWeight);
		m_comboTarget.SetWindowText(sWeight);
	}

	// Bias
	for(int i=0; i<m_comboBias.GetCount(); i++)
	{
		CString s;
		m_comboBias.GetLBText(i, s);
		if(s == m_sBias)
		{
			m_comboBias.SetCurSel(i);
			break;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
