// DialogMonteCarloInputs.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogMonteCarloInputs.h"

#include "ViewMonteCarlo.h"
#include "ViewGraph.h"

#include "Company.h"
#include "Universe.h"
#include "MacroParameters.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialogMonteCarloInputs* pThis;

/////////////////////////////////////////////////////////////////////////////
// CDialogMonteCarloInputs dialog


CDialogMonteCarloInputs::CDialogMonteCarloInputs(CCompany* pCompany, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMonteCarloInputs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMonteCarloInputs)
	m_nMarketDirection = 1;
	m_nMarketVariance = 1;
	m_nDistrib = 1;
	m_fF1 = 0.0;
	m_fF2 = 0.0;
	m_nIterations = 10000;
	m_bInteractive = FALSE;
	m_bOutActual = true;
	m_bOutFore = true;
	m_bOutRazor = true;
	m_bOutForeRazor = true;
	//}}AFX_DATA_INIT

	m_pCompany = pCompany;
	m_pParent = (CViewMonteCarlo*)pParent;
}
 
void CDialogMonteCarloInputs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMonteCarloInputs)
	DDX_Control(pDX, IDC_EDIT_F2, m_editF2);
	DDX_Control(pDX, IDC_EDIT_F1, m_editF1);
	DDX_Control(pDX, IDC_COMBO_DISTRIB, m_comboDistrib);
	DDX_Control(pDX, IDC_LIST, m_ListParams);
	DDX_Radio(pDX, IDC_RADIO_DIRECTION_1, m_nMarketDirection);
	DDX_Radio(pDX, IDC_RADIO_VARIANCE_1, m_nMarketVariance);
	DDX_CBIndex(pDX, IDC_COMBO_DISTRIB, m_nDistrib);
	DDX_Text(pDX, IDC_EDIT_F1, m_fF1);
	DDX_Text(pDX, IDC_EDIT_F2, m_fF2);
	DDX_Text(pDX, IDC_EDIT_ITERATIONS, m_nIterations);
	DDX_Check(pDX, IDC_CHECK_INTERACTIVE, m_bInteractive);
	DDX_Check(pDX, IDC_CHECK_OUT_ACTUAL, m_bOutActual);
	DDX_Check(pDX, IDC_CHECK_OUT_FORE, m_bOutFore);
	DDX_Check(pDX, IDC_CHECK_OUT_RAZOR, m_bOutRazor);
	DDX_Check(pDX, IDC_CHECK_OUT_FORWARD_RAZOR, m_bOutForeRazor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogMonteCarloInputs, CDialog)
	//{{AFX_MSG_MAP(CDialogMonteCarloInputs)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON_SETPARAMS, OnButtonSetparams)
	ON_BN_CLICKED(IDC_BUTTON_RUN, OnButtonRun)
	ON_EN_KILLFOCUS(IDC_EDIT_F1, OnKillfocusEditF1)
	ON_EN_KILLFOCUS(IDC_EDIT_F2, OnKillfocusEditF2)
	ON_CBN_SELCHANGE(IDC_COMBO_DISTRIB, OnSelchangeComboDistrib)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMonteCarloInputs message handlers

BOOL CDialogMonteCarloInputs::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	strcpy(lf.lfFaceName, "Arial");
	lf.lfHeight = 14;
	m_Font.CreateFontIndirect(&lf);

	m_ListParams.SetFont(&m_Font);
	
	m_ListParams.AddString("Discount Rate");
	m_ListParams.AddString("Next Year EPS (Forecast)");
	m_ListParams.AddString("This Year EPS (Forecast)");
	m_ListParams.AddString("Last Year EPS");
	m_ListParams.AddString("Dividend");
	m_ListParams.AddString("Debt Ratio");
	m_ListParams.AddString("Return on Capital (ROC)");
	m_ListParams.AddString("Return on Equity (ROE)");
	m_ListParams.AddString("Earnings Per Share (EPS)");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MonteCarloCallBack(int nInputs, double* inputs, int nOuts, double* outs)
{
	CCompany c; 
	c = *pThis->m_pCompany;

	c.GetMacroParametersPointer()->SetDiscountRate(inputs[0]);
	c.DoSensitivityAnalysis(inputs[6], inputs[7], inputs[8], inputs[5], c.GetPrice(), inputs[4], 
							inputs[1], inputs[2], inputs[3]);
		
	outs[0] = c.GetEPV();
	outs[1] = c.GetEPVFore();
	outs[2] = c.GetEPVRazor();
	outs[3] = c.GetEPVRazorFore();

	//pThis->m_pParent->SetCharts();
	//pThis->m_pParent->m_pGraph->RedrawWindow();
}

void CDialogMonteCarloInputs::SetupSimulation()
{
	// Sample size - iterations
	m_Simulation.SetSampleSize(m_nIterations);

	// input size
	m_Simulation.SetInputCount(9);
	for(int i=0; i<9; i++)
		m_Simulation.GetInput(i)->SetType(MC_PARAM_NORMAL);

	// output size
	int nOuts=0;
	if(m_bOutActual)
		++nOuts;
	if(m_bOutFore)
		++nOuts;
	if(m_bOutRazor)
		++nOuts;
	if(m_bOutForeRazor)
		++nOuts;
	m_Simulation.SetOutCount(nOuts);

	// Callback calc function
	m_Simulation.SetCalcFunction(MonteCarloCallBack);

	SetDistributions(m_nMarketDirection, m_nMarketVariance);
}

void CDialogMonteCarloInputs::SetDistributions(int nDirection, int nVariance)
{
	if(!m_pCompany)
		return;

	int index = 0;
	double fMean, fStdev;

	// discount rate
	fMean = m_pCompany->GetMacroParametersPointer()->GetDiscountRate();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);
	
	// Next Year EPS
	++index;
	fMean = m_pCompany->GetEPSForecastNextYear();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// This Year EPS
	++index;
	fMean = m_pCompany->GetEPSForecastThisYear();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// Last Year EPS
	++index;
	fMean = m_pCompany->GetEPSForecastLastYear();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// Dividend
	++index;
	fMean = m_pCompany->GetDividend();
	//ApplyMarketConditions(fMean, fStdev);
	fStdev = sqrt(fMean)/4.0;
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// Debt Ratio
	++index;
	fMean = m_pCompany->GetDebtRatio();
	//ApplyMarketConditions(fMean, fStdev);
	fStdev = sqrt(fMean)/4.0;
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// Mean ROC
	++index;
	fMean = m_pCompany->GetMeanROC();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// Mean ROE
	++index;
	fMean = m_pCompany->GetMeanROE();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);

	// Mean EPS
	++index;
	fMean = m_pCompany->GetMeanEPS();
	ApplyMarketConditions(fMean, fStdev);
	m_Simulation.GetInput(index)->SetF1(fMean);
	m_Simulation.GetInput(index)->SetF2(fStdev);
}

void CDialogMonteCarloInputs::ApplyMarketConditions(double& fMean, double &fStdev)
{
	switch(m_nMarketVariance)
	{
	case 0: fStdev = sqrt(fMean); break;
	case 1: fStdev = sqrt(fMean)/2.0; break;
	case 2: fStdev = sqrt(fMean)/3.0; break;
	}
	
	switch(m_nMarketDirection)
	{
	case 0: fMean += fStdev/2.0; break;
	case 2: fMean -= fStdev/2.0; break;
	}
}

void CDialogMonteCarloInputs::OnSelchangeList() 
{
	int nSel = m_ListParams.GetCurSel();
	if(nSel == -1)
		return;

	int nCombo;
	int nDistrib = m_Simulation.GetInput(nSel)->GetType();
	switch(nDistrib)
	{
	case MC_PARAM_CONST:		nCombo = 0; break;
	case MC_PARAM_UNIFORM:		nCombo = 3; break;
	case MC_PARAM_NORMAL:		nCombo = 1; break;
	case MC_PARAM_LOGNORMAL:	nCombo = 2; break;
	case MC_PARAM_EXPONENTIAL:	nCombo = 4; break;
	case MC_PARAM_ERLANG:		nCombo = 7; break;
	case MC_PARAM_CHISQUARE:	nCombo = 6; break;
	case MC_PARAM_STUDENT:		nCombo = 5; break;
	}
	m_comboDistrib.SetCurSel(nCombo);


	CString s;
	m_fF1 = m_Simulation.GetInput(nSel)->GetF1();
	m_fF2 = m_Simulation.GetInput(nSel)->GetF2();
	
	s.Format("%.4f", m_fF1);
	m_editF1.SetWindowText(s);
	s.Format("%.4f", m_fF2);
	m_editF2.SetWindowText(s);
}

void CDialogMonteCarloInputs::OnButtonSetparams() 
{
	UpdateData();
 	SetDistributions(m_nMarketDirection, m_nMarketVariance);	
	
	OnSelchangeList();
}

void CDialogMonteCarloInputs::OnButtonRun() 
{
	UpdateData();

	double fDiscountOriginal = m_pCompany->GetMacroParametersPointer()->GetDiscountRate();
	pThis = this;

	m_Simulation.SetSampleSize(m_nIterations);
	m_Simulation.Run();	

	m_pCompany->GetMacroParametersPointer()->SetDiscountRate(fDiscountOriginal);
	m_pParent->SetCharts();
}

void CDialogMonteCarloInputs::OnKillfocusEditF1() 
{
	if(!m_editF1.m_hWnd)
		return;

	int nSel = m_ListParams.GetCurSel();
	if(nSel == -1)
		return;

	CString s;
	m_editF1.GetWindowText(s);

	m_fF1 = atof(s);
	m_Simulation.GetInput(nSel)->SetF1(m_fF1);
}

void CDialogMonteCarloInputs::OnKillfocusEditF2() 
{
	if(!m_editF2.m_hWnd)
		return;

	int nSel = m_ListParams.GetCurSel();
	if(nSel == -1)
		return;

	CString s;
	m_editF2.GetWindowText(s);

	m_fF1 = atof(s);
	m_Simulation.GetInput(nSel)->SetF2(m_fF1);
}

void CDialogMonteCarloInputs::OnSelchangeComboDistrib() 
{
	int nSel = m_comboDistrib.GetCurSel();
	int nParam = m_ListParams.GetCurSel();

	if(nParam == -1)
		return;

	CMonteCarloInput* pParam = m_Simulation.GetInput(nParam);
	switch(nSel)
	{
	case 0:	pParam->SetType(MC_PARAM_CONST); break;
	case 1:	pParam->SetType(MC_PARAM_NORMAL); break;
	case 2:	pParam->SetType(MC_PARAM_LOGNORMAL); break;
	case 3:	pParam->SetType(MC_PARAM_UNIFORM); break;
	case 4:	pParam->SetType(MC_PARAM_EXPONENTIAL); break;
	case 5:	pParam->SetType(MC_PARAM_STUDENT); break;
	case 6:	pParam->SetType(MC_PARAM_CHISQUARE); break;
	case 7:	pParam->SetType(MC_PARAM_ERLANG); break;
	}
}
