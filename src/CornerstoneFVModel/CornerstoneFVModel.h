// CornerstoneFVModel.h : main header file for the CORNERSTONEFVMODEL application
//

#if !defined(AFX_CORNERSTONEFVMODEL_H__C706DBB8_2CFD_4C60_896D_88D9A299B1E2__INCLUDED_)
#define AFX_CORNERSTONEFVMODEL_H__C706DBB8_2CFD_4C60_896D_88D9A299B1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "universe.h"
#include "sqlcompany.h"
#include "enginemsexcel.h"

#include "report.h"

/////////////////////////////////////////////////////////////////////////////
// CApp:
// See CornerstoneFVModel.cpp for the implementation of this class
//

class CViewCharts;
class CViewMeetings;
class CViewTable;
class CViewChanges;
class CViewHTML;
class CViewMap;
class CViewMapSectors;
class CViewDailySummary;

class CDialogStocksAlphabetical;
struct SReport;
class CApp : public CWinApp
{
public:
	CApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// custom functions
	void NewViewGeneral(CCompany* pCompany);
	void NewViewMonteCarlo(CCompany* pCompany);
	void NewViewHistory(CCompany* pCompany);
	CViewCharts*	NewViewCharts();
	CViewMeetings*	NewViewMeetings();
	CViewTable*		NewViewTable();
	CViewChanges*	NewViewChanges();
	CViewHTML*		NewViewHTML();
	CViewMap*		NewViewMap();
	CViewMapSectors*	NewViewMapSectors();
	CViewDailySummary*	NewViewSummaries();

	// Main Universe Data
	CUniverse	m_Universe;
	int			m_nDatabase;
	int			m_nUniverse;

	int			m_nUserId;

	// Current Year for EPS, ROC and ROE (-1 is default and means last Annual Report year+1)
	int			m_nCurrentYear;

	SReportFilter	m_RepFilter;

	// Document templates
	CMultiDocTemplate*	m_pTemplateGeneral;
	CMultiDocTemplate*	m_pTemplateMonteCarlo;
	CMultiDocTemplate*	m_pTemplateHistory;
	CMultiDocTemplate*	m_pTemplateCharts;
	CMultiDocTemplate*	m_pTemplateMeetings;
	CMultiDocTemplate*	m_pTemplateTable;
	CMultiDocTemplate*	m_pTemplateChanges;
	CMultiDocTemplate*	m_pTemplateHTML;
	CMultiDocTemplate*	m_pTemplateMap;
	CMultiDocTemplate*	m_pTemplateMapSectors;
	CMultiDocTemplate*	m_pTemplateSummary;

	CDialogStocksAlphabetical*		m_pDialogStocks;

	////////////////////////////////////////////////////////////////////////////////////////////
	CString CreateTempDirectory();

	BOOL CreateReportHTML_1(const char* szFile, SReport* pRep);
	BOOL CreateReportHTML_2(const char* szFile, SReport* pRep);
	BOOL CreateReportHTML_3(const char* szFile, SReport* pRep);
	BOOL CreateReportHTML_4(const char* szFile, SReport* pRep, SReportFilter& filter);

	BOOL CreateReportExcel_1(const char* szFile, SReport* pRep);
	BOOL CreateReportExcel_2(const char* szFile, SReport* pRep);
	BOOL CreateReportExcel_3(const char* szFile, SReport* pRep);
	BOOL CreateReportExcel_4(const char* szFile, SReport* pRep, SReportFilter& filter);


// Implementation
	COleTemplateServer m_server;
	//{{AFX_MSG(CApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileExportValues();
	afx_msg void OnAppLastEpsYear();
	afx_msg void OnAppMacroparams();
	afx_msg void OnAppUploadData();
	afx_msg void OnAppCompanies();
	afx_msg void OnAppSectors();
	afx_msg void OnAppIndustries();
	afx_msg void OnAppUploadHistory();
	afx_msg void OnAppAddToHistory();
	afx_msg void OnAppDeleteHistoryItem();
	afx_msg void OnAppCharts();
	afx_msg void OnAppUploadWeights();
	afx_msg void OnAppPriceDate();
	afx_msg void OnAppMeetings();
	afx_msg void OnAppCheckSplits();
	afx_msg void OnAppCheckData();
	afx_msg void OnAppReportEbitdaLeverage();
	afx_msg void OnAppMap();
	afx_msg void OnAppReportEarningsMomentum();
	afx_msg void OnAppReportValueMomentum();
	afx_msg void OnAppReportGeneral();
	afx_msg void OnConnectServers();
	afx_msg void OnAppMapsectors();
	afx_msg void OnAppSelectUniverse();
	afx_msg void OnAppSummaries();
	afx_msg void OnAppChangePassword();
	afx_msg void OnAppInvprocess();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CORNERSTONEFVMODEL_H__C706DBB8_2CFD_4C60_896D_88D9A299B1E2__INCLUDED_)
