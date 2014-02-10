// ViewMap.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "ViewMap.h"

#include "ViewGraph.h"
#include "ViewCharts.h"
#include "DialogDate.h"
#include "DialogMapStep.h"

#include "Company.h"
#include "DataManager.h"
#include "History.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewMap

IMPLEMENT_DYNCREATE(CViewMap, CFormView)

CViewMap::CViewMap()
	: CFormView(CViewMap::IDD)
{
	//{{AFX_DATA_INIT(CViewMap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ColorTableArray.Add(RGB(0,0,192));
	m_ColorTableArray.Add(RGB(0,192,0));		
	m_ColorTableArray.Add(RGB(192,0,0));

	m_ColorTableArray.Add(RGB(0,192,192));		
	m_ColorTableArray.Add(RGB(192,192,0));		
	m_ColorTableArray.Add(RGB(192,0,192));

	m_ColorTableArray.Add(RGB(0,0,0xff));
	m_ColorTableArray.Add(RGB(0,0xff,0));		
	m_ColorTableArray.Add(RGB(0xff,0,0));
	
	m_ColorTableArray.Add(RGB(0,0,128));		
	m_ColorTableArray.Add(RGB(0,128,0));		
	m_ColorTableArray.Add(RGB(128,0,0));

	m_ColorTableArray.Add(RGB(0xff,128,0));	
	m_ColorTableArray.Add(RGB(0xff,0,128));	
	m_ColorTableArray.Add(RGB(0xff,128,128));

	m_ColorTableArray.Add(RGB(192,0,0xff));	
	m_ColorTableArray.Add(RGB(0,192,0xff));	
	m_ColorTableArray.Add(RGB(192,192,0xff));
	
	m_ColorTableArray.Add(RGB(192,0xff,0));	
	m_ColorTableArray.Add(RGB(0,0xff,192));	
	m_ColorTableArray.Add(RGB(192,0xff,192));
	
	m_ColorTableArray.Add(RGB(0xff,192,0));	
	m_ColorTableArray.Add(RGB(0xff,0,192));	
	m_ColorTableArray.Add(RGB(0xff,192,192));
	
	m_ColorTableArray.Add(RGB(192,0xff,0xff));	
	m_ColorTableArray.Add(RGB(0xff,0xff,192));	
	m_ColorTableArray.Add(RGB(0xff,192,0xff));

	m_From.SetStatus(COleDateTime::invalid);
	m_To.SetStatus(COleDateTime::invalid);

	m_pGraph = NULL;
	m_nStep = 50;
}

CViewMap::~CViewMap()
{
}

void CViewMap::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewMap)
	DDX_Control(pDX, IDC_EDIT_SYMBOLS, m_editSymbols);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewMap, CFormView)
	//{{AFX_MSG_MAP(CViewMap)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_MAP_FROM, OnMapFrom)
	ON_COMMAND(ID_MAP_END, OnMapEnd)
	ON_COMMAND(ID_MAP_HISTORY, OnMapHistory)
	ON_COMMAND(ID_MAP_GRAPH, OnMapGraph)
	ON_COMMAND(ID_MAP_STEP, OnMapStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewMap diagnostics

#ifdef _DEBUG
void CViewMap::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewMap::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewMap message handlers

void CViewMap::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
	SetScrollSizes(MM_TEXT, CSize(0,0));

	m_pGraph = new CViewGraph();
	m_pGraph->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, 23457);
	m_pGraph->ShowWindow(SW_SHOW);

	m_pGraph->m_sLabelX = "Normalized Rank";
	m_pGraph->m_sLabelY = "Razor Rank";

	m_pGraph->AddRect(1,1,400,400,RGB(181,230,29));
	m_pGraph->AddRect(400,400,800,800,RGB(255,174,201));
	
	m_pGraph->Invalidate(false);
	Resize();
}

void CViewMap::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(m_pGraph)
		m_pGraph->DestroyWindow();
}

void CViewMap::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();		
}

void CViewMap::Resize()
{
	CRect rc;
	GetClientRect(rc);

	if(m_pGraph)
		m_pGraph->MoveWindow(5,5,rc.right-10, rc.bottom-10-20-15);

	int nW = rc.right/4;
	if(GetDlgItem(IDC_STATIC_TOKENS))
		GetDlgItem(IDC_STATIC_TOKENS)->MoveWindow(rc.right/2-nW-52, rc.bottom-10-17, 50, 20);

	if(m_editSymbols)
		m_editSymbols.MoveWindow(rc.right/2-nW, rc.bottom-10-20, 2*nW, 23);

}

void CViewMap::SetupMap(CCompany* pCompany)
{
	int nItem1 = CDataManager::GetColIndexFromName("NEPVRnk");
	int nItem2 = CDataManager::GetColIndexFromName("REPVRnk");
	CHistory* pHist = pCompany->GetHistory();
	if(!pHist->IsItemAllLoaded(nItem1))
		pHist->LoadItem(pCompany->GetTicker(), "NEPVRnk");
	if(!pHist->IsItemAllLoaded(nItem2))
		pHist->LoadItem(pCompany->GetTicker(), "REPVRnk");

	int nStart, nEnd;
	pHist->GetStartEndIndexesForPeriod(m_From, m_To, nStart, nEnd);
	if(nStart == -1 || nEnd == -1)
	{
		CString sMessage; sMessage.Format("No Valid History exists for specified dates for Ticker: '%s'", pCompany->GetTicker());
		AfxMessageBox(sMessage);
		return;
	}

	double xPrev = _INF_;
	double yPrev = _INF_;
	
	int nInd = 0;
	int nSize = nStart - nEnd + 1;

	double *x = new double[nSize];
	double *y = new double[nSize];
	for(int i=nEnd; i<=nStart; i++)
	{
		double fx = pHist->GetItemPtr(i)->data[nItem1];
		double fy = pHist->GetItemPtr(i)->data[nItem2];

		if( (xPrev >= _INF_ || fabs(fx - xPrev) > m_nStep) || (yPrev >= _INF_ || fabs(fy - yPrev) > m_nStep) )
		{
			x[nInd] = fx;
			y[nInd] = fy;
			
			xPrev = fx;
			yPrev = fy;

			++nInd;
		}
	}

	int nIndex = m_pGraph->m_arSeries.GetSize();
	int colorIndex = nIndex % m_ColorTableArray.GetSize();
	COLORREF color = m_ColorTableArray[colorIndex];

	m_pGraph->AddSeries(pCompany->GetTicker(), x, y, nInd, SERIES_TYPE_PT_LINE_1, 1, PS_SOLID, color);

	delete [] x;
	delete [] y;
}

BOOL CViewMap::PreTranslateMessage(MSG* pMsg) 
{
	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocus = GetFocus();
		if(	pFocus && ::IsWindow(pFocus->m_hWnd) && 
			(pFocus->GetDlgCtrlID() == IDC_EDIT_SYMBOLS) )
		{
			CString sTokens;
			m_editSymbols.GetWindowText(sTokens); 

			SetMaps(sTokens);
		}
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CViewMap::SetMaps(const char* tokens)
{
	m_arCompanies.RemoveAll();
	m_pGraph->RemoveAll();

	char szToken[32];
	const char* sz = tokens;
	const char* sz0 = sz;
	int nIndex = 0;
	while(*sz != '\0')
	{
		if(*sz == ' ')
		{
			strncpy(szToken, sz0, (size_t)(sz-sz0));
			szToken[sz-sz0] = '\0';

			if(strlen(szToken) > 0)
			{
				int nIndex = theApp.m_Universe.GetIndexByTicker(szToken);
				if(nIndex == -1)
				{
					CString sMessage; sMessage.Format("Unknown Company: '%s'", szToken);
					AfxMessageBox(sMessage, MB_ICONINFORMATION);
				}
				else
				{
					CCompany* pCompany = theApp.m_Universe.GetCompanyByTicker(nIndex);
					SetupMap(pCompany);
					m_arCompanies.Add(pCompany);
				}
			}
			
			while(*sz != '\0' && *sz == ' ')
				++sz;
			sz0 = sz;
		}
		++sz;
	}
	
	if(sz0 < sz)
	{
		strncpy(szToken, sz0, (size_t)(sz-sz0));
		szToken[sz-sz0] = '\0';

		if(strlen(szToken) > 0)
		{
			int nIndex = theApp.m_Universe.GetIndexByTicker(szToken);
			if(nIndex == -1)
			{
				CString sMessage; sMessage.Format("Unknown Company: '%s'", szToken);
				AfxMessageBox(sMessage, MB_ICONINFORMATION);
			}
			else
			{
				CCompany* pCompany = theApp.m_Universe.GetCompanyByTicker(nIndex);
				SetupMap(pCompany);
				m_arCompanies.Add(pCompany);
			}
		}
	} 

	m_pGraph->AddRect(1,1,400,400,RGB(181,230,29));
	m_pGraph->AddRect(400,400,800,800,RGB(255,174,201));

	m_pGraph->Invalidate();
	m_pGraph->RedrawWindow();
}

void CViewMap::OnMapFrom() 
{
	CDialogDate dlg;
	dlg.m_Date = m_From;
	if(dlg.DoModal() != IDOK)
		return;

	m_From = dlg.m_Date;

	CString sTokens;
	m_editSymbols.GetWindowText(sTokens); 

	SetMaps(sTokens);
}

void CViewMap::OnMapEnd() 
{
	CDialogDate dlg;
	dlg.m_Date = m_From;
	if(dlg.DoModal() != IDOK)
		return;

	m_To = dlg.m_Date;

	CString sTokens;
	m_editSymbols.GetWindowText(sTokens); 

	SetMaps(sTokens);
}

void CViewMap::OnMapHistory() 
{
	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];
		theApp.NewViewHistory(pCompany);
	} 
}

void CViewMap::OnMapGraph() 
{
	CString sTokens;
	m_editSymbols.GetWindowText(sTokens); 

	CViewCharts* pView = theApp.NewViewCharts();
	pView->SetCharts(sTokens, "", true);
}

void CViewMap::OnMapStep() 
{
	CDialogMapStep dlg;
	dlg.m_nStep = m_nStep;
	if(dlg.DoModal() != IDOK)
		return;

	m_nStep = dlg.m_nStep;

	CString sTokens;
	m_editSymbols.GetWindowText(sTokens); 

	SetMaps(sTokens);
}
