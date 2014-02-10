// DialogNewMeeting.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogNewMeeting.h"

#include "SQLCompany.h"
#include "Universe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogNewMeeting dialog


CDialogNewMeeting::CDialogNewMeeting(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNewMeeting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNewMeeting)
	//}}AFX_DATA_INIT
}


void CDialogNewMeeting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNewMeeting)
	DDX_Control(pDX, IDC_LIST_FILES, m_ListFiles);
	DDX_Control(pDX, IDC_COMBO_REC, m_comboRec);
	DDX_Control(pDX, IDC_COMBO_DEC, m_comboDec);
	DDX_Control(pDX, IDC_EDIT_DESC, m_editDesc);
	DDX_Control(pDX, IDC_EDIT_ANALYST, m_editAnalyst);
	DDX_Control(pDX, IDC_EDIT_DATE, m_editDate);
	DDX_Control(pDX, IDC_COMBO_COMPANY, m_comboCompany);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNewMeeting, CDialog)
	//{{AFX_MSG_MAP(CDialogNewMeeting)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FILE, OnButtonAddFile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_FILE, OnButtonDeleteFile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALLFILES, OnButtonDeleteAllfiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNewMeeting message handlers

BOOL CDialogNewMeeting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nIndex; 

	// Fill up the companies combo
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

	// Set the date to todays date
	COleDateTime now = COleDateTime::GetCurrentTime();
	m_editDate.SetWindowText(now.Format("%m/%d/%Y"));
		
	return TRUE;  
}

void CDialogNewMeeting::OnOK() 
{
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("Please select a company, or specify 'New Idea'");
		return;
	}
	m_nCompanyID = m_comboCompany.GetItemData(nSel);

	CString sDate;
	m_editDate.GetWindowText(sDate);
	m_Date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(m_Date.GetStatus() != COleDateTime::valid)
	{
		AfxMessageBox("Please specify a valid date.");
		return;
	}

	m_editAnalyst.GetWindowText(m_sAnalyst);
	if(m_sAnalyst.IsEmpty())
	{
		AfxMessageBox("Please enter Analyst Name");
		return;
	}

	m_nRecommend = m_comboRec.GetCurSel();
	if(m_nRecommend == -1)
	{
		AfxMessageBox("Please select a 'Recommendation'.");
		return;
	}

	m_nDecision = m_comboDec.GetCurSel();
	if(m_nDecision == -1)
	{
		AfxMessageBox("Please select a 'Decision'.");
		return;
	}

	m_editDesc.GetWindowText(m_sDesc);
	if(m_sDesc.IsEmpty())
	{
		AfxMessageBox("Please enter a Description.");
		return;
	}

	for(int i=0; i<m_ListFiles.GetCount(); i++)
	{
		CString s;
		m_ListFiles.GetText(i, s);
		m_arFiles.Add(s);
	}

	if(m_arFiles.GetSize() == 0)
	{
		int nRes = AfxMessageBox("No files are attached to the Meeting. Do you want to continue without files?", MB_ICONQUESTION|MB_YESNO);
		if(nRes == IDNO)
			return;
	}
	
	CDialog::OnOK();
}

void CDialogNewMeeting::OnButtonAddFile() 
{
	static char szFilters[] ="All Files (*.*)|*.*|PDF Files (*.pdf)|*.pdf|Word Files (*.doc)|*.doc|Excel Files (*.xls)|*.xls||";

	CFileDialog fileDlg (true, "", "", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	m_ListFiles.AddString(fileDlg.GetPathName());
}

void CDialogNewMeeting::OnButtonDeleteFile() 
{
	int nSel = m_ListFiles.GetCurSel();
	if(nSel == -1)
	{
		AfxMessageBox("No Selection.");
		return;
	}

	m_ListFiles.DeleteString(nSel);
}

void CDialogNewMeeting::OnButtonDeleteAllfiles() 
{
	m_ListFiles.ResetContent();
}
