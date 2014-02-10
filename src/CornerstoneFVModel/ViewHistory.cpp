// ViewHistory.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewHistory.h"

#include "DialogDate.h"
#include "WndGrid.h"
#include "Company.h"
#include "ViewCharts.h"
#include "ViewHTML.h"

#include "History.h"
#include "DataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewHistory

IMPLEMENT_DYNCREATE(CViewHistory, CFormView)

CViewHistory::CViewHistory()
	: CFormView(CViewHistory::IDD)
{
	//{{AFX_DATA_INIT(CViewHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCompany = NULL;
	m_pGrid = NULL;
}

CViewHistory::~CViewHistory()
{
}

void CViewHistory::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewHistory)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

static int format[] = {-1, 1,1, 1,2,2, 0,0,0,0,0, 1,1, 0,0, 1,1, 0,0, 1,0,1,0,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void CViewHistory::PopulateGrid()
{
	if(!m_pCompany)
		return;

	CHistory* pHist = m_pCompany->GetHistory();
	if(!pHist)
		return;

	m_pGrid->SetRowCount(pHist->GetSize()+2);
	m_pGrid->SetCellsFont(m_Font);
	
	CString s;

	// Forecast Row
	m_pGrid->SetItemText(1, 0, "Forecast");
	
	s.Format("%.2f", pHist->GetRetFore());
	m_pGrid->SetItemText(1, 1, s);

	s.Format("%.2f", pHist->GetPriceFore());
	m_pGrid->SetItemText(1, 2, s);

	s.Format("%.2f", pHist->GetMAFore());
	m_pGrid->SetItemText(1, 3, s);

	s.Format("%.3f", pHist->GetPMFore());
	m_pGrid->SetItemText(1, 4, s);

	s.Format("%.3f", pHist->GetMMFore());
	m_pGrid->SetItemText(1, 5, s);

	// All others
	for(int nRow=0; nRow<pHist->GetSize(); nRow++)
	{
		SHistoryItem* pItem = pHist->GetItemPtr(nRow);

		// Set Date
		CString sDate = pItem->date.Format("%m/%d/%Y");
		m_pGrid->SetItemText(nRow+2, 0, sDate);

		// Data
		for(int nCol=1; nCol<NUM_COLUMNS_IN_HISTORY_DB+4; nCol++)
		{
			int index = CDataManager::GetColIndex(nCol);
			if(pItem->data[index] < (double)_INF_)
			{
				if(format[nCol] == 1)
					s.Format("%.2f", pItem->data[index]);
				else if(format[nCol] == 2)
					s.Format("%.3f", pItem->data[index]);
				else
					s.Format("%i", (int)pItem->data[index]);
			}
			else
				s = "n/a";

			m_pGrid->SetItemText(nRow+2, nCol, s);
		}

		if(nRow % 2 == 0)
			m_pGrid->SetRowBkColor(nRow+2, RGB(226,237,250));
	}
}

void CViewHistory::SetCompany(CCompany* pCompany)
{
	m_pCompany = pCompany;

	// caption
	CString sCaption;
	sCaption.Format("[%s] %s, %s, %s", pCompany->GetTicker(), pCompany->GetName(), pCompany->GetIndustry(), pCompany->GetSector());
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(sCaption);

	COleDateTime nullDate; nullDate.SetStatus(COleDateTime::invalid);
	if(!m_pCompany->LoadHistory(nullDate, nullDate))
		return;

	PopulateGrid();
}


BEGIN_MESSAGE_MAP(CViewHistory, CFormView)
	//{{AFX_MSG_MAP(CViewHistory)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_HISTORY_DELETE, OnHistoryDelete)
	ON_COMMAND(ID_HISTORY_EXCEL, OnHistoryExcel)
	ON_COMMAND(ID_HISTORY_GOTO, OnHistoryGoto)
	ON_COMMAND(ID_HISTORY_GRAPH, OnHistoryGraph)
	ON_COMMAND(ID_HISTORY_SAVE, OnHistorySave)
	ON_COMMAND(ID_HISTORY_ADD_TO_HISTORY, OnHistoryAddToHistory)
	ON_COMMAND(ID_HISTORY_COPY, OnHistoryCopy)
	ON_COMMAND(ID_HISTORY_PASTE, OnHistoryPaste)
	ON_COMMAND(ID_HISTORY_PRINT, OnHistoryPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewHistory diagnostics

#ifdef _DEBUG
void CViewHistory::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewHistory::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewHistory message handlers

void CViewHistory::OnInitialUpdate() 
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
	m_pGrid->SetDefaultColumnSize(60);
	m_pGrid->SetDefaultRowSize(21);
	m_pGrid->SetTextAlign(DT_RIGHT);

	m_pGrid->Create(this);

	m_pGrid->SetColumnCount(NUM_COLUMNS_IN_HISTORY_DB+4);
	for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+4; i++)
		m_pGrid->SetItemText(0, i, CDataManager::GetColName(i));

	m_pGrid->SetRowCount(10);

	m_pGrid->SetColumnSize(0, 70);
	m_pGrid->SetCellsFont(m_Font);

	m_pGrid->SetFixedColumnCount(1);
	m_pGrid->SetFixedRowCount(1);
	
	//m_pGrid->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGrid->SetDefaultColorGridNonFixed(RGB(255,255,255));
	m_pGrid->SetFixedCellsBkColor(RGB(200,200,200));

	Resize();
}

void CViewHistory::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewHistory::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcGrid(10, 34, rc.right-10, rc.bottom-10);

	if(GetDlgItem(IDC_STATIC_NAME))
		GetDlgItem(IDC_STATIC_NAME)->MoveWindow(10, 8, rc.Width()-20, 18);

	if(m_pGrid)
		m_pGrid->MoveWindow(rcGrid);
}

void CViewHistory::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(m_pGrid)
		m_pGrid->DestroyWindow();
}

HBRUSH CViewHistory::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CViewHistory::OnHistoryDelete() 
{
	if(!m_pCompany)
		return;

	CHistory* pHist = m_pCompany->GetHistory();
	if(!pHist)
		return;

	CDialogDate dlg;
	if(dlg.DoModal() != IDOK)
		return;
	
	CString s; s.Format("Are you sure you want to permanently delete the record for '%s'?", dlg.m_Date.Format("%m/%d/%Y"));
	int nResult = AfxMessageBox(s, MB_YESNO);
	if(nResult != IDYES)
		return;

	int nIndex = pHist->GetIndexByDate(dlg.m_Date);
	if(nIndex == -1)
	{
		AfxMessageBox("There is no history record for this date.  Please try again.");
		return;
	}

	if(!CSQLCompany::DeleteHistoryItem(m_pCompany->GetTicker(), dlg.m_Date))
	{
		AfxMessageBox("Unable to delete the history record.  Please try again.");
		return;
	}

	pHist->DeleteItem(nIndex);
	PopulateGrid();
}

void CViewHistory::OnHistoryExcel() 
{
	if(!m_pCompany)
		return;

	CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("%s_History_%s.xls", m_pCompany->GetTicker(), sTime);
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

void CViewHistory::OnHistoryGoto() 
{
	if(!m_pCompany)
		return;

	CHistory* pHist = m_pCompany->GetHistory();
	if(!pHist)
		return;

	CDialogDate dlg;
	if(pHist->GetSize() > 0)
		dlg.m_Date = pHist->GetItemPtr(pHist->GetSize()-1)->date;
	if(dlg.DoModal() != IDOK)
		return;
	
	int nIndex = pHist->GetIndexByDate(dlg.m_Date);
	if(nIndex != -1)
		m_pGrid->SetSelection(nIndex+2, 1, nIndex+2, m_pGrid->GetColumnCount()-1);
	else
		AfxMessageBox("There is no history record for this date.  Please try again.");
}

void CViewHistory::OnHistoryGraph() 
{
	if(!m_pCompany)
		return;

	CViewCharts* pView = theApp.NewViewCharts();

	CString sTokens;
	CString sTicker = m_pCompany->GetTicker();

	sTokens.Format("%s_REPV %s_NEPV %s_Price", sTicker, sTicker, sTicker);

	pView->SetCharts(sTokens, "", true);
}

void CViewHistory::OnHistorySave() 
{
	if(!m_pCompany)
		return;

	CHistory* pHist = m_pCompany->GetHistory();
	if(!pHist)
		return;

	int nResult = AfxMessageBox("Are you sure you want to save the history into the Data-Base?  You cannot undo this operation.", MB_YESNO);
	if(nResult != IDYES)
		return;

	if(!CSQLCompany::UpdateHistory(*pHist, NUM_COLUMNS_IN_HISTORY_DB))
		AfxMessageBox("Unable to save the information into Data-Base.  Please try again.");
	else
		AfxMessageBox("The history is successfully saved into the Data-Base.");
}

LRESULT CViewHistory::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WMU_GRID_CELLCHANGED:
		{
			if(!m_pCompany)
				return 0;
			CHistory* pHist = m_pCompany->GetHistory();
			if(!pHist)
				return 0;

			int nRow = (int)wParam;
			int nCol = (int)lParam;

			int nCol1 = nCol <= 2 ? nCol-1 : nCol-4;
			
			CString s = m_pGrid->GetItemText(nRow, nCol);
			double f = (s.IsEmpty() || s=="-" || s=="n/a") ? _INF_ : atof(s);

			SHistoryItem* pItem = pHist->GetItemPtr(nRow-2);
			pItem->data[nCol1] = f;
		}
		break;
	}
	return CFormView::WindowProc(message, wParam, lParam);
}

void CViewHistory::OnHistoryAddToHistory() 
{
	if(!m_pCompany)
		return;

	COleDateTime date;
	CHistory* pHist = m_pCompany->GetHistory();
	if(!pHist)
	{
		COleDateTime null; null.SetStatus(COleDateTime::null);
		m_pCompany->LoadHistory(null, null);
		pHist = m_pCompany->GetHistory();
	}

	if(pHist->GetSize() == 0)
	{
		COleDateTime now(COleDateTime::GetCurrentTime());
		date = COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0,0,0);
	}
	else
	{
		date = pHist->GetItemPtr(0)->date;
		COleDateTimeSpan sp(7,0,0,0);
		date += sp;
	}

	CDialogDate dlg;
	dlg.m_Date = date;
	if(dlg.DoModal() != IDOK)
		return;

	// New record
	SHistoryItem item;
	item.date = dlg.m_Date;
	item.data[0] = _INF_;
	m_pCompany->GetHistoryItem(item);
	CSQLCompany::UpdateHistoryItem(m_pCompany->GetTicker(), item, NUM_COLUMNS_IN_HISTORY_DB);

	// Modify the last one
	if(pHist->GetSize() > 0)
	{
		pHist->GetItemPtr(0)->data[0] = 100.0 * ( item.data[1] / pHist->GetItemPtr(0)->data[1]  -  1 );
		CSQLCompany::UpdateHistoryItem(m_pCompany->GetTicker(), *pHist->GetItemPtr(0), NUM_COLUMNS_IN_HISTORY_DB);
	}

	COleDateTime null; null.SetStatus(COleDateTime::null);
	m_pCompany->LoadHistory(null, null);
	PopulateGrid();
}

void CViewHistory::OnHistoryCopy() 
{
	m_pGrid->CopySelectionToClipboard();
}

void CViewHistory::OnHistoryPaste() 
{
	m_pGrid->PasteFromClipboard();
}

void CViewHistory::OnHistoryPrint() 
{
	if(!m_pCompany)
		return;

	CString sDir = theApp.CreateTempDirectory();	
	CString sFile; sFile.Format("%s_%s.html", m_pCompany->GetTicker(), COleDateTime::GetCurrentTime().Format("%Y_%m_%d"));
	CString sPath = sDir + sFile;

	CStdioFile file;
	if(!file.Open(sPath, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
		return;

	CString sHeader;
	GetDlgItem(IDC_STATIC_NAME)->GetWindowText(sHeader);

	CString sLine;

	sLine.Format("<div align='center' style='font-size: 20px;'><b>%s</b></div><br><br>", sHeader);
	file.WriteString(sLine);

	sLine = "<table align='center' cellpadding='0' cellspacing='0' border='1'><tr><td>";
	file.WriteString(sLine);

	sLine = "<table width='100%' cellpadding='0' cellspacing='4px' border='0'>";
	file.WriteString(sLine);

	int nRows = m_pGrid->GetRowCount();
	int nCols = m_pGrid->GetColumnCount();

	for(int x=0; x<nRows; x++)
	{
		sLine = "<tr>";
		file.WriteString(sLine);

		for(int y=0; y<nCols; y++)
		{
			if(y==0 || x==0)
				sLine += "<td align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			else
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'>";
			sLine += m_pGrid->GetItemText(x, y);

			if(y==0) sLine += "</b></td>"; else sLine += "</td>";
		}
		file.WriteString(sLine);

		sLine = "</tr>";
		file.WriteString(sLine);
	}

	sLine = "</table>";
	file.WriteString(sLine);

	sLine = "</table></tr></td>";
	file.WriteString(sLine);
	file.Close();

	//::ShellExecute(m_hWnd, "open", sPath, NULL, NULL, SW_SHOW);
	CViewHTML* pView = theApp.NewViewHTML();
	pView->Navigate(sPath);
}
