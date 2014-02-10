// ViewDailySummary.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "ViewDailySummary.h"
#include "View2x2Metrics.h"
#include "Dialog2Minute.h"
#include "Dialog2MinuteV2.h"
#include "DialogConfirmWeight.h"

#include <SQLUser.h>
#include <SQLDailySum.h>

extern CApp theApp ;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDailySummary

IMPLEMENT_DYNCREATE(CViewDailySummary, CFormView)

CViewDailySummary::CViewDailySummary()
	: CFormView(CViewDailySummary::IDD)
{ 
	//{{AFX_DATA_INIT(CViewDailySummary)
	m_nExecution = 0;
	m_nVision = 0;
	//}}AFX_DATA_INIT
	m_pViewMetrics = NULL;
	m_nID = -1;
	m_nUserLevel = 0;
	m_bConfirmed = false;
}

CViewDailySummary::~CViewDailySummary()
{
}

void CViewDailySummary::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDailySummary)
	DDX_Control(pDX, IDC_EDIT_ONELINER, m_editOneLiner);
	DDX_Control(pDX, IDC_COMBO_EXECUTION, m_comboExecution);
	DDX_Control(pDX, IDC_COMBO_VISION, m_comboVision);
	DDX_Control(pDX, IDC_SLIDER_FUNDAMENTALS, m_sliderFund);
	DDX_Control(pDX, IDC_SLIDER_VALUATION, m_sliderValue);
	DDX_Control(pDX, IDC_COMBO_BIAS, m_comboBias);
	DDX_Control(pDX, IDC_COMBO_TARGET_WEIGHT, m_comboTarget);
	DDX_Control(pDX, IDC_EDIT_NOTES_VALUATION, m_editNotesValuation);
	DDX_Control(pDX, IDC_EDIT_NOTES_MANAGEMENT, m_editNotesManagement);
	DDX_Control(pDX, IDC_EDIT_NOTES_FUNDAMENTALS, m_editNotesFundamentals);
	DDX_Control(pDX, IDC_EDIT_ITEMDATE, m_editDate);
	DDX_Control(pDX, IDC_COMBO_COMPANY, m_comboCompany);
	DDX_Control(pDX, IDC_COMBO_ANALYST, m_comboAnalysts);
	DDX_CBIndex(pDX, IDC_COMBO_EXECUTION, m_nExecution);
	DDX_CBIndex(pDX, IDC_COMBO_VISION, m_nVision);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDailySummary, CFormView)
	//{{AFX_MSG_MAP(CViewDailySummary)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_COMMAND(ID_DAILYSUMS_SAVE, OnDailysumsSave)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_ANALYST, OnSelchangeComboAnalyst)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPANY, OnSelchangeComboCompany)
	ON_EN_KILLFOCUS(IDC_EDIT_ITEMDATE, OnKillfocusEditItemdate)
	ON_COMMAND(ID_DAILYSUMS_2MINUTE, OnDailysums2minute)
	ON_COMMAND(ID_DAILYSUMS_PRINT, OnDailysumsPrint)
	ON_COMMAND(ID_DAILYSUMS_UNLOCK, OnDailysumsUnlock)
	ON_COMMAND(ID_DAILYSUMS_LOCK, OnDailysumsLock)
	ON_BN_CLICKED(IDC_CONFIRM, OnConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDailySummary diagnostics

#ifdef _DEBUG
void CViewDailySummary::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewDailySummary::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDailySummary message handlers

void CViewDailySummary::SelectUserInCombo(int nUserId)
{
	int nCount = m_comboAnalysts.GetCount();
	for(int i=0; i<nCount; i++)
	{
		int nId = m_comboAnalysts.GetItemData(i);
		if(nId == nUserId)
			m_comboAnalysts.SetCurSel(i);
	}
}

void CViewDailySummary::SetPermissions()
{
	if(m_nUserLevel >= 3)
	{
		m_comboAnalysts.EnableWindow(true);
		return;
	}

	// Company
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		m_comboAnalysts.EnableWindow(true);
		return;
	}
	int nCompanyId = m_comboCompany.GetItemData(nSel);

	// Date
	CString sDate;
	m_editDate.GetWindowText(sDate);
	COleDateTime date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(date.m_status != COleDateTime::valid)
	{
		m_comboAnalysts.EnableWindow(true);
		return;
	}

	int nUnlock = CSQLDailySum::IsUnlocked(nCompanyId, date);
	if(nUnlock == 0)
	{
		SelectUserInCombo(theApp.m_nUserId);
		m_comboAnalysts.EnableWindow(false);
	}
	else
		m_comboAnalysts.EnableWindow(true);
}

void CViewDailySummary::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	// Load Users (Analysts)
	int i;
	int nIndex = m_comboAnalysts.AddString("- Select Analyst -");
	CArray<int,int> arIds;
	CStringArray arUsers;
	CSQLUser::GetUserIdAndNames(arIds, arUsers);
	for(i=0; i<arIds.GetSize(); i++)
	{
		int nID = arIds[i];
		CString sUser = arUsers[i];

		nIndex = m_comboAnalysts.AddString(sUser);
		m_comboAnalysts.SetItemData(nIndex, nID);
	}

	SelectUserInCombo(theApp.m_nUserId);

	m_nUserLevel = CSQLUser::GetLevel(theApp.m_nUserId);
	SetPermissions();

	/*// check the permissions to see other users' HW
	m_nUserLevel = CSQLUser::GetLevel(theApp.m_nUserId);
	if(m_nUserLevel < 3)
		m_comboAnalysts.EnableWindow(false);*/

	// Load companies 
	nIndex = m_comboCompany.AddString("- Select Ticker -");
	arIds.RemoveAll();
	CStringArray arTickers;
	CSQLCompany::GetCompanyIDAndTickers(arIds, arTickers, theApp.m_Universe.GetDate());
	for(i=0; i<arIds.GetSize(); i++)
	{
		int nID = arIds[i];
		CString sTicker = arTickers[i];

		nIndex = m_comboCompany.AddString(sTicker);
		m_comboCompany.SetItemData(nIndex, nID);
	}
	m_comboCompany.SetCurSel(0);

	// today's date
	CString sDate = CTime::GetCurrentTime().Format("%m/%d/%Y");
	m_editDate.SetWindowText(sDate);

	// create the Metrics View
	m_pViewMetrics = new CView2x2Metrics();
	m_pViewMetrics->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), this, 778547);
	Resize();

	m_comboTarget.SetCurSel(0);
	m_comboBias.SetCurSel(0);

	m_sliderValue.SetRange(1,5);
	m_sliderValue.SetPos(3);

	m_sliderFund.SetRange(1,5);
	m_sliderFund.SetPos(3);
}

void CViewDailySummary::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewDailySummary::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CWnd* pPlace = GetDlgItem(IDC_STATIC_PLACE_METRICS);
	if(pPlace && m_pViewMetrics)
	{
		CRect rcMetrics;
		pPlace->GetWindowRect(rcMetrics);
		ScreenToClient(rcMetrics);
		rcMetrics.DeflateRect(1,1,1,1);
		
		m_pViewMetrics->MoveWindow(rcMetrics);
	}
}

HBRUSH CViewDailySummary::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CAPTION)
	{
		pDC->SetTextColor(RGB(0, 255, 0));
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		strcpy(lf.lfFaceName,"Courier New");
		lf.lfHeight = (long)16;
		lf.lfWeight = 400;
		CFont font; font.CreateFontIndirect(&lf);
		pDC->SelectObject(&font);


		static CBrush br(RGB(0,0,0));
		return br;

	}

	return hbr;
}

void CViewDailySummary::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(m_pViewMetrics)
		delete m_pViewMetrics;

}

void CViewDailySummary::OnDailysumsSave() 
{
	UpdateData(true);
	
	int nSel;
	int nAnalystID;
	int nCompanyID;
	COleDateTime date;
	double fWeight;
	CString sBias;
	int nCoordValue;
	int nCoordFund;
	int nConfidenceValue;
	int nConfidenceFund; 
	CString sNotesOnValue;
	CString sNotesOnMgmt;
	CString sNotesOnFund;

	// Analyst
	nSel = m_comboAnalysts.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select your name from Analyst drop-down menu. If this is a group decision, select 'The Investment Committee' option.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	nAnalystID = m_comboAnalysts.GetItemData(nSel);

	// Company
	nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the Company from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	nCompanyID = m_comboCompany.GetItemData(nSel);

	// Date
	CString sDate;
	m_editDate.GetWindowText(sDate);
	date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(date.m_status != COleDateTime::valid)
	{
		::MessageBox(m_hWnd, "Please enter a valid date in mm/dd/yyyy format.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	
	// Target weight
	CString sWeight;
	m_comboTarget.GetWindowText(sWeight);
	
	// check for special cases (WIP, QC, Shipping Dock, Truck, Pass)
	if(sWeight == "WIP")				fWeight = -101; else
	if(sWeight == "QC")					fWeight = -102; else
	if(sWeight == "Shipping Dock")		fWeight = -103; else
	if(sWeight == "Truck")				fWeight = -104; else
	if(sWeight == "Pass")				fWeight = -105; 
	else
		fWeight = atof(sWeight);

	/*nSel = m_comboTarget.GetCurSel();
	if(nSel == -1) // the numerical value is typed - actual weight
		fWeight = atof(sWeight);
	else
	{
		//if(nSel == 0)
		//{
		//	::MessageBox(m_hWnd, "Please enter the Target Weight as numerical value (e.g. 3.3%). Otherwise, select one of special items from drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		//	return;
		//}
		fWeight = -100 - nSel;
	}*/

	// Bias
	nSel = m_comboBias.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the 'Bias' from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	m_comboBias.GetLBText(nSel, sBias);

	// 2x2 metrics View
	nCoordValue = (int)m_pViewMetrics->m_fX;
	nCoordFund = (int)m_pViewMetrics->m_fY;
	nConfidenceValue = (int)m_pViewMetrics->m_nSizeX;
	nConfidenceFund = (int)m_pViewMetrics->m_nSizeY;

	// Notes on Valuation
	m_editNotesValuation.GetWindowText(sNotesOnValue);

	// Management 
	if(m_nVision == 0)
	{
		::MessageBox(m_hWnd, "Please score 'Management Vision' using the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	if(m_nExecution == 0)
	{
		::MessageBox(m_hWnd, "Please score 'Management Execution' using the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	m_editNotesManagement.GetWindowText(sNotesOnMgmt);

	// notes on fundamentals
	m_editNotesFundamentals.GetWindowText(sNotesOnFund);

	// escape strings for MySQL
	sNotesOnValue.Replace("'", "\\'");
	sNotesOnMgmt.Replace("'", "\\'");
	sNotesOnFund.Replace("'", "\\'");  

	m_editOneLiner.GetWindowText(m_sOneLiner);
	m_sOneLiner.Replace("'", "\\'");
	
	// call mySql
	m_nID = CSQLDailySum::UpdateDailySum(nCompanyID, nAnalystID, date, fWeight, sBias, nCoordValue, nCoordFund, 
								 nConfidenceValue, nConfidenceFund, sNotesOnValue, sNotesOnFund, 
								 m_nVision, m_nExecution, sNotesOnMgmt, m_bConfirmed, m_fWeight2, m_sBias2, m_sOneLiner);

	if(m_nID > 0)
	{
		::MessageBox(m_hWnd, "The Daily Summary (Homework) has been successfully saved into Database.", "Cornerstone FV Model", MB_ICONINFORMATION);
		return;
	}
	else
	{
		::MessageBox(m_hWnd, "Unable to save Daily Summary (Homework). Please check your network connection and try again.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

}

void CViewDailySummary::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int curpos = pScrollBar->GetScrollPos();

	switch(nSBCode)
	{
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
	case SB_PAGELEFT:
	case SB_PAGERIGHT:
	case SB_ENDSCROLL:

		{
			if((CWnd*)pScrollBar == (CWnd*)&m_sliderValue)
			{
				int nNewPos = m_sliderValue.GetPos();
				m_pViewMetrics->SetConfidenceValue(nNewPos);

			}
			else if((CWnd*)pScrollBar == (CWnd*)&m_sliderFund)
			{
				int nNewPos = m_sliderFund.GetPos();
				m_pViewMetrics->SetConfidenceFund(nNewPos);
			}			
			break;
		}
	
	}

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CViewDailySummary::UpdateView()
{
	int nSel;
	nSel = m_comboAnalysts.GetCurSel();
	if(nSel == 0)
	{
		SetDataToDefaults();
		return;
	}
	int nAnalystID = m_comboAnalysts.GetItemData(nSel);

	// Company
	nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		SetDataToDefaults();
		return;
	}
	int nCompanyID = m_comboCompany.GetItemData(nSel);

	// Date
	CString sDate;
	m_editDate.GetWindowText(sDate);
	COleDateTime date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(date.m_status != COleDateTime::valid)
	{
		SetDataToDefaults();
		return;
	}
	
	// everything is up - query the data
	SDailySum daily;
	BOOL bRes = CSQLDailySum::GetDailySum(nCompanyID, nAnalystID, date, daily);
	if(bRes)
		SetData(daily);
	else
	{
		if(nAnalystID == 9) // special case fot the whole team
		{
			BOOL bRet = SetDataCombineAllAnalysts(nCompanyID, date);
			if(!bRet)
				SetDataToDefaults();
		}
		else
			SetDataToDefaults();
	}
}

void CViewDailySummary::OnSelchangeComboAnalyst() 
{
	UpdateView();
}

void CViewDailySummary::OnSelchangeComboCompany() 
{
	UpdateView();
	SetPermissions();
}

BOOL CViewDailySummary::SetDataCombineAllAnalysts(int nCompanyID, COleDateTime date)
{
	SDailySumArray ar;
	int nCount = CSQLDailySum::GetDailySumsForTickerAndDate(nCompanyID, date, ar);
	if(nCount <= 0)
		return false;

	int nX=0, nY=0, nSizeX=0, nSizeY=0, nVision=0, nExec=0;
	double fWeight=0; int nWeight=0;
	CString sNotesOnValue, sNotesOnFund, sNotesOnMgmt;

	for(int i=0; i<nCount; i++)
	{
		SDailySum *ds = ar[i];
		nX += ds->nCoordValue;
		nY += ds->nCoordFund;
		nSizeX += ds->nConfidenceValue;
		nSizeY += ds->nConfidenceFund;
		nVision += ds->nScoreMgmtVision;
		nExec += ds->nScoreMgmtExec;
		if(ds->fWeight >= 0.0 && ds->sAnalystInitials != "FW")
		{
			fWeight += ds->fWeight;
			nWeight ++;
		}

		CString s = ds->sNotesOnValue;
		int nL = s.FindOneOf("\r\n");
		if(nL != -1)
			s = s.Left(nL+1);

		sNotesOnValue += ds->sAnalystInitials + ":\r\n";
		sNotesOnValue += s;
		sNotesOnValue += "\r\n--------------------------\r\n\r\n";

		s = ds->sNotesOnFund;
		nL = s.FindOneOf("\r\n");
		if(nL != -1)
			s = s.Left(nL+1);

		sNotesOnFund += ds->sAnalystInitials + ":\r\n";
		sNotesOnFund += s;
		sNotesOnFund += "\r\n--------------------------\r\n\r\n";
		
		s = ds->sNotesOnMgmt;
		nL = s.FindOneOf("\r\n");
		if(nL != -1)
			s = s.Left(nL+1);

		sNotesOnMgmt += ds->sAnalystInitials + ":\r\n";
		sNotesOnMgmt += s;
		sNotesOnMgmt += "\r\n--------------------------\r\n\r\n";
	}

	nX /= nCount;
	nY /= nCount;
	nSizeX /= nCount;
	nSizeY /= nCount;
	nVision /= nCount;
	nExec /= nCount;

	// ID is special meaning The Investment Team // D
	m_nID = 9;

	// Weight
	if(nWeight > 0)
	{
		fWeight /= (double)nWeight;
		CString sWeight; sWeight.Format("%g", fWeight);
		m_comboTarget.SetWindowText(sWeight);
	}
	else
		m_comboTarget.SetCurSel(0);

	// Bias
	m_comboBias.SetCurSel(0);
	
	// 2x2 view
	m_pViewMetrics->m_fX = nX;
	m_pViewMetrics->m_fY = nY;
	m_pViewMetrics->m_nSizeX = nSizeX;
	m_pViewMetrics->m_nSizeY = nSizeY;
	m_pViewMetrics->Invalidate(false);

	// Confidence sliders
	m_sliderValue.SetPos(nSizeX);
	m_sliderFund.SetPos(nSizeY);

	// Notes on Valuation
	m_editNotesValuation.SetWindowText(sNotesOnValue);

	// Management Scores
	m_comboVision.SetCurSel(nVision);
	m_comboExecution.SetCurSel(nExec);

	// Notes on management
	m_editNotesManagement.SetWindowText(sNotesOnMgmt);
	
	// Notes on Fundamentals
	m_editNotesFundamentals.SetWindowText(sNotesOnFund);

	for(int i=0; i<nCount; i++)
		delete ar[i];

	return true;
}

void CViewDailySummary::SetData(SDailySum& daily)
{
	m_nID = daily.nID;
	m_bConfirmed = daily.bConfirmed;
	m_fWeight2 = daily.fWeight2;
	m_sBias2 = daily.sBias2;
	
	// Weight
	double fWeight = daily.fWeight;
	if(fWeight <= -100)
		m_comboTarget.SetCurSel(-(100+(int)fWeight));
	else
	{
		CString sWeight;
		sWeight.Format("%g", fWeight);
		m_comboTarget.SetWindowText(sWeight);
	}

	// Bias
	for(int i=0; i<m_comboBias.GetCount(); i++)
	{
		CString s;
		m_comboBias.GetLBText(i, s);
		if(s == daily.sBias)
		{
			m_comboBias.SetCurSel(i);
			break;
		}
	}

	// Confirmed Weight
	if(!m_bConfirmed) 
	{
		GetDlgItem(IDC_STATIC_CONF_WEIGHT)->SetWindowText("N/A");
		GetDlgItem(IDC_STATIC_CONF_BIAS)->SetWindowText("N/A");
	}
	else
	{
		CString sWeight;
		if(m_fWeight2 <= -100)
			m_comboTarget.GetLBText(-(100+(int)m_fWeight2), sWeight);
		else
			sWeight.Format("%g", m_fWeight2);

		GetDlgItem(IDC_STATIC_CONF_WEIGHT)->SetWindowText(sWeight);
		GetDlgItem(IDC_STATIC_CONF_BIAS)->SetWindowText(m_sBias2);

	}

	// 2x2 view
	m_pViewMetrics->m_fX = daily.nCoordValue;
	m_pViewMetrics->m_fY = daily.nCoordFund;
	m_pViewMetrics->m_nSizeX = daily.nConfidenceValue;
	m_pViewMetrics->m_nSizeY = daily.nConfidenceFund;
	m_pViewMetrics->Invalidate(false);

	// Confidence sliders
	m_sliderValue.SetPos(daily.nConfidenceValue);
	m_sliderFund.SetPos(daily.nConfidenceFund);

	// Notes on Valuation
	m_editNotesValuation.SetWindowText(daily.sNotesOnValue);

	// Management Scores
	m_comboVision.SetCurSel(daily.nScoreMgmtVision);
	m_comboExecution.SetCurSel(daily.nScoreMgmtExec);

	// Notes on management
	m_editNotesManagement.SetWindowText(daily.sNotesOnMgmt);
	
	// Notes on Fundamentals
	m_editNotesFundamentals.SetWindowText(daily.sNotesOnFund);

	// One Liner
	m_editOneLiner.SetWindowText(daily.sOneLiner);
}

void CViewDailySummary::SetDataToDefaults()
{
	m_nID = -1;
	
	m_comboTarget.SetCurSel(0);
	m_comboBias.SetCurSel(0);

	GetDlgItem(IDC_STATIC_CONF_WEIGHT)->SetWindowText("N/A");
	GetDlgItem(IDC_STATIC_CONF_BIAS)->SetWindowText("N/A");
	m_bConfirmed = false;
	
	// 2x2 view
	m_pViewMetrics->m_fX = 50;
	m_pViewMetrics->m_fY = 50;
	m_pViewMetrics->m_nSizeX = 3;
	m_pViewMetrics->m_nSizeY = 3;
	m_pViewMetrics->Invalidate(false);

	// Confidence sliders
	m_sliderValue.SetPos(3);
	m_sliderFund.SetPos(3);

	CString s("");
	// Notes on Valuation
	m_editNotesValuation.SetWindowText(s);

	// Management Scores
	m_comboVision.SetCurSel(0);
	m_comboExecution.SetCurSel(0);

	// Notes on management
	m_editNotesManagement.SetWindowText(s);
	
	// Notes on Fundamentals
	m_editNotesFundamentals.SetWindowText(s);

	// One Liner
	m_editOneLiner.SetWindowText(s);
}


void CViewDailySummary::OnKillfocusEditItemdate() 
{
	if(!m_editDate.m_hWnd)
		return;

	UpdateView();	
	SetPermissions();
}

void CViewDailySummary::OnDailysums2minute() 
{
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the Company from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	int nCompany = m_comboCompany.GetItemData(nSel);

	//CDialog2Minute dlg(nCompany);
	CDialog2MinuteV2 dlg;
	dlg.DoModal();	
}

BOOL CViewDailySummary::CreateTemplateFromResource(unsigned int nRes, const char* szFile)
{
	const char* szRName = MAKEINTRESOURCE(nRes);
	HRSRC hrSrc = ::FindResource(theApp.m_hInstance, szRName, "HTM");
	int nnn = ::GetLastError();
	if(!hrSrc)
		return false;

	DWORD nSize = ::SizeofResource(theApp.m_hInstance, hrSrc);
	if(nSize <= 0)
		return false;

	HGLOBAL hGlobal = ::LoadResource(theApp.m_hInstance, hrSrc);
	if(!hGlobal)
		return false;

	char* lpData = (char*)::LockResource(hGlobal);
	if(!lpData)
		return false;

	////////////////////////////////////////////////////////
	// Fill the template
	UpdateData(true);

	// Analyst
	int nSel = m_comboAnalysts.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select your name from Analyst drop-down menu. If this is a group decision, select 'The Investment Committee' option.", "Cornerstone FV Model", MB_ICONSTOP);
		return false;
	}
	CString sAnalyst;
	m_comboAnalysts.GetLBText(nSel, sAnalyst);

	// Company
	nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the Company from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return false;
	}
	CString sCompany;
	m_comboCompany.GetLBText(nSel, sCompany);

	// Date
	CString sDate;
	m_editDate.GetWindowText(sDate);

	// Target
	CString sWeight;
	m_comboTarget.GetWindowText(sWeight);
	nSel = m_comboTarget.GetCurSel();
	if(nSel > 0) // the numerical value is typed - actual weight
		m_comboTarget.GetLBText(nSel, sWeight);

	// Bias
	CString sBias;
	nSel = m_comboBias.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the 'Bias' from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return false;
	}
	m_comboBias.GetLBText(nSel, sBias);

	char sz[] = { (char)13, '\0' };

	// Notes on Valuation
	CString sNotesOnValue;
	m_editNotesValuation.GetWindowText(sNotesOnValue);
	sNotesOnValue.Replace(sz, "<br>");

	// Notes on Fundamentals
	CString sNotesOnFund;
	m_editNotesFundamentals.GetWindowText(sNotesOnFund);
	sNotesOnFund.Replace(sz, "<br>");

	// Notes on Management
	CString sNotesOnMgmt;
	m_editNotesManagement.GetWindowText(sNotesOnMgmt);
	sNotesOnMgmt.Replace(sz, "<br>");

	// Vision & Execution
	CString sVision, sExec;
	if(m_nVision == 0)
	{
		::MessageBox(m_hWnd, "Please score 'Management Vision' using the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return false;
	}

	if(m_nExecution == 0)
	{
		::MessageBox(m_hWnd, "Please score 'Management Execution' using the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return false;
	}
	sVision.Format("%u", m_nVision);
	sExec.Format("%u", m_nExecution);

	// Coordinates
	int x = (int)(347.0*m_pViewMetrics->m_fX/100.0) + 50 - 25;
	int y = (int)(247.0*m_pViewMetrics->m_fY/100.0) + 50 - 25;
	CString sX; sX.Format("%u", x);
	CString sY; sY.Format("%u", y);
	
	// Format
	CString sFile;
	sFile.Format(lpData, sAnalyst, sCompany, sDate, 
			"", sWeight, sBias, sX, sY, 
			sNotesOnValue, 
			sNotesOnFund,
			sVision, sExec,
			sNotesOnMgmt);


	CString sDir = theApp.CreateTempDirectory();	
	CString s; s.Format("%s_%s.html", sCompany, sAnalyst);
	CString sPath = sDir + s;

	///////////////////////////////////////////////////////
	CFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite))
		return false;
	
	file.Write(sFile, sFile.GetLength());
	file.Close();

	CString sExecute = "chrome.exe";
	//sExecute += szFile;
	::ShellExecute(m_hWnd, "open", szFile, NULL, NULL, SW_SHOW);
	
	::UnlockResource(hGlobal);
	::FreeResource(hGlobal);

	return true;
}

void CViewDailySummary::OnDailysumsPrint() 
{
	CreateTemplateFromResource(IDR_HTML_HW, "homework.htm");
}

void CViewDailySummary::OnDailysumsUnlock() 
{
	// Access Level
	if(m_nUserLevel < 3)
	{
		MessageBox("You have no permission to unlock Daily Summary. This action can be performed only by CIO level access.", "Cornerstone Investment Partners", MB_ICONSTOP);
		return;
	}

	// Company
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the Company from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	int nCompanyID = m_comboCompany.GetItemData(nSel);

	// Date
	CString sDate;
	m_editDate.GetWindowText(sDate);
	COleDateTime date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(date.m_status != COleDateTime::valid)
	{
		::MessageBox(m_hWnd, "Please enter a valid date in mm/dd/yyyy format.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	int nLockId = CSQLDailySum::IsUnlocked(nCompanyID, date);
	if(nLockId != 0)
	{
		MessageBox("The Daily Summary is already unlocked and all users can see it.", "Cornerstone Investment Partners", MB_ICONINFORMATION);
		return;
	}

	nLockId = CSQLDailySum::Unlock(nCompanyID, date);
	if(nLockId == 0)
	{
		MessageBox("Unable to unlock the Daily Summary. An error occured. Please try again later.", "Cornerstone Investment Partners", MB_ICONSTOP);
		return;
	}

	MessageBox("The Daily Summary has been successfully unlocked and all users can now see it.", "Cornerstone Investment Partners", MB_ICONINFORMATION);
	return;
}

void CViewDailySummary::OnDailysumsLock() 
{
	if(m_nUserLevel < 3)
	{
		MessageBox("You have no permission to unlock Daily Summaries. This action can be performed only by CIO level access.", "Cornerstone Investment Partners", MB_ICONSTOP);
		return;
	}	

	// Company
	int nSel = m_comboCompany.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox(m_hWnd, "Please select the Company from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}
	int nCompanyID = m_comboCompany.GetItemData(nSel);

	// Date
	CString sDate;
	m_editDate.GetWindowText(sDate);
	COleDateTime date = CSQLCompany::ConvertUSToOleDate(sDate);
	if(date.m_status != COleDateTime::valid)
	{
		::MessageBox(m_hWnd, "Please enter a valid date in mm/dd/yyyy format.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	int nLockId = CSQLDailySum::IsUnlocked(nCompanyID, date);
	if(nLockId == 0)
	{
		MessageBox("The Daily Summary is already locked and only CIO level users can see it.", "Cornerstone Investment Partners", MB_ICONINFORMATION);
		return;
	}

	nLockId = CSQLDailySum::Lock(nCompanyID, date);
	if(nLockId == 0)
	{
		MessageBox("Unable to lock the Daily Summary. An error occured. Please try again later.", "Cornerstone Investment Partners", MB_ICONSTOP);
		return;
	}

	MessageBox("The Daily Summary has been successfully locked and only CIO level users can now see it.", "Cornerstone Investment Partners", MB_ICONINFORMATION);
	return;
}

void CViewDailySummary::OnConfirm() 
{
	double fWeight;
	CString sBias;

	if(m_bConfirmed) 
	{
		fWeight = m_fWeight2;
		sBias = m_sBias2;
	}
	else
	{
		// Target weight
		CString sWeight;
		m_comboTarget.GetWindowText(sWeight);
		int nSel = m_comboTarget.GetCurSel();
		if(nSel == -1) // the numerical value is typed - actual weight
			fWeight = atof(sWeight);
		else
		{
			//if(nSel == 0)
			//{
			//	::MessageBox(m_hWnd, "Please enter the Target Weight as numerical value (e.g. 3.3%). Otherwise, select one of special items from drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
			//	return;
			//}
			fWeight = -100 - nSel;
		}

		// Bias
		nSel = m_comboBias.GetCurSel();
		if(nSel == 0)
		{
			::MessageBox(m_hWnd, "Please select the 'Bias' from the drop-down menu.", "Cornerstone FV Model", MB_ICONSTOP);
			return;
		}
		m_comboBias.GetLBText(nSel, sBias);
	}

	CDialogConfirmWeight dlg;
	dlg.m_fWeight = fWeight;
	dlg.m_sBias = sBias;

	if(dlg.DoModal() == IDOK)
	{
		m_bConfirmed = true;
		m_fWeight2 = dlg.m_fWeight;
		m_sBias2 = dlg.m_sBias;

		GetDlgItem(IDC_STATIC_CONF_WEIGHT)->SetWindowText(dlg.m_sWeight);
		GetDlgItem(IDC_STATIC_CONF_BIAS)->SetWindowText(m_sBias2);
	}
}
