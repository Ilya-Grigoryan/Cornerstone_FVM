// Dialog2Minute.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "Dialog2Minute.h"
#include "SQLDailySum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog2Minute dialog


CDialog2Minute::CDialog2Minute(int nCompanyID, CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2Minute::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog2Minute)
	m_s1 = _T("");
	m_s2 = _T("");
	m_s3 = _T("");
	m_s4 = _T("");
	m_s5 = _T("");
	m_s6 = _T("");
	m_s7 = _T("");
	m_s8 = _T("");
	m_s9 = _T("");
	m_n1 = -1;
	m_n2 = -1;
	m_n3 = -1;
	m_n4 = -1;
	m_n5 = -1;
	m_n6 = -1;
	m_n7 = -1;
	m_n8 = -1;
	m_n9 = -1;
	m_sDate = _T("");
	m_sThesis = _T("");
	m_n10 = -1;
	m_s10 = _T("");
	//}}AFX_DATA_INIT
	m_nCompanyID = nCompanyID;
}


void CDialog2Minute::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog2Minute)
	DDX_Text(pDX, IDC_EDIT_1, m_s1);
	DDX_Text(pDX, IDC_EDIT_2, m_s2);
	DDX_Text(pDX, IDC_EDIT_3, m_s3);
	DDX_Text(pDX, IDC_EDIT_4, m_s4);
	DDX_Text(pDX, IDC_EDIT_5, m_s5);
	DDX_Text(pDX, IDC_EDIT_6, m_s6);
	DDX_Text(pDX, IDC_EDIT_7, m_s7);
	DDX_Text(pDX, IDC_EDIT_8, m_s8);
	DDX_Text(pDX, IDC_EDIT_9, m_s9);
	DDX_Radio(pDX, IDC_RADIO_1_0, m_n1);
	DDX_Radio(pDX, IDC_RADIO_2_0, m_n2);
	DDX_Radio(pDX, IDC_RADIO_3_0, m_n3);
	DDX_Radio(pDX, IDC_RADIO_4_0, m_n4);
	DDX_Radio(pDX, IDC_RADIO_5_0, m_n5);
	DDX_Radio(pDX, IDC_RADIO_6_0, m_n6);
	DDX_Radio(pDX, IDC_RADIO_7_0, m_n7);
	DDX_Radio(pDX, IDC_RADIO_8_0, m_n8);
	DDX_Radio(pDX, IDC_RADIO_9_0, m_n9);
	DDX_Text(pDX, IDC_STATIC_DATE, m_sDate);
	DDX_Text(pDX, IDC_EDIT_THESIS, m_sThesis);
	DDX_Radio(pDX, IDC_RADIO_10_0, m_n10);
	DDX_Text(pDX, IDC_EDIT_10, m_s10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog2Minute, CDialog)
	//{{AFX_MSG_MAP(CDialog2Minute)
	ON_BN_CLICKED(IDC_OK, OnSave)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog2Minute message handlers

void CDialog2Minute::OnCancel() 
{
	CDialog::OnCancel();	
}

void CDialog2Minute::OnSave() 
{
	UpdateData();	

	for(int i=0; i<10; i++)
	{
		if((*m_arChecks[i]) == -1)
		{
			::MessageBox(m_hWnd, "Please answer to all questions before saving.", "Cornerstone FV Model", MB_ICONSTOP);
			return;
		}
		
		*m_arChecks[i] = 1 - (*m_arChecks[i]); // just inverse (first one is YES, but has value 0) 
	}

	// today's date (get this by default)
	COleDateTime today = COleDateTime::GetCurrentTime(); 
	
	// insert or update DataBase 
	int nRet = CSQLDailySum::Update2MinuteDrill(m_nCompanyID, today, 
					m_n1, m_s1, m_n2, m_s2, m_n3, m_s3, m_n4, m_s4, m_n5, m_s5, m_n6, m_s6, m_n7, m_s7, m_n8, m_s8, m_n9, m_s9, m_n10, m_s10, 
					m_sThesis );
	if(nRet)
		::MessageBox(m_hWnd, "The information is saved into Database.", "Cornerstone FV Model", MB_ICONINFORMATION);
	else
		::MessageBox(m_hWnd, "Unable to save information into Database.", "Cornerstone FV Model", MB_ICONSTOP);
}

BOOL CDialog2Minute::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_arChecks[0] = &m_n1;
	m_arChecks[1] = &m_n2;
	m_arChecks[2] = &m_n3;
	m_arChecks[3] = &m_n4;
	m_arChecks[4] = &m_n5;
	m_arChecks[5] = &m_n6;
	m_arChecks[6] = &m_n7;
	m_arChecks[7] = &m_n8;
	m_arChecks[8] = &m_n9;
	m_arChecks[9] = &m_n10;

	S2MinuteDrill data;
	BOOL b = CSQLDailySum::Get2MinuteDrill(m_nCompanyID, data);
	if(!b)
		return true;

	
	m_n1 = data.nNormalized;
	m_s1 = data.sNormalizedNotes;
	m_n2 = data.nRazor;
	m_s2 = data.sRazorNotes;
	m_n3 = data.nUndervalued;
	m_s3 = data.sUndervaluedNotes;
	m_n4 = data.nPredictor;
	m_s4 = data.sPredictorNotes;
	m_n5 = data.nMeanEPS;
	m_s5 = data.sMeanEPSNotes;
	m_n6 = data.n3Year;			
	m_s6 = data.s3YearNotes;
	m_n7 = data.nGrowth;		
	m_s7 = data.sGrowthNotes;
	m_n8 = data.nEstimates;
	m_s8 = data.sEstimatesNotes;
	m_n9 = data.nReturns;
	m_s9 = data.sReturnsNotes;
	m_n10 = data.nAcct;
	m_s10 = data.sAcctNotes;
	m_sThesis = data.sThesis;

	for(int i=0; i<10; i++)	
		*m_arChecks[i] = 1 - (*m_arChecks[i]); // just inverse (first one is YES, but has value 0)

	m_sDate = data.date.Format("%Y-%m-%d");

	UpdateData(false);
	return true;
}
