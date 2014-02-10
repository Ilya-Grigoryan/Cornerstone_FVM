// ViewTable.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewTable.h"

#include "WndGrid.h"
#include "ViewHTML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewTable

IMPLEMENT_DYNCREATE(CViewTable, CFormView)

CViewTable::CViewTable()
	: CFormView(CViewTable::IDD)
{
	//{{AFX_DATA_INIT(CViewTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
}

CViewTable::~CViewTable()
{
}

void CViewTable::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTable)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTable, CFormView)
	//{{AFX_MSG_MAP(CViewTable)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_TABLE_COPY, OnTableCopy)
	ON_COMMAND(ID_TABLE_EXCEL, OnTableExcel)
	ON_COMMAND(ID_TABLE_PRINT, OnTablePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTable diagnostics

#ifdef _DEBUG
void CViewTable::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewTable::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewTable message handlers
void CViewTable::OnInitialUpdate() 
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

	m_pGrid->SetColumnCount(2);
	//for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+4; i++)
	//	m_pGrid->SetItemText(0, i, CDataManager::GetColName(i));

	m_pGrid->SetRowCount(10);

	m_pGrid->SetColumnSize(0, 75);
	m_pGrid->SetCellsFont(m_Font);

	m_pGrid->SetFixedColumnCount(1);
	m_pGrid->SetFixedRowCount(1);
	
	//m_pGrid->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGrid->SetDefaultColorGridNonFixed(RGB(255,255,255));
	m_pGrid->SetFixedCellsBkColor(RGB(200,200,200));

	Resize();
}
void CViewTable::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewTable::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcGrid(10, 34, rc.right-10, rc.bottom-10);

	if(GetDlgItem(IDC_STATIC_NAME))
		GetDlgItem(IDC_STATIC_NAME)->MoveWindow(10, 8, rc.Width()-20, 18);

	if(m_pGrid)
		m_pGrid->MoveWindow(rcGrid);
}

void CViewTable::OnDestroy() 
{
	CFormView::OnDestroy();

	CDataManager dm(&theApp.m_Universe);
	dm.FreeDataCollection(m_DataCollect);
	
	if(m_pGrid)
		m_pGrid->DestroyWindow();
}

HBRUSH CViewTable::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CViewTable::SetData(const CDataCollection& dataCollect)
{
	CDataManager::CopyDataCollection(dataCollect, m_DataCollect);

	SetupGrid();
}

void CViewTable::SetupGrid()
{
	CString sTitle = "Data";

	int nCols = m_DataCollect.GetSize() + 1;
	int nRows = 1;
	if(nCols > 1)
	{
		nRows = m_DataCollect[0]->GetSize() + 1;
		sTitle += " - ";
		sTitle += m_DataCollect[0]->m_szToken;
	}

	m_pGrid->SetColumnCount(nCols);	
	m_pGrid->SetRowCount(nRows);
	m_pGrid->SetCellsFont(m_Font);

	// column names
	m_pGrid->SetItemText(0,0, "Date");
	for(int i=1; i<nCols; i++)
	{
		m_pGrid->SetItemText(0, i, m_DataCollect[i-1]->m_szToken);
		if(i>1)
		{
			sTitle += ", ";
			sTitle += m_DataCollect[i-1]->m_szToken;
		}
	}

	if(nCols <= 1)
		return;

	CData *pData0 = m_DataCollect[0];
	for(int nRow = 1; nRow < nRows; nRow++)
	{
		COleDateTime date = COleDateTime(pData0->m_pDates[nRow-1]);
		m_pGrid->SetItemText(nRow, 0, date.Format("%m/%d/%Y"));

		for(int nCol = 1; nCol < nCols; nCol++)
		{
			CData* pData = m_DataCollect[nCol-1];
			double f = (*pData)[date];
			CString s;
			if(f<_INF_)
				s.Format("%.2f", f);
			else
				s = "n/a";
			m_pGrid->SetItemText(nRow, nCol, s);
		}

		if(nRow % 2 == 0)
			m_pGrid->SetRowBkColor(nRow, RGB(226,237,250));
		else
			m_pGrid->SetRowBkColor(nRow, RGB(255,255,255));
	}

	GetDocument()->SetTitle(sTitle);
}

void CViewTable::OnTableCopy() 
{
	m_pGrid->CopySelectionToClipboard();
}

void CViewTable::OnTableExcel() 
{
	CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("Data_%s.xls", sTime);
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

void CViewTable::OnTablePrint() 
{
	CString sDir = theApp.CreateTempDirectory();	
	CString sFile; sFile.Format("PrintPreview_%s.html", COleDateTime::GetCurrentTime().Format("%Y_%m_%d"));
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
