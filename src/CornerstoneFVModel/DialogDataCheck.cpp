// DialogDataCheck.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogDataCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDataCheck dialog


CDialogDataCheck::CDialogDataCheck(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDataCheck::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDataCheck)
	m_nBEV = 0;
	m_fDebt = 0.0;
	m_fEPSNext = 0.0;
	m_fEPSThis = 0.0;
	m_nNorm = 0;
	m_nPeriods = 0;
	m_fPrice = 0.0;
	m_nRazor = 0;
	m_nRadioBEV = -1;
	m_nRadioDebt = -1;
	m_nRadioEPSNext = -1;
	m_nRadioEPSThis = -1;
	m_nRadioNorm = -1;
	m_nRadioPrice = -1;
	m_nRadioRazor = -1;
	//}}AFX_DATA_INIT
}


void CDialogDataCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDataCheck)
	DDX_Control(pDX, IDC_RADIO_RAZOR_1, m_radioRazor);
	DDX_Control(pDX, IDC_RADIO_PRICE_1, m_radioPrice);
	DDX_Control(pDX, IDC_RADIO_NORMALIZED_1, m_radioNorm);
	DDX_Control(pDX, IDC_RADIO_EPSTHIS_1, m_radioEPSThis);
	DDX_Control(pDX, IDC_RADIO_EPSNEXT_1, m_radioEPSNext);
	DDX_Control(pDX, IDC_RADIO_DEBT_1, m_radioDebt);
	DDX_Control(pDX, IDC_RADIO_BEV_1, m_radioBev);
	DDX_Text(pDX, IDC_EDIT_BEV, m_nBEV);
	DDX_Text(pDX, IDC_EDIT_DEBT, m_fDebt);
	DDX_Text(pDX, IDC_EDIT_EPSNEXT, m_fEPSNext);
	DDX_Text(pDX, IDC_EDIT_EPSTHIS, m_fEPSThis);
	DDX_Text(pDX, IDC_EDIT_NORMALIZED, m_nNorm);
	DDX_Text(pDX, IDC_EDIT_PERIODS, m_nPeriods);
	DDX_Text(pDX, IDC_EDIT_PRICE, m_fPrice);
	DDX_Text(pDX, IDC_EDIT_RAZOR, m_nRazor);
	DDX_Radio(pDX, IDC_RADIO_BEV_1, m_nRadioBEV);
	DDX_Radio(pDX, IDC_RADIO_DEBT_1, m_nRadioDebt);
	DDX_Radio(pDX, IDC_RADIO_EPSNEXT_1, m_nRadioEPSNext);
	DDX_Radio(pDX, IDC_RADIO_EPSTHIS_1, m_nRadioEPSThis);
	DDX_Radio(pDX, IDC_RADIO_NORMALIZED_1, m_nRadioNorm);
	DDX_Radio(pDX, IDC_RADIO_PRICE_1, m_nRadioPrice);
	DDX_Radio(pDX, IDC_RADIO_RAZOR_1, m_nRadioRazor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDataCheck, CDialog)
	//{{AFX_MSG_MAP(CDialogDataCheck)
	ON_BN_CLICKED(IDC_BUTTON_INCLUDE_ALL, OnButtonIncludeAll)
	ON_BN_CLICKED(IDC_BUTTON_EXCLUDE_ALL, OnButtonExcludeAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDataCheck message handlers

BOOL CDialogDataCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nPeriods = m_CH.nPeriods;

	/////////////////////////////////////
	if(m_CH.nDeltaRankN == 0)
	{
		m_nRadioNorm = 0;
		m_nNorm = 0;
	} 
	else
	{
		m_nRadioNorm = 1;
		m_nNorm = m_CH.nDeltaRankN;
	}

	////////////////////////////////////
	if(m_CH.nDeltaRankR == 0)
	{
		m_nRadioRazor = 0;
		m_nRazor = 0;
	} 
	else
	{
		m_nRadioRazor = 1;
		m_nRazor = m_CH.nDeltaRankR;
	}

	/////////////////////////////////////
	if(m_CH.nDeltaBEV == 0)
	{
		m_nRadioBEV = 0;
		m_nBEV = 0;
	} 
	else
	{
		m_nRadioBEV = 1;
		m_nBEV = m_CH.nDeltaBEV;
	}

	/////////////////////////////////////
	if(m_CH.fDeltaPrice == 0)
	{
		m_nRadioPrice = 0;
		m_fPrice = 0;
	} 
	else
	{
		m_nRadioPrice = 1;
		m_fPrice = m_CH.fDeltaPrice * 100;
	}

	/////////////////////////////////////
	if(m_CH.fDeltaEPSNext == 0)
	{
		m_nRadioEPSNext = 0;
		m_fEPSNext = 0;
	} 
	else
	{
		m_nRadioEPSNext = 1;
		m_fEPSNext = m_CH.fDeltaEPSNext * 100;
	}

	/////////////////////////////////////
	if(m_CH.fDeltaEPSThis == 0)
	{
		m_nRadioEPSThis = 0;
		m_fEPSThis = 0;
	} 
	else
	{
		m_nRadioEPSThis = 1;
		m_fEPSThis = m_CH.fDeltaEPSThis * 100;
	}

	/////////////////////////////////////
	if(m_CH.fDeltaDebt == 0)
	{
		m_nRadioDebt = 0;
		m_fDebt = 0;
	} 
	else
	{
		m_nRadioDebt = 1;
		m_fDebt = m_CH.fDeltaDebt * 100;
	}
	
	UpdateData(false);
	return TRUE;
}

void CDialogDataCheck::OnOK() 
{
	UpdateData(true);

	m_CH.nPeriods = m_nPeriods;

	/////////////////////////////////////
	if(m_nRadioNorm == 0)
		m_CH.nDeltaRankN = 0;
	else
		m_CH.nDeltaRankN = m_nNorm;

	/////////////////////////////////////
	if(m_nRadioRazor == 0)
		m_CH.nDeltaRankR = 0;
	else
		m_CH.nDeltaRankR = m_nRazor;

	/////////////////////////////////////
	if(m_nRadioBEV == 0)
		m_CH.nDeltaBEV = 0;
	else
		m_CH.nDeltaBEV = m_nBEV;

	/////////////////////////////////////
	if(m_nRadioPrice == 0)
		m_CH.fDeltaPrice = 0;
	else
		m_CH.fDeltaPrice = m_fPrice / 100.0;

	/////////////////////////////////////
	if(m_nRadioEPSNext == 0)
		m_CH.fDeltaEPSNext = 0;
	else
		m_CH.fDeltaEPSNext = m_fEPSNext / 100.0;

	/////////////////////////////////////
	if(m_nRadioEPSThis == 0)
		m_CH.fDeltaEPSThis = 0;
	else
		m_CH.fDeltaEPSThis = m_fEPSThis / 100.0;

	/////////////////////////////////////
	if(m_nRadioDebt == 0)
		m_CH.fDeltaDebt = 0;
	else
		m_CH.fDeltaDebt = m_fDebt / 100.0;


	CDialog::OnOK();
}

void CDialogDataCheck::OnButtonIncludeAll() 
{
	m_nRadioBEV = 1;
	m_nRadioDebt = 1;
	m_nRadioEPSNext = 1;
	m_nRadioEPSThis = 1;
	m_nRadioNorm = 1;
	m_nRadioPrice = 1;
	m_nRadioRazor = 1;

	UpdateData(false);
}

void CDialogDataCheck::OnButtonExcludeAll() 
{
	m_nRadioBEV = 0;
	m_nRadioDebt = 0;
	m_nRadioEPSNext = 0;
	m_nRadioEPSThis = 0;
	m_nRadioNorm = 0;
	m_nRadioPrice = 0;
	m_nRadioRazor = 0;

	UpdateData(false);
}
