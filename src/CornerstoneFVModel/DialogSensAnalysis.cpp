// DialogSensAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogSensAnalysis.h"
#include "WndGrid.h"
#include "ViewGraph.h"
#include "Universe.h"
#include "MacroParameters.h"
#include "MonteCarlo.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSensAnalysis dialog


CDialogSensAnalysis::CDialogSensAnalysis(CCompany* pCompany, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSensAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSensAnalysis)
	m_fDebt = 0.0;
	m_fDiscount = 0.0;
	m_fDiv = 0.0;
	m_fEPS = 0.0;
	m_fEPSLast = 0.0;
	m_fEPSNext = 0.0;
	m_fEPSThis = 0.0;
	m_fPrice = 0.0;
	m_fROC = 0.0;
	m_fROE = 0.0;
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
	m_pGraph = NULL;

	m_pCompany = pCompany;
}


void CDialogSensAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSensAnalysis)
	DDX_Control(pDX, IDC_COMBO_VIEW, m_comboView);
	DDX_Control(pDX, IDC_STATIC_GROWTH_RAZOR_FORE, m_stGrowthRazorFore);
	DDX_Control(pDX, IDC_STATIC_GROWTH_RAZOR, m_stGrowthRazor);
	DDX_Control(pDX, IDC_STATIC_GROWTH_FORE, m_stGrowthFore);
	DDX_Control(pDX, IDC_STATIC_GROWTH, m_stGrowth);
	DDX_Control(pDX, IDC_SLIDER_EPS_THIS, m_slEPSThis);
	DDX_Control(pDX, IDC_SLIDER_EPS_NEXT, m_slEPSNext);
	DDX_Control(pDX, IDC_SLIDER_EPS_LAST, m_slEPSLast);
	DDX_Control(pDX, IDC_EDIT_EPS_THIS, m_editEPSThis);
	DDX_Control(pDX, IDC_EDIT_EPS_NEXT, m_editEPSNext);
	DDX_Control(pDX, IDC_EDIT_EPS_LAST, m_editEPSLast);
	DDX_Control(pDX, IDC_SLIDER_ROE, m_slROE);
	DDX_Control(pDX, IDC_SLIDER_ROC, m_slROC);
	DDX_Control(pDX, IDC_SLIDER_PRICE, m_slPrice);
	DDX_Control(pDX, IDC_SLIDER_EPS, m_slEPS);
	DDX_Control(pDX, IDC_SLIDER_DIV, m_slDiv);
	DDX_Control(pDX, IDC_SLIDER_DISCOUNT, m_slDiscount);
	DDX_Control(pDX, IDC_SLIDER_DEBT, m_slDebt);
	DDX_Control(pDX, IDC_EDIT_ROE, m_editROE);
	DDX_Control(pDX, IDC_EDIT_ROC, m_editROC);
	DDX_Control(pDX, IDC_EDIT_PRICE, m_editPrice);
	DDX_Control(pDX, IDC_EDIT_EPS, m_editEPS);
	DDX_Control(pDX, IDC_EDIT_DIV, m_editDiv);
	DDX_Control(pDX, IDC_EDIT_DISCOUNT, m_editDiscount);
	DDX_Control(pDX, IDC_EDIT_DEBT, m_editDebt);
	DDX_Control(pDX, IDC_STATIC_ROI_RAZOR_FORE, m_stROIRazorFore);
	DDX_Control(pDX, IDC_STATIC_ROI_RAZOR, m_stROIRazor);
	DDX_Control(pDX, IDC_STATIC_ROI_FORE, m_stROIFore);
	DDX_Control(pDX, IDC_STATIC_MULTIPLE_RAZOR_FORE, m_stMultipleRazorFore);
	DDX_Control(pDX, IDC_STATIC_MULTIPLE_RAZOR, m_stMultipleRazor);
	DDX_Control(pDX, IDC_STATIC_MULTIPLE_FORE, m_stMultipleFore);
	DDX_Control(pDX, IDC_STATIC_ROI, m_stROI);
	DDX_Control(pDX, IDC_STATIC_MULTIPLE, m_stMultiple);
	DDX_Text(pDX, IDC_EDIT_DEBT, m_fDebt);
	DDX_Text(pDX, IDC_EDIT_DISCOUNT, m_fDiscount);
	DDX_Text(pDX, IDC_EDIT_DIV, m_fDiv);
	DDX_Text(pDX, IDC_EDIT_EPS, m_fEPS);
	DDX_Text(pDX, IDC_EDIT_EPS_LAST, m_fEPSLast);
	DDX_Text(pDX, IDC_EDIT_EPS_NEXT, m_fEPSNext);
	DDX_Text(pDX, IDC_EDIT_EPS_THIS, m_fEPSThis);
	DDX_Text(pDX, IDC_EDIT_PRICE, m_fPrice);
	DDX_Text(pDX, IDC_EDIT_ROC, m_fROC);
	DDX_Text(pDX, IDC_EDIT_ROE, m_fROE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSensAnalysis, CDialog)
	//{{AFX_MSG_MAP(CDialogSensAnalysis)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_ROC, OnKillfocusEditRoc)
	ON_BN_CLICKED(ID_BUTTON_RESET, OnButtonReset)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW, OnSelchangeComboView)
	ON_EN_KILLFOCUS(IDC_EDIT_EPS_THIS, OnKillfocusEditEpsThis)
	ON_EN_KILLFOCUS(IDC_EDIT_EPS_NEXT, OnKillfocusEditEpsNext)
	ON_BN_CLICKED(ID_BUTTON_EXPORT_RANKINGS, OnButtonExportRankings)
	ON_EN_KILLFOCUS(IDC_EDIT_ROE, OnKillfocusEditRoe)
	ON_EN_KILLFOCUS(IDC_EDIT_EPS, OnKillfocusEditEps)
	ON_EN_KILLFOCUS(IDC_EDIT_DEBT, OnKillfocusEditDebt)
	ON_EN_KILLFOCUS(IDC_EDIT_PRICE, OnKillfocusEditPrice)
	ON_EN_KILLFOCUS(IDC_EDIT_DISCOUNT, OnKillfocusEditDiscount)
	ON_EN_KILLFOCUS(IDC_EDIT_DIV, OnKillfocusEditDiv)
	ON_EN_KILLFOCUS(IDC_EDIT_EPS_LAST, OnKillfocusEditEpsLast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSensAnalysis message handlers

void CDialogSensAnalysis::OnOK() 
{	
	//CDialog::OnOK();
}

void CDialogSensAnalysis::OnCancel() 
{
	CDialog::OnCancel();
}

HBRUSH CDialogSensAnalysis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nID = pWnd->GetDlgCtrlID();
	if (nID == IDC_STATIC_MULTIPLE || 
		nID == IDC_STATIC_ROI || 
		nID == IDC_STATIC_MULTIPLE_FORE || 
		nID == IDC_STATIC_ROI_FORE || 
		nID == IDC_STATIC_MULTIPLE_RAZOR || 
		nID == IDC_STATIC_ROI_RAZOR || 
		nID == IDC_STATIC_MULTIPLE_RAZOR_FORE || 
		nID == IDC_STATIC_ROI_RAZOR_FORE || 
		nID == IDC_STATIC_GROWTH || 
		nID == IDC_STATIC_GROWTH_FORE || 
		nID == IDC_STATIC_GROWTH_RAZOR || 
		nID == IDC_STATIC_GROWTH_RAZOR_FORE )
	{
		switch(nID)
		{
		case IDC_STATIC_MULTIPLE:
		case IDC_STATIC_ROI:
		case IDC_STATIC_GROWTH:
			pDC->SetTextColor(RGB(0, 0, 100));
			break;
		case IDC_STATIC_MULTIPLE_FORE:
		case IDC_STATIC_ROI_FORE:
		case IDC_STATIC_GROWTH_FORE:
			pDC->SetTextColor(RGB(0, 100, 0));
			break;
		case IDC_STATIC_MULTIPLE_RAZOR:
		case IDC_STATIC_ROI_RAZOR:
		case IDC_STATIC_GROWTH_RAZOR:
			pDC->SetTextColor(RGB(100, 0, 0));
			break;
		case IDC_STATIC_MULTIPLE_RAZOR_FORE:
		case IDC_STATIC_ROI_RAZOR_FORE:
		case IDC_STATIC_GROWTH_RAZOR_FORE:
			pDC->SetTextColor(RGB(100, 100, 0));
			break;
		}
		
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		strcpy(lf.lfFaceName,"Arial");
		lf.lfHeight = (long)15;
		lf.lfWeight = 400;
		CFont font; font.CreateFontIndirect(&lf);
		pDC->SelectObject(&font);


		static CBrush br(RGB(0,0,0));
		return hbr;

	}

	return hbr;
}

BOOL CDialogSensAnalysis::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_fOriginalDiscount = m_pCompany->GetMacroParametersPointer()->GetDiscountRate();

	m_OriginalCompany = *m_pCompany;
	m_OriginalCompany.CalcAll();

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName,"MS Sans Serif");
	lf.lfHeight = (long)15;
	lf.lfWeight = 400;
	m_font.CreateFontIndirect(&lf);

	// Create Graph View
	m_pGraph = new CViewGraph;
	m_pGraph->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, 2345);
	m_pGraph->ShowWindow(SW_HIDE);

	// store the original ranks
	for(int i=0; i<m_pCompany->GetUniverse()->GetSize(); i++)
		m_pCompany->GetUniverse()->GetCompany(i)->StoreRanks();
	
	// Create Grid Window
	m_pGrid = new CWndGrid();
	m_pGrid->SetEnableScrollBars(false);
	m_pGrid->SetDefaultColumnSize(50);
	m_pGrid->SetDefaultRowSize(21);
	m_pGrid->SetTextAlign(DT_RIGHT);

	m_pGrid->Create(this);

	m_pGrid->SetColumnCount(6);
	m_pGrid->SetRowCount(13);

	m_pGrid->SetFixedCellsBkColor(RGB(100,100,100));
	m_pGrid->SetFixedCellsForeColor(RGB(255,255,255));

	m_pGrid->SetItemText(1, 0, "Actual EPV:");
	m_pGrid->SetItemText(2, 0, "RAV:");
	m_pGrid->SetItemText(3, 0, "PMV:");
	m_pGrid->SetItemText(4, 0, "Fore EPV:");
	m_pGrid->SetItemText(5, 0, "RAV:");
	m_pGrid->SetItemText(6, 0, "PMV:");
	m_pGrid->SetItemText(7, 0, "Razor EPV:");
	m_pGrid->SetItemText(8, 0, "RAV:");
	m_pGrid->SetItemText(9, 0, "PMV:");
	m_pGrid->SetItemText(10, 0, "Razor Fore EPV:");
	m_pGrid->SetItemText(11, 0, "RAV:");
	m_pGrid->SetItemText(12, 0, "PMV:");

	m_pGrid->SetItemText(0, 1, "Old Val");
	m_pGrid->SetItemText(0, 2, "New Val");
	m_pGrid->SetItemText(0, 3, "Rank");
	m_pGrid->SetItemText(0, 4, "P/Value");

	/*m_pGrid->SetRowBkColor(1, RGB(202,237,97));
	m_pGrid->SetRowBkColor(4, RGB(255,193,214));
	m_pGrid->SetRowBkColor(7, RGB(153,217,234));
	m_pGrid->SetRowBkColor(10, RGB(255,242,30));*/

	m_pGrid->SetRowBkColor(1, RGB(207,232,245));
	m_pGrid->SetRowBkColor(4, RGB(207,232,245));
	m_pGrid->SetRowBkColor(7, RGB(207,232,245));
	m_pGrid->SetRowBkColor(10, RGB(207,232,245));

	m_pGrid->SetColumnBkColor(1, RGB(220,220,220));
	m_pGrid->SetColumnBkColor(5, RGB(220,220,220));

	m_pGrid->SetItemOwnerDraw(1, 5, true);
	m_pGrid->SetItemOwnerDraw(2, 5, true);
	m_pGrid->SetItemOwnerDraw(3, 5, true);
	m_pGrid->SetItemOwnerDraw(4, 5, true);
	m_pGrid->SetItemOwnerDraw(5, 5, true);
	m_pGrid->SetItemOwnerDraw(6, 5, true);
	m_pGrid->SetItemOwnerDraw(7, 5, true);
	m_pGrid->SetItemOwnerDraw(8, 5, true);
	m_pGrid->SetItemOwnerDraw(9, 5, true);
	m_pGrid->SetItemOwnerDraw(10, 5, true);
	m_pGrid->SetItemOwnerDraw(11, 5, true);
	m_pGrid->SetItemOwnerDraw(12, 5, true);

	m_pGrid->SetColumnSize(0, 98);
	m_pGrid->SetColumnSize(5, 215);
	m_pGrid->SetCellsFont(m_font);

	m_pGrid->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGrid->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Set Original Data
	CString s;
	// EPV
	s.Format("%.2f", m_pCompany->GetEPV());
	m_pGrid->SetItemText(1, 1, s);

	s.Format("%u", m_pCompany->GetRankEPV());
	m_pGrid->SetItemText(1, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPV());
	m_pGrid->SetItemText(1, 3, s);

	// RAV
	s.Format("%.2f", m_pCompany->GetRAV());
	m_pGrid->SetItemText(2, 1, s);

	s.Format("%u", m_pCompany->GetRankRAV());
	m_pGrid->SetItemText(2, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAV());
	m_pGrid->SetItemText(2, 3, s);

	// PMV
	s.Format("%.2f", m_pCompany->GetPMV());
	m_pGrid->SetItemText(3, 1, s);

	s.Format("%u", m_pCompany->GetRankPMV());
	m_pGrid->SetItemText(3, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMV());
	m_pGrid->SetItemText(3, 3, s);

	// Forecast EPV
	s.Format("%.2f", m_pCompany->GetEPVFore());
	m_pGrid->SetItemText(4, 1, s);

	s.Format("%u", m_pCompany->GetRankEPVFore());
	m_pGrid->SetItemText(4, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVFore());
	m_pGrid->SetItemText(4, 3, s);

	// Forecast RAV
	s.Format("%.2f", m_pCompany->GetRAVFore());
	m_pGrid->SetItemText(5, 1, s);

	s.Format("%u", m_pCompany->GetRankRAVFore());
	m_pGrid->SetItemText(5, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVFore());
	m_pGrid->SetItemText(5, 3, s);

	// Forecast PMV
	s.Format("%.2f", m_pCompany->GetPMVFore());
	m_pGrid->SetItemText(6, 1, s);

	s.Format("%u", m_pCompany->GetRankPMVFore());
	m_pGrid->SetItemText(6, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVFore());
	m_pGrid->SetItemText(6, 3, s);

	// Razor EPV
	s.Format("%.2f", m_pCompany->GetEPVRazor());
	m_pGrid->SetItemText(7, 1, s);

	s.Format("%u", m_pCompany->GetRankEPVRazor());
	m_pGrid->SetItemText(7, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVRazor());
	m_pGrid->SetItemText(7, 3, s);

	// Razor RAV
	s.Format("%.2f", m_pCompany->GetRAVRazor());
	m_pGrid->SetItemText(8, 1, s);

	s.Format("%u", m_pCompany->GetRankRAVRazor());
	m_pGrid->SetItemText(8, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVRazor());
	m_pGrid->SetItemText(8, 3, s);

	// Razor PMV
	s.Format("%.2f", m_pCompany->GetPMVRazor());
	m_pGrid->SetItemText(9, 1, s);

	s.Format("%u", m_pCompany->GetRankPMVRazor());
	m_pGrid->SetItemText(9, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVRazor());
	m_pGrid->SetItemText(9, 3, s);

	// Razor Forecast EPV
	s.Format("%.2f", m_pCompany->GetEPVRazorFore());
	m_pGrid->SetItemText(10, 1, s);

	s.Format("%u", m_pCompany->GetRankEPVRazorFore());
	m_pGrid->SetItemText(10, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVRazorFore());
	m_pGrid->SetItemText(10, 3, s);

	// Razor Forecast RAV
	s.Format("%.2f", m_pCompany->GetRAVRazorFore());
	m_pGrid->SetItemText(11, 1, s);

	s.Format("%u", m_pCompany->GetRankRAVRazorFore());
	m_pGrid->SetItemText(11, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVRazorFore());
	m_pGrid->SetItemText(11, 3, s);

	// Razor Forecast PMV
	s.Format("%.2f", m_pCompany->GetPMVRazorFore());
	m_pGrid->SetItemText(12, 1, s);

	s.Format("%u", m_pCompany->GetRankPMVRazorFore());
	m_pGrid->SetItemText(12, 2, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVRazorFore());
	m_pGrid->SetItemText(12, 3, s);

	m_comboView.SetCurSel(0);

	// set data
	SetInputs();
	SetGridData();

	Resize();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSensAnalysis::SetInputs()
{
	if(!m_pCompany)
		return;

	CString s;

	// ROC
	m_fROC = m_pCompany->GetMeanROC();
	m_slROC.SetRange(0,1000);
	m_slROC.SetPageSize(1);
	m_slROC.SetPos((int)(m_fROC*10));
	s.Format("%.2f", m_fROC); m_editROC.SetWindowText(s);

	// ROE
	m_fROE = m_pCompany->GetMeanROE();
	m_slROE.SetRange(0,1000);
	m_slROE.SetPageSize(1);
	m_slROE.SetPos((int)(m_fROE*10));
	s.Format("%.2f", m_fROE); m_editROE.SetWindowText(s);

	// EPS
	m_fEPS = m_pCompany->GetMeanEPS();
	m_slEPS.SetRange(1, (int)(50.0*m_fEPS) );
	m_slEPS.SetPageSize(1);
	m_slEPS.SetPos((int)(m_fEPS*10));
	s.Format("%.2f", m_fEPS); m_editEPS.SetWindowText(s);

	// Debt
	m_fDebt = m_pCompany->GetDebtRatio();
	m_slDebt.SetRange(0, 100 );
	m_slDebt.SetPageSize(1);
	m_slDebt.SetPos((int)(m_fDebt*100));
	s.Format("%.2f", m_fDebt); m_editDebt.SetWindowText(s);

	// Price
	m_fPrice = m_pCompany->GetPrice();
	m_slPrice.SetRange(0, (int) (50.0*m_fPrice) );
	m_slPrice.SetPageSize(1);
	m_slPrice.SetPos((int)(m_fPrice*10));
	s.Format("%.2f", m_fPrice); m_editPrice.SetWindowText(s);

	// Discount Rate
	///////////////////////////////
	m_fDiscount = m_pCompany->GetMacroParametersPointer()->GetDiscountRate();
	m_slDiscount.SetRange(2*4, 15*4);
	m_slDiscount.SetPageSize(1);
	m_slDiscount.SetPos((int)(m_fDiscount*400));
	s.Format("%.2f", m_fDiscount*100); m_editDiscount.SetWindowText(s);

	// Dividend
	m_fDiv = m_pCompany->GetDividend();
	m_slDiv.SetRange(0, (int) (10*m_fEPS) );
	m_slDiv.SetPageSize(1);
	m_slDiv.SetPos((int)(m_fDiv*10));
	s.Format("%.2f", m_fDiv); m_editDiv.SetWindowText(s);

	// EPS Next Year
	m_fEPSNext = m_pCompany->GetEPSForecastNextYear();
	m_slEPSNext.SetRange(0, (int)(50.0*m_fEPSNext) );
	m_slEPSNext.SetPageSize(1);
	m_slEPSNext.SetPos((int)(m_fEPSNext*10));
	s.Format("%.2f", m_fEPSNext); m_editEPSNext.SetWindowText(s);

	// EPS This Year
	m_fEPSThis = m_pCompany->GetEPSForecastThisYear();
	m_slEPSThis.SetRange(0, (int)(50.0*m_fEPSThis) );
	m_slEPSThis.SetPageSize(1);
	m_slEPSThis.SetPos((int)(m_fEPSThis*10));
	s.Format("%.2f", m_fEPSThis); m_editEPSThis.SetWindowText(s);

	// EPS Last Year
	m_fEPSLast = m_pCompany->GetEPSForecastLastYear();
	m_slEPSLast.SetRange(0, (int)(50.0*m_fEPSLast) );
	m_slEPSLast.SetPageSize(1);
	m_slEPSLast.SetPos((int)(m_fEPSLast*10));
	s.Format("%.2f", m_fEPSLast); m_editEPSLast.SetWindowText(s);
}

void CDialogSensAnalysis::SetGridData()
{
	if(!m_pCompany)
		return;

	CString s;
	
	// EPV
	s.Format("%.2f", m_pCompany->GetEPV());
	m_pGrid->SetItemText(1, 2, s);

	s.Format("%u", m_pCompany->GetRankEPV());
	m_pGrid->SetItemText(1, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPV());
	m_pGrid->SetItemText(1, 4, s);

	// RAV
	s.Format("%.2f", m_pCompany->GetRAV());
	m_pGrid->SetItemText(2, 2, s);

	s.Format("%u", m_pCompany->GetRankRAV());
	m_pGrid->SetItemText(2, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAV());
	m_pGrid->SetItemText(2, 4, s);

	// PMV
	s.Format("%.2f", m_pCompany->GetPMV());
	m_pGrid->SetItemText(3, 2, s);

	s.Format("%u", m_pCompany->GetRankPMV());
	m_pGrid->SetItemText(3, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMV());
	m_pGrid->SetItemText(3, 4, s);

	// Forecast EPV
	s.Format("%.2f", m_pCompany->GetEPVFore());
	m_pGrid->SetItemText(4, 2, s);

	s.Format("%u", m_pCompany->GetRankEPVFore());
	m_pGrid->SetItemText(4, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVFore());
	m_pGrid->SetItemText(4, 4, s);

	// Forecast RAV
	s.Format("%.2f", m_pCompany->GetRAVFore());
	m_pGrid->SetItemText(5, 2, s);

	s.Format("%u", m_pCompany->GetRankRAVFore());
	m_pGrid->SetItemText(5, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVFore());
	m_pGrid->SetItemText(5, 4, s);

	// Forecast PMV
	s.Format("%.2f", m_pCompany->GetPMVFore());
	m_pGrid->SetItemText(6, 2, s);

	s.Format("%u", m_pCompany->GetRankPMVFore());
	m_pGrid->SetItemText(6, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVFore());
	m_pGrid->SetItemText(6, 4, s);

	// Razor EPV
	s.Format("%.2f", m_pCompany->GetEPVRazor());
	m_pGrid->SetItemText(7, 2, s);

	s.Format("%u", m_pCompany->GetRankEPVRazor());
	m_pGrid->SetItemText(7, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVRazor());
	m_pGrid->SetItemText(7, 4, s);

	// Razor RAV
	s.Format("%.2f", m_pCompany->GetRAVRazor());
	m_pGrid->SetItemText(8, 2, s);

	s.Format("%u", m_pCompany->GetRankRAVRazor());
	m_pGrid->SetItemText(8, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVRazor());
	m_pGrid->SetItemText(8, 4, s);

	// Razor PMV
	s.Format("%.2f", m_pCompany->GetPMVRazor());
	m_pGrid->SetItemText(9, 2, s);

	s.Format("%u", m_pCompany->GetRankPMVRazor());
	m_pGrid->SetItemText(9, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVRazor());
	m_pGrid->SetItemText(9, 4, s);

	// Razor Forecast EPV
	s.Format("%.2f", m_pCompany->GetEPVRazorFore());
	m_pGrid->SetItemText(10, 2, s);

	s.Format("%u", m_pCompany->GetRankEPVRazorFore());
	m_pGrid->SetItemText(10, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVRazorFore());
	m_pGrid->SetItemText(10, 4, s);

	// Razor Forecast RAV
	s.Format("%.2f", m_pCompany->GetRAVRazorFore());
	m_pGrid->SetItemText(11, 2, s);

	s.Format("%u", m_pCompany->GetRankRAVRazorFore());
	m_pGrid->SetItemText(11, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVRazorFore());
	m_pGrid->SetItemText(11, 4, s);

	// Razor Forecast PMV
	s.Format("%.2f", m_pCompany->GetPMVRazorFore());
	m_pGrid->SetItemText(12, 2, s);

	s.Format("%u", m_pCompany->GetRankPMVRazorFore());
	m_pGrid->SetItemText(12, 3, s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVRazorFore());
	m_pGrid->SetItemText(12, 4, s);

	// Multiple
	s.Format("%.2f", m_pCompany->GetMultiple()*100);
	m_stMultiple.SetWindowText(s);

	// ROI
	s.Format("%.2f%%", m_pCompany->GetMeanROI()*100);
	m_stROI.SetWindowText(s);

	// Adjusted Growth Rate
	s.Format("%.2f%%", m_pCompany->GetAdjustedGrowthRate()*100);
	m_stGrowth.SetWindowText(s);

	// Multiple Fore
	s.Format("%.2f", m_pCompany->GetMultipleFore()*100);
	m_stMultipleFore.SetWindowText(s);

	// ROI Fore
	s.Format("%.2f%%", m_pCompany->GetMeanROIFore()*100);
	m_stROIFore.SetWindowText(s);

	// Adjusted Growth Rate Fore
	s.Format("%.2f%%", m_pCompany->GetAdjustedGrowthRateFore()*100);
	m_stGrowthFore.SetWindowText(s);

	// Multiple Razor
	s.Format("%.2f", m_pCompany->GetMultipleRazor()*100);
	m_stMultipleRazor.SetWindowText(s);

	// ROI Razor
	s.Format("%.2f%%", m_pCompany->GetMeanROIRazor()*100);
	m_stROIRazor.SetWindowText(s);

	// Adjusted Growth Rate Razor
	s.Format("%.2f%%", m_pCompany->GetAdjustedGrowthRateRazor()*100);
	m_stGrowthRazor.SetWindowText(s);

	// Multiple Razor Fore
	s.Format("%.2f", m_pCompany->GetMultipleRazorFore()*100);
	m_stMultipleRazorFore.SetWindowText(s);

	// ROI Razor Fore
	s.Format("%.2f%%", m_pCompany->GetMeanROIRazorFore()*100);
	m_stROIRazorFore.SetWindowText(s);

	// Adjusted Growth Rate Razor Fore
	s.Format("%.2f%%", m_pCompany->GetAdjustedGrowthRateRazorFore()*100);
	m_stGrowthRazorFore.SetWindowText(s);
}

void CDialogSensAnalysis::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize();
}

void CDialogSensAnalysis::OnDestroy() 
{
	CDialog::OnDestroy();

	m_pCompany->GetMacroParametersPointer()->SetDiscountRate(m_fOriginalDiscount);
	*m_pCompany = m_OriginalCompany;
	m_pCompany->GetUniverse()->CalcAll();
	
	if(m_pGrid)
		m_pGrid->DestroyWindow();
}

void CDialogSensAnalysis::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcGraph(23, 52, 23+515, 52+13*21+1);

	if(m_pGrid)
		m_pGrid->MoveWindow(rcGraph);

	if(m_pGraph)
		m_pGraph->MoveWindow(rcGraph);
}

LRESULT CDialogSensAnalysis::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WMU_GRID_DRAWCELL)
	{
		STRUCT_OWNERDRAW_INFO *st = (STRUCT_OWNERDRAW_INFO*)wParam;

		int nRow = st->nRow;
		double fPercent;
		int nAll = m_pCompany->GetUniverse()->GetSize();
		switch(nRow)
		{
		case 1: fPercent = 1.0 - ( (double) m_pCompany->GetRankEPV() / (double)nAll );		break;
		case 2: fPercent = 1.0 - ( (double) m_pCompany->GetRankRAV() / (double)nAll );		break;
		case 3: fPercent = 1.0 - ( (double) m_pCompany->GetRankPMV() / (double)nAll );		break;

		case 4: fPercent = 1.0 - ( (double) m_pCompany->GetRankEPVFore() / (double)nAll );		break;
		case 5: fPercent = 1.0 - ( (double) m_pCompany->GetRankRAVFore() / (double)nAll );		break;
		case 6: fPercent = 1.0 - ( (double) m_pCompany->GetRankPMVFore() / (double)nAll );		break;

		case 7: fPercent = 1.0 - ( (double) m_pCompany->GetRankEPVRazor() / (double)nAll );		break;
		case 8: fPercent = 1.0 - ( (double) m_pCompany->GetRankRAVRazor() / (double)nAll );		break;
		case 9: fPercent = 1.0 - ( (double) m_pCompany->GetRankPMVRazor() / (double)nAll );		break;

		case 10: fPercent = 1.0 - ( (double) m_pCompany->GetRankEPVRazorFore()/ (double)nAll );		break;
		case 11: fPercent = 1.0 - ( (double) m_pCompany->GetRankRAVRazorFore()/ (double)nAll );		break;
		case 12: fPercent = 1.0 - ( (double) m_pCompany->GetRankPMVRazorFore()/ (double)nAll );		break;
		}

		DrawRankGraph(st->pDC, st->rcDraw, fPercent);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDialogSensAnalysis::DrawRankGraph(CDC* pDC, CRect rc, double fPercent)
{
	rc.DeflateRect(1,1);
	int nRight = (int) ( (double)rc.left + fPercent*rc.Width() );

	CRect rcFill(rc.left, rc.top, nRight, rc.bottom);
	pDC->FillSolidRect(rcFill, RGB(0,174,249));

	pDC->SetTextColor(RGB(0,0,0));

	CString s;
	s.Format("%u%%", (int)(fPercent*100.0));

	CRect rcText(nRight+2, rc.top, rc.right, rc.bottom);
	if(rcText.left > rcText.right-25)
	{
		rcText.left = nRight-25;
		pDC->SetTextColor(RGB(255,255,255));
	}

	CFont* pOldFont = pDC->SelectObject(&m_font);
	pDC->DrawText(s, rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	if(pOldFont)
		pDC->SelectObject(pOldFont);
}

void CDialogSensAnalysis::Calc()
{
	m_pCompany->DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);
	m_pCompany->GetUniverse()->RecalcRankings();
	
	SetGridData();
	Invalidate(false);
}

void CDialogSensAnalysis::CalcForDiscount()
{
	m_pCompany->GetMacroParametersPointer()->SetDiscountRate(m_fDiscount);
	m_pCompany->GetUniverse()->CalcAll();

	m_pCompany->DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);
	m_pCompany->GetUniverse()->RecalcRankings();
	
	SetGridData();
	Invalidate(false);
}

void CDialogSensAnalysis::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if((CSliderCtrl*)pScrollBar == &m_slROC)
	{
		m_fROC = (double)(m_slROC.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fROC);
		m_editROC.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slROE)
	{
		m_fROE = (double)(m_slROE.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fROE);
		m_editROE.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slEPS)
	{
		m_fEPS = (double)(m_slEPS.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fEPS);
		m_editEPS.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slDebt)
	{
		m_fDebt = (double)(m_slDebt.GetPos()) / 100.0;
		
		CString s; s.Format("%.2f", m_fDebt);
		m_editDebt.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slPrice)
	{
		m_fPrice = (double)(m_slPrice.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fPrice);
		m_editPrice.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slDiscount)
	{
		m_fDiscount = (double)(m_slDiscount.GetPos()) / 400.0;
		
		CString s; s.Format("%.2f", m_fDiscount*100);
		m_editDiscount.SetWindowText(s);

		CalcForDiscount();
	}

	if((CSliderCtrl*)pScrollBar == &m_slDiv)
	{
		m_fDiv = (double)(m_slDiv.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fDiv);
		m_editDiv.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slEPSNext)
	{
		m_fEPSNext = (double)(m_slEPSNext.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fEPSNext);
		m_editEPSNext.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slEPSThis)
	{
		m_fEPSThis = (double)(m_slEPSThis.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fEPSThis);
		m_editEPSThis.SetWindowText(s);

		Calc();
	}

	if((CSliderCtrl*)pScrollBar == &m_slEPSLast)
	{
		m_fEPSLast = (double)(m_slEPSLast.GetPos()) / 10.0;
		
		CString s; s.Format("%.2f", m_fEPSLast);
		m_editEPSLast.SetWindowText(s);

		Calc();
	}

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDialogSensAnalysis::OnKillfocusEditRoc() 
{
	if(!m_editROC.m_hWnd)
		return;

	CString s;
	m_editROC.GetWindowText(s);

	m_fROC = atof(s);
	m_slROC.SetPos((int)(m_fROC*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}	
}

void CDialogSensAnalysis::OnButtonReset() 
{
	m_pCompany->GetMacroParametersPointer()->SetDiscountRate(m_fOriginalDiscount);
	*m_pCompany = m_OriginalCompany;

	m_pCompany->GetUniverse()->CalcAll();
	SetInputs();
	SetGridData();

	Invalidate(false);
}

void CDialogSensAnalysis::OnSelchangeComboView() 
{
	m_pGrid->ShowWindow(SW_HIDE);
	m_pGraph->ShowWindow(SW_HIDE);

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}

}

const int nPoints = 100;

void CDialogSensAnalysis::SetupChartROCEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetMeanROC();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "ROC";
	m_pGraph->m_sLabelY = "EPV";

	// Series
	int nMin, nMax;
	m_slROC.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		c.DoSensitivityAnalysis(x, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);

		double y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetMeanROC(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetMeanROC(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartROEEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetMeanROE();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "ROE";
	m_pGraph->m_sLabelY = "EPV";

	// Series
	int nMin, nMax;
	m_slROE.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		c.DoSensitivityAnalysis(m_fROC, x, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);

		double y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetMeanROE(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetMeanROE(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartDebtEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetDebtRatio();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "Debt Ratio";
	m_pGraph->m_sLabelY = "EPV";

	// Series
	int nMin, nMax;
	m_slDebt.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		c.DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, x, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);

		double y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetDebtRatio(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetDebtRatio(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartEPSEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetMeanEPS();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "EPS";
	m_pGraph->m_sLabelY = "EPV";

	// Series
	int nMin, nMax;
	m_slEPS.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		c.DoSensitivityAnalysis(m_fROC, m_fROE, x, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);

		double y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetMeanEPS(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetMeanEPS(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}


void CDialogSensAnalysis::SetupChartDivEPV() 
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetDividend();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "Dividend";
	m_pGraph->m_sLabelY = "Actual EPV";

	// Series
	int nMin, nMax;
	m_slDiv.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		double y;
		c.DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, x, m_fEPSNext, m_fEPSThis, m_fEPSLast);
		
		y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetDividend(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetDividend(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartEPSNextYEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetEPSForecastNextYear();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "EPS Forecast Next Year";
	m_pGraph->m_sLabelY = "Actual EPV";

	// Series
	int nMin, nMax;
	this->m_slEPSNext.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		double y;
		c.DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, x, m_fEPSThis, m_fEPSLast);
		
		y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetEPSForecastNextYear(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetEPSForecastNextYear(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartEPSThisYEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetEPSForecastThisYear();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "EPS Forecast This Year";
	m_pGraph->m_sLabelY = "Actual EPV";

	// Series
	int nMin, nMax;
	this->m_slEPSThis.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		double y;
		c.DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, x, m_fEPSLast);
		
		y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetEPSForecastThisYear(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetEPSForecastThisYear(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartEPSLastYEPV()
{
	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetEPSForecastLastYear();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "EPS Last Year";
	m_pGraph->m_sLabelY = "Actual EPV";

	// Series
	int nMin, nMax;
	this->m_slEPSLast.GetRange(nMin, nMax);
	double fMin = (double)nMin / 10.0;
	double fMax = (double)nMax / 10.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		double y;
		c.DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, x);
		
		y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_OriginalCompany.GetEPSForecastLastYear(), "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(m_pCompany->GetEPSForecastLastYear(), "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::SetupChartDiscountEPV()
{
	CMacroParameters* pMacro = m_pCompany->GetMacroParametersPointer();
	double fOriginal = pMacro->GetDiscountRate();

	CCompany c; 
	c = *m_pCompany;

	double fX0 = c.GetDividend();
	double fY0 = c.GetEPV();

	// Labels
	m_pGraph->m_sLabelX = "Discount Rate";
	m_pGraph->m_sLabelY = "EPV";

	// Series
	int nMin, nMax;
	m_slDiscount.GetRange(nMin, nMax);
	double fMin = (double)nMin / 400.0;
	double fMax = (double)nMax / 400.0;
	double fRange = fMax - fMin;
	double fStep = fRange/(double)nPoints;
	
	m_pGraph->RemoveAll();
	CArray<double,double> arX, arY, arY2;

	for(int i=0; i<nPoints; i++)
	{
		double x = fMin + (double)i*fStep;
		arX.Add(x);

		double y;
		pMacro->SetDiscountRate(x);
		c.DoSensitivityAnalysis(m_fROC, m_fROE, m_fEPS, m_fDebt, m_fPrice, m_fDiv, m_fEPSNext, m_fEPSThis, m_fEPSLast);
		
		y = c.GetEPV();
		arY.Add(y);

		y = c.GetEPVFore();
		arY2.Add(y);

	}

	m_pGraph->AddSeries("Actual", arX, arY, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,128,0));
	m_pGraph->AddSeries("Forecast", arX, arY2, SERIES_TYPE_LINE, 2, PS_SOLID, RGB(0,174,249));

	// Original Values
	CString s; 
	
	s.Format("Original: %.2f", m_OriginalCompany.GetEPV());
	m_pGraph->AddLine(m_OriginalCompany.GetEPV(), s, LINE_TEXT_LEFTBOTTOM, LINE_TYPE_HORZ, 1, PS_DASH, RGB(235,0,0));

	s.Format("Current: %.2f", m_pCompany->GetEPV());
	m_pGraph->AddLine(m_pCompany->GetEPV(), s, LINE_TEXT_TOPRIGHT, LINE_TYPE_HORZ, 1, PS_DASH, RGB(0,128,0));

	m_pGraph->AddLine(m_fOriginalDiscount, "", LINE_TEXT_LEFTBOTTOM, LINE_TYPE_VERT, 1, PS_DASH, RGB(235,0,0));
	m_pGraph->AddLine(fOriginal, "", LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_DASH, RGB(0,128,0));

	pMacro->SetDiscountRate(fOriginal);
	
	m_pGraph->ShowWindow(SW_SHOW);
	m_pGraph->RedrawWindow();
}

void CDialogSensAnalysis::OnKillfocusEditEpsThis() 
{
	CString s;
	m_editEPSThis.GetWindowText(s);

	m_fEPSThis = atof(s);
	m_slEPSThis.SetPos((int)(m_fEPSThis*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}
}

void CDialogSensAnalysis::OnKillfocusEditEpsNext() 
{
	CString s;
	m_editEPSNext.GetWindowText(s);

	m_fEPSNext = atof(s);
	m_slEPSNext.SetPos((int)(m_fEPSNext*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}
}

void CDialogSensAnalysis::OnButtonExportRankings() 
{
	CTime date = CTime::GetCurrentTime();
	CString sTime = date.Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("CIP_Universe_Values_%s.txt", sTime);

	static char szFilters[] ="Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog fileDlg (false, "txt", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

	if(fileDlg.DoModal() != IDOK )
		return;
	
	CString sName = fileDlg.GetPathName();
	
	CStdioFile file;
	if(!file.Open(sName, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sErr; sErr.Format("Unable to create file: '%s'", sName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		return;
	}

	CString s = "Ticker\tOriginal Rank\tCurrent\tDifference\n";
	file.WriteString(s);

	for(int i=0; i<m_pCompany->GetUniverse()->GetSize(); i++)
	{
		CCompany* pCompany = m_pCompany->GetUniverse()->GetCompany(i);
		CString sTicker = pCompany->GetTicker();

		CString s;
		s.Format("%s\t%u\t%u\t%i\n", sTicker, pCompany->GetEPVRankOriginal(), pCompany->GetRankEPV(), pCompany->GetEPVRankDiff());
		file.WriteString(s);
	}
}

void CDialogSensAnalysis::OnKillfocusEditRoe() 
{
	if(!m_editROE.m_hWnd)
		return;

	CString s;
	m_editROE.GetWindowText(s);

	m_fROE = atof(s);
	m_slROE.SetPos((int)(m_fROE*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}	
}

void CDialogSensAnalysis::OnKillfocusEditEps() 
{
	if(!m_editEPS.m_hWnd)
		return;

	CString s;
	m_editEPS.GetWindowText(s);

	m_fEPS = atof(s);
	m_slEPS.SetPos((int)(m_fEPS*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}	
}

void CDialogSensAnalysis::OnKillfocusEditDebt() 
{
	if(!m_editDebt.m_hWnd)
		return;

	CString s;
	m_editDebt.GetWindowText(s);

	m_fDebt = atof(s);
	if(m_fDebt < 0) m_fDebt = 0;
	m_slDebt.SetPos((int)(m_fDebt*100));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}
}

void CDialogSensAnalysis::OnKillfocusEditPrice() 
{
	if(!m_editPrice.m_hWnd)
		return;

	CString s;
	m_editPrice.GetWindowText(s);

	m_fPrice = atof(s);
	m_slPrice.SetPos((int)(m_fPrice*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}	
}

void CDialogSensAnalysis::OnKillfocusEditDiscount() 
{
	if(!m_editDiscount.m_hWnd)
		return;

	CString s;
	m_editDiscount.GetWindowText(s);

	m_fDiscount = atof(s);
	m_slDiscount.SetPos((int)(m_fDiscount*400));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}	
}

void CDialogSensAnalysis::OnKillfocusEditDiv() 
{
	if(!m_editDiv.m_hWnd)
		return;

	CString s;
	m_editDiv.GetWindowText(s);

	m_fDiv = atof(s);
	m_slDiv.SetPos((int)(m_fDiv*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}
}

void CDialogSensAnalysis::OnKillfocusEditEpsLast() 
{
	if(!m_editEPSLast.m_hWnd)
		return;

	CString s;
	m_editEPSLast.GetWindowText(s);

	m_fEPSLast = atof(s);
	m_slEPSLast.SetPos((int)(m_fEPSLast*10));
		
	Calc();

	int nSel = m_comboView.GetCurSel();
	switch(nSel)
	{
	case 0: m_pGrid->ShowWindow(SW_SHOW);	break;

	case 1: SetupChartDiscountEPV();		break;
	case 2: SetupChartROCEPV();				break;
	case 3: SetupChartROEEPV();				break;
	case 4: SetupChartEPSEPV();				break;
	case 5: SetupChartDebtEPV();			break;
	case 6: SetupChartDivEPV();				break;
	case 7: SetupChartEPSNextYEPV();		break;
	case 8: SetupChartEPSThisYEPV();		break;
	case 9: SetupChartEPSLastYEPV();		break;
	}	
}
