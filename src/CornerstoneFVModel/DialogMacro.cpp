// DialogMacro.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMacro dialog


CDialogMacro::CDialogMacro(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMacro::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMacro)
	m_fDiscount = 0.0;
	m_fInterest = 0.0;
	m_fInterestFactor = 0.0;
	m_fMaxReinvest = 0.0;
	m_nPeriodsMovingAve = 0;
	m_fTax = 0.0;
	m_nYearsDiscountEarnings = 0;
	m_nYearsEPSROIMeans = 0;
	m_fMinGrowth = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogMacro::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMacro)
	DDX_Control(pDX, IDC_EDIT_MIN_GROWTH, m_editMinGrowth);
	DDX_Control(pDX, IDC_EDIT_PERIODS_MOVING_AVE, m_editPeriodsMovingAve);
	DDX_Control(pDX, IDC_EDIT_YEARS_EPSROI_MEANS, m_editYearsEPSRIOMeans);
	DDX_Control(pDX, IDC_EDIT_YEARS_DISCOUNT_EARNINGS, m_editYearsDiscountEarnings);
	DDX_Control(pDX, IDC_EDIT_MAX_REINVEST, m_editMaxReinvestment);
	DDX_Control(pDX, IDC_EDIT_TAX, m_editTax);
	DDX_Control(pDX, IDC_EDIT_INTEREST, m_editInterest);
	DDX_Control(pDX, IDC_EDIT_INTERESTFACTOR, m_edtInterestFactor);
	DDX_Control(pDX, IDC_EDIT_DISCOUNT, m_editDiscount);
	DDX_Text(pDX, IDC_EDIT_DISCOUNT, m_fDiscount);
	DDX_Text(pDX, IDC_EDIT_INTEREST, m_fInterest);
	DDX_Text(pDX, IDC_EDIT_INTERESTFACTOR, m_fInterestFactor);
	DDX_Text(pDX, IDC_EDIT_MAX_REINVEST, m_fMaxReinvest);
	DDX_Text(pDX, IDC_EDIT_PERIODS_MOVING_AVE, m_nPeriodsMovingAve);
	DDX_Text(pDX, IDC_EDIT_TAX, m_fTax);
	DDX_Text(pDX, IDC_EDIT_YEARS_DISCOUNT_EARNINGS, m_nYearsDiscountEarnings);
	DDX_Text(pDX, IDC_EDIT_YEARS_EPSROI_MEANS, m_nYearsEPSROIMeans);
	DDX_Text(pDX, IDC_EDIT_MIN_GROWTH, m_fMinGrowth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogMacro, CDialog)
	//{{AFX_MSG_MAP(CDialogMacro)
	ON_EN_KILLFOCUS(IDC_EDIT_DISCOUNT, OnKillfocusEditDiscount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMacro message handlers

BOOL CDialogMacro::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_edtInterestFactor.EnableWindow(false);
	m_editMaxReinvestment.EnableWindow(false);

	SetDataToControls();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMacro::SetDataToControls()
{
	CString s;

	m_fDiscount = m_Macro.GetDiscountRate();
	s.Format("%.2f", m_fDiscount*100);
	m_editDiscount.SetWindowText(s);

	m_fInterestFactor = m_Macro.GetInterestFactor();
	s.Format("%.2f", m_fInterestFactor*100);
	m_edtInterestFactor.SetWindowText(s);

	m_fMaxReinvest = m_Macro.GetMaxReinvestFactor();
	s.Format("%.2f", m_fMaxReinvest*100);
	m_editMaxReinvestment.SetWindowText(s);

	m_fInterest = m_Macro.GetInterestRate();
	s.Format("%.2f", m_fInterest*100);
	m_editInterest.SetWindowText(s);

	m_fTax = m_Macro.GetTaxRate();
	s.Format("%.2f", m_fTax*100);
	m_editTax.SetWindowText(s);
	
	m_nYearsDiscountEarnings = m_Macro.GetYearsToDiscountEarnings();
	s.Format("%u", m_nYearsDiscountEarnings);
	m_editYearsDiscountEarnings.SetWindowText(s);

	m_nYearsEPSROIMeans = m_Macro.GetYearsToUseEpsRoi();
	s.Format("%u", m_nYearsEPSROIMeans);
	m_editYearsEPSRIOMeans.SetWindowText(s);

	m_nPeriodsMovingAve = m_Macro.GetPeriodsMA();
	s.Format("%u", m_nPeriodsMovingAve);
	m_editPeriodsMovingAve.SetWindowText(s);

	m_fMinGrowth = m_Macro.GetMinGrowth();
	s.Format("%.2f", m_fMinGrowth*100);
	m_editMinGrowth.SetWindowText(s);
}

BOOL CDialogMacro::GetDataFromControls()
{
	CString s;
	double f;
	int n;

	m_editDiscount.GetWindowText(s);
	f = atof(s) / 100.0;
	if(f < 0 || f > 1)
	{
		AfxMessageBox("Please enter valid Discount Rate between 0.0% and 100.0%");
		return false;
	}
	m_Macro.SetDiscountRate(f);

	/*m_edtInterestFactor.GetWindowText(s);
	f = atof(s) / 100.0;
	m_Macro.SetInterestFactor(f);

	m_editMaxReinvestment.GetWindowText(s);
	f = atof(s) / 100.0;
	m_Macro.SetMaxReinvestFactor(f);*/

	m_editInterest.GetWindowText(s);
	f = atof(s) / 100.0;
	if(f < 0 || f > 1)
	{
		AfxMessageBox("Please enter valid Interest Rate between 0.0% and 100.0%");
		return false;
	}
	m_Macro.SetInterestRate(f);

	m_editTax.GetWindowText(s);
	f = atof(s) / 100.0;
	if(f < 0 || f > 1)
	{
		AfxMessageBox("Please enter valid Tax Rate between 0.0% and 100.0%");
		return false;
	}
	m_Macro.SetTaxRate(f);


	m_editYearsDiscountEarnings.GetWindowText(s);
	n = atoi(s);
	if(n < 1 || n > 50)
	{
		AfxMessageBox("Please enter valid number for years to discount eranings; between 1 and 50");
		return false;
	}
	m_Macro.SetYearsToDiscountEarnings(n);

	m_editYearsEPSRIOMeans.GetWindowText(s);
	n = atoi(s);
	if(n < 1 || n > 50)
	{
		AfxMessageBox("Please enter valid number for years to calculate ROI; between 1 and 50");
		return false;
	}
	m_Macro.SetYearsToUseEpsRoi(n);

	m_editPeriodsMovingAve.GetWindowText(s);
	n = atoi(s);
	if(n < 1 || n > 200)
	{
		AfxMessageBox("Please enter valid number of periods for Moving Average calculation; between 1 and 200");
		return false;
	}
	m_Macro.SetPeriodsMA(n);

	m_editMinGrowth.GetWindowText(s);
	f = atof(s) / 100.0;
	if(f < 0 || f > 1)
	{
		AfxMessageBox("Please enter valid Interest Rate between 0.0% and 100.0%");
		return false;
	}
	m_Macro.SetMinGrowth(f);

	return true;
}

void CDialogMacro::OnOK() 
{
	if(!GetDataFromControls())
		return;

	CDialog::OnOK();
}

void CDialogMacro::OnKillfocusEditDiscount() 
{
	CString s;
	double f;

	m_editDiscount.GetWindowText(s);
	f = atof(s) / 100.0;
	if(f < 0.02 || f > 0.15)
	{
		AfxMessageBox("Please enter valid Discount Rate between 2.0% and 15.0%");
		return;
	}
	m_Macro.SetDiscountRate(f);

	m_fInterestFactor = m_Macro.GetInterestFactor();
	s.Format("%.2f", m_fInterestFactor*100);
	m_edtInterestFactor.SetWindowText(s);

	m_fMaxReinvest = m_Macro.GetMaxReinvestFactor();
	s.Format("%.2f", m_fMaxReinvest*100);
	m_editMaxReinvestment.SetWindowText(s);
}
