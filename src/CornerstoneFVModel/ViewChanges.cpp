// ViewTable.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewChanges.h"

#include "WndGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewChanges

IMPLEMENT_DYNCREATE(CViewChanges, CFormView)

CViewChanges::CViewChanges()
	: CFormView(CViewChanges::IDD)
{
	//{{AFX_DATA_INIT(CViewChanges)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
}

CViewChanges::~CViewChanges()
{
}

void CViewChanges::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewChanges)
	DDX_Control(pDX, IDC_COMBO_SORT, m_comboSort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewChanges, CFormView)
	//{{AFX_MSG_MAP(CViewChanges)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_CHANGES_COPY, OnChangesCopy)
	ON_COMMAND(ID_CHANGES_EXCEL, OnChangesExcel)
	ON_COMMAND(ID_CHANGES_OPEN, OnChangesOpen)
	ON_COMMAND(ID_CHANGES_PRINT, OnChangesPrint)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT, OnSelchangeComboSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewChanges diagnostics

#ifdef _DEBUG
void CViewChanges::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewChanges::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewChanges message handlers
void CViewChanges::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	SetScrollSizes(MM_TEXT, CSize(0,0));
	
	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName,"Ms Sans Serif");
	lf.lfHeight = (long)13;
	lf.lfWeight = 400;
	m_Font.CreateFontIndirect(&lf);

	// GRID
	m_pGrid = new CWndGrid();
	m_pGrid->SetEnableScrollBars(true);
	m_pGrid->SetDefaultColumnSize(90);
	m_pGrid->SetDefaultRowSize(21);
	m_pGrid->SetTextAlign(DT_RIGHT);

	m_pGrid->Create(this);

	m_pGrid->SetColumnCount(8);
	m_pGrid->SetItemText(0, 0, "Company");
	m_pGrid->SetItemText(0, 1, "NEPV Rank");
	m_pGrid->SetItemText(0, 2, "REPV Rank");
	m_pGrid->SetItemText(0, 3, "BEV Rank");
	m_pGrid->SetItemText(0, 4, "Price (%)");
	m_pGrid->SetItemText(0, 5, "EPS Next (%)");
	m_pGrid->SetItemText(0, 6, "EPS This (%)");
	m_pGrid->SetItemText(0, 7, "Debt Ratio (%)");
	
	m_pGrid->SetRowCount(1);

	m_pGrid->SetColumnSize(0, 75);
	m_pGrid->SetCellsFont(m_Font);

	m_pGrid->SetFixedColumnCount(1);
	m_pGrid->SetFixedRowCount(1);
	
	//m_pGrid->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGrid->SetDefaultColorGridNonFixed(RGB(255,255,255));
	m_pGrid->SetFixedCellsBkColor(RGB(200,200,200));

	Resize();
}
void CViewChanges::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewChanges::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcGrid(10, 34, rc.right-10, rc.bottom-10-15-10);

	if(GetDlgItem(IDC_STATIC_NAME))
		GetDlgItem(IDC_STATIC_NAME)->MoveWindow(10, 8, rc.Width()-20, 18);

	if(m_pGrid)
		m_pGrid->MoveWindow(rcGrid);

	int nW = rc.right/4;
	if(GetDlgItem(IDC_STATIC_SORT))
		GetDlgItem(IDC_STATIC_SORT)->MoveWindow(10, rc.bottom-5-17, 40, 20);

	if(m_comboSort)
		m_comboSort.MoveWindow(10+40+3, rc.bottom-5-20, 100, 300);
}

void CViewChanges::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(m_pGrid)
		m_pGrid->DestroyWindow();
}

HBRUSH CViewChanges::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_NAME)
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

void CViewChanges::SetData(const CArray<CCompany*,CCompany*>& arCompanies, const CArray<SChangeCriteria,SChangeCriteria>& arChanges)
{
	m_arCompanies.RemoveAll();
	m_arChanges.RemoveAll();

	for(int i=0; i<arCompanies.GetSize(); i++)
	{
		m_arCompanies.Add(arCompanies[i]);
		m_arChanges.Add(arChanges[i]);
	}

	SetupGrid();
}

void CViewChanges::SetupGrid()
{
	CString sTitle = "Recent Significant Changes";

	int nRows = m_arCompanies.GetSize();
	if(nRows > 1)
	{
		CString s; s.Format(": %u Companies", nRows);
		sTitle += s;
	}

	m_pGrid->SetRowCount(nRows+1);
	m_pGrid->SetCellsFont(m_Font);

	for(int i=0; i<nRows; i++)
	{
		CCompany* pCompany = m_arCompanies[i];
		SChangeCriteria ch = m_arChanges[i];		

		m_pGrid->SetItemText(i+1, 0, pCompany->GetTicker());

		CString s; s.Format("%u", ch.nDeltaRankN);
		m_pGrid->SetItemText(i+1, 1, s);

		s.Format("%u", ch.nDeltaRankR);
		m_pGrid->SetItemText(i+1, 2, s);

		s.Format("%u", ch.nDeltaBEV);
		m_pGrid->SetItemText(i+1, 3, s);

		s.Format("%.2f", ch.fDeltaPrice*100.0);
		m_pGrid->SetItemText(i+1, 4, s);

		s.Format("%.2f", ch.fDeltaEPSNext*100.0);
		m_pGrid->SetItemText(i+1, 5, s);
	
		s.Format("%.2f", ch.fDeltaEPSThis*100.0);
		m_pGrid->SetItemText(i+1, 6, s);		

		s.Format("%.2f", ch.fDeltaDebt*100.0);
		m_pGrid->SetItemText(i+1, 7, s);		

		if(i % 2 == 0)
			m_pGrid->SetRowBkColor(i+2, RGB(226,237,250));
		else
			m_pGrid->SetRowBkColor(i+2, RGB(255,255,255));
	}

	m_comboSort.SetCurSel(0);
	m_pGrid->SortByColumn(0, false);

	if(m_arChanges.GetSize() > 0)
	{
		CString sPeriods;
		sPeriods.Format(", Screening Period: %u week(s)", m_arChanges[0].nPeriods);
		sTitle += sPeriods;
	}

	GetDocument()->SetTitle(sTitle);
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(sTitle);
}

void CViewChanges::OnChangesCopy() 
{
	m_pGrid->CopySelectionToClipboard();
}

void CViewChanges::OnChangesExcel() 
{
	CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("Changes_%s.xls", sTime);
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";
	CFileDialog fileDlg (false, "xls", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

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

	for(int nRow = 0; nRow<m_pGrid->GetRowCount(); nRow++)
	{
		CString sLine;
		for(int nCol=0; nCol<m_pGrid->GetColumnCount(); nCol++)
			sLine += m_pGrid->GetItemText(nRow, nCol) + "\t";
		sLine += "\n";
		file.WriteString(sLine);
	}

	file.Close();
	ShellExecute(m_hWnd, "open", sName, NULL, NULL, SW_SHOW);
}

void CViewChanges::OnChangesOpen() 
{
	int nR1, nS1, nR2, nS2;
	m_pGrid->GetSelectionRange(nR1, nS1, nR2, nS2);
	int nSel = nR1;
	if(nSel == -1)
	{
		AfxMessageBox("Please select a company.", MB_ICONINFORMATION);
		return;
	}
	
	CString sTicker = m_pGrid->GetItemText(nSel, 0);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker);
	if(nIndex == -1)
	{
		AfxMessageBox("Invalid Selection. Please try again.", MB_ICONINFORMATION);
		return;
	}

	CCompany* pCompany = theApp.m_Universe.GetCompany(nIndex);
	theApp.NewViewGeneral(pCompany);
}

void CViewChanges::OnChangesPrint() 
{
		
}

void CViewChanges::OnSelchangeComboSort() 
{
	int nSel = m_comboSort.GetCurSel();
	m_pGrid->SortByColumn(nSel, false);	

	for(int i=1; i<m_pGrid->GetRowCount(); i++)
	{
		if(i % 2 == 0)
			m_pGrid->SetRowBkColor(i, RGB(226,237,250));
		else
			m_pGrid->SetRowBkColor(i, RGB(255,255,255));
	}
}
