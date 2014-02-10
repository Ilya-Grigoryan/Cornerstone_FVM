// DialogFilterMeetings.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogFilterMeetings.h"

#include "Universe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogFilterMeetings dialog


CDialogFilterMeetings::CDialogFilterMeetings(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFilterMeetings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFilterMeetings)
	m_nSort = 0;
	//}}AFX_DATA_INIT
	m_nCompanyID = -1;
	m_From.SetStatus(COleDateTime::invalid);
	m_To.SetStatus(COleDateTime::invalid);
	m_nRec = -1;
	m_nDec = -1;
}


void CDialogFilterMeetings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFilterMeetings)
	DDX_Control(pDX, IDC_EDIT_TO_Y, m_editToY);
	DDX_Control(pDX, IDC_EDIT_TO_M, m_editToM);
	DDX_Control(pDX, IDC_EDIT_TO_D, m_editToD);
	DDX_Control(pDX, IDC_EDIT_FROM_Y, m_editFromY);
	DDX_Control(pDX, IDC_EDIT_FROM_M, m_editFromM);
	DDX_Control(pDX, IDC_EDIT_FROM_D, m_editFromD);
	DDX_Control(pDX, IDC_CHECK_TO, m_checkTo);
	DDX_Control(pDX, IDC_CHECK_FROM, m_checkFrom);
	DDX_Control(pDX, IDC_COMBO_REC, m_comboRec);
	DDX_Control(pDX, IDC_COMBO_DEC, m_comboDec);
	DDX_Control(pDX, IDC_COMBO_COMPANY, m_comboCompany);
	DDX_Radio(pDX, IDC_RADIO_SORT, m_nSort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFilterMeetings, CDialog)
	//{{AFX_MSG_MAP(CDialogFilterMeetings)
	ON_BN_CLICKED(IDC_CHECK_FROM, OnCheckFrom)
	ON_BN_CLICKED(IDC_CHECK_TO, OnCheckTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFilterMeetings message handlers

BOOL CDialogFilterMeetings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/////////////////////////////////////////////////////
	if(m_From.GetStatus() == COleDateTime::valid)
	{
		m_checkFrom.SetCheck(1);
		CString s; 
		
		s.Format("%u", m_From.GetYear());
		m_editFromY.SetWindowText(s);

		s.Format("%02u", m_From.GetMonth());
		m_editFromM.SetWindowText(s);

		s.Format("%02u", m_From.GetDay());
		m_editFromD.SetWindowText(s);
	}
	else
	{
		m_checkFrom.SetCheck(0);
		m_editFromY.EnableWindow(false);
		m_editFromM.EnableWindow(false);
		m_editFromD.EnableWindow(false);
	}

	////////////////////////////////////////////////////
	if(m_To.GetStatus() == COleDateTime::valid)
	{
		m_checkTo.SetCheck(1);
		CString s; 
		
		s.Format("%u", m_To.GetYear());
		m_editToY.SetWindowText(s);

		s.Format("%02u", m_To.GetMonth());
		m_editToM.SetWindowText(s);

		s.Format("%02u", m_To.GetDay());
		m_editToD.SetWindowText(s);
	}
	else
	{
		m_checkTo.SetCheck(0);
		m_editToY.EnableWindow(false);
		m_editToM.EnableWindow(false);
		m_editToD.EnableWindow(false);
	}

	////////////////////////////////////////////////////
	// Fill up the companies combo
	int nIndex = m_comboCompany.AddString("---  All  ---");
	m_comboCompany.SetItemData(nIndex, -1);

	nIndex = m_comboCompany.AddString("- New Idea -");
	m_comboCompany.SetItemData(nIndex, 0);

	CArray<int,int> arIds;
	CStringArray arTickers;
	CSQLCompany::GetCompanyIDAndTickers(arIds, arTickers, theApp.m_Universe.GetDate());
	for(int i=0; i<arIds.GetSize(); i++)
	{
		int nID = arIds[i];
		CString sTicker = arTickers[i];

		nIndex = m_comboCompany.AddString(sTicker);
		m_comboCompany.SetItemData(nIndex, nID);
	}

	// select
	for(int i=0; i<m_comboCompany.GetCount(); i++)
		if(m_nCompanyID == (int)m_comboCompany.GetItemData(i))
		{
			m_comboCompany.SetCurSel(i);
			break;
		}

	///////////////////////////////////////////////////////////
	for(int i=0; i<m_comboRec.GetCount(); i++)
		if(m_nRec+1 == i)
		{
			m_comboRec.SetCurSel(i);
			break;
		}

	///////////////////////////////////////////////////////////
	for(int i=0; i<m_comboDec.GetCount(); i++)
		if(m_nDec+1 == i)
		{
			m_comboDec.SetCurSel(i);
			break;
		}

	return TRUE;
}

void CDialogFilterMeetings::OnCheckFrom() 
{
	int nCheck = m_checkFrom.GetCheck();
	m_editFromY.EnableWindow(nCheck);
	m_editFromM.EnableWindow(nCheck);
	m_editFromD.EnableWindow(nCheck);
}

void CDialogFilterMeetings::OnCheckTo() 
{
	int nCheck = m_checkTo.GetCheck();
	m_editToY.EnableWindow(nCheck);
	m_editToM.EnableWindow(nCheck);
	m_editToD.EnableWindow(nCheck);
}

void CDialogFilterMeetings::OnOK() 
{
	if(m_checkFrom.GetCheck())
	{
		CString sY; m_editFromY.GetWindowText(sY);
		CString sM; m_editFromM.GetWindowText(sM);
		CString sD; m_editFromD.GetWindowText(sD);
		int nY=atoi(sY);
		int nM=atoi(sM);
		int nD=atoi(sD);
		m_From = COleDateTime(nY, nM, nD, 0,0,0);
		if(m_From.GetStatus() != COleDateTime::valid)
		{
			AfxMessageBox("Please enter a correct Date.");
			return;
		}
	}
	else
		m_From.SetStatus(COleDateTime::invalid);

	if(m_checkTo.GetCheck())
	{
		CString sY; m_editToY.GetWindowText(sY);
		CString sM; m_editToM.GetWindowText(sM);
		CString sD; m_editToD.GetWindowText(sD);
		int nY=atoi(sY);
		int nM=atoi(sM);
		int nD=atoi(sD);
		m_To = COleDateTime(nY, nM, nD, 0,0,0);
		if(m_To.GetStatus() != COleDateTime::valid)
		{
			AfxMessageBox("Please enter a correct Date.");
			return;
		}
	}
	else
		m_To.SetStatus(COleDateTime::invalid);

	int nSel = m_comboCompany.GetCurSel();
	m_nCompanyID = m_comboCompany.GetItemData(nSel);

	m_nRec = m_comboRec.GetCurSel()-1;
	m_nDec = m_comboDec.GetCurSel()-1;

	CDialog::OnOK();
}
