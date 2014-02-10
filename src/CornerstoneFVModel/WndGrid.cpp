/////////////////////////////////////////////////////////////////////////////////////////////
// WndGrid.cpp : implementation file
//
//
// 22.05.2003
// D.D.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WndGrid.h"
//#include "globalfunctions.h"
#include "float.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ITEM(row, col)	((row)*m_nColCount+(col))
#define CELL(row, col)	m_pCells[((row)*m_nColCount+(col))]

/////////////////////////////////////////////////////////////////////////////
// CCell
CCell::CCell()
{
	m_nType = CELL_EMPTY;
	m_colorFore = RGB(0,0,0);
	m_colorBk = GetSysColor(COLOR_WINDOW);
	m_hFont = NULL;
	m_nFormat = 0;
	m_bAllowEdit = 1;
	m_bAllowCombo = false;
	m_bOwnerDraw = false;
	m_pComboValues = new CStringArray;
}

CCell::~CCell()
{
	if(m_hFont)
	{
	//	::GlobalUnlock(m_hFont);
		m_hFont = NULL;
	}

	if(m_pComboValues)
		delete m_pComboValues;
}

void CCell::operator = (CCell& cell)
{
	m_nType = cell.m_nType;
	m_sText = cell.m_sText;
	m_colorFore = cell.m_colorFore;
	m_colorBk = cell.m_colorBk;
	m_hFont = cell.m_hFont;
	//m_hFont = (HFONT)CGlobalFunctions::CopyHandle(cell.m_hFont);
	m_bAllowEdit = cell.m_bAllowEdit;
	m_bAllowCombo = cell.m_bAllowCombo;

	m_pComboValues->RemoveAll(); 
	for(int i=0; i<cell.m_pComboValues->GetSize(); i++)
		m_pComboValues->Add((*cell.m_pComboValues)[i]);
}

void CCell::Draw(CWndGrid* pGrid, int x, int y, CDC* pDC, CRect rcCell)
{
	// back
	COLORREF colorBack;
	if(	(pGrid->m_Selection.InRange(x,y) && !pGrid->m_Selection.IsFirstCell(x,y)) ||
		(pGrid->IsColumnFixed(x) && pGrid->m_Selection.InRangeY(y)) ||
		(pGrid->IsRowFixed(y) && pGrid->m_Selection.InRangeX(x) ))
		colorBack = pGrid->m_colorSelection;
	else
		colorBack = m_colorBk;
	CRect rcBack(rcCell); 
	rcBack.DeflateRect(1,1,0,0);
	pDC->FillSolidRect(rcBack, colorBack);

	// bounding lines
	CPen pen; 
	if(pGrid->IsColumnFixed(x) || pGrid->IsRowFixed(y))
		pen.CreatePen(PS_SOLID, pGrid->m_nFixedGridLineWidth, pGrid->m_nFixedGridLineWidth==0 ? m_colorBk : pGrid->m_colorGridFixed);
	else
		pen.CreatePen(PS_SOLID, pGrid->m_nGridLineWidth, pGrid->m_nGridLineWidth==0 ? m_colorBk : pGrid->m_colorGrid);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rcCell.right, rcCell.top);
	pDC->LineTo(rcCell.right, rcCell.bottom);
	pDC->LineTo(rcCell.left, rcCell.bottom);
	if(pOldPen)
		pDC->SelectObject(pOldPen);

	// special case for owner draw items
	if(m_bOwnerDraw)
	{
		CWnd* pOwner = pGrid->GetOwner();
		if(pOwner && pOwner->m_hWnd)
		{
			STRUCT_OWNERDRAW_INFO st;
			st.pDC = pDC;
			st.rcDraw = rcCell;
			st.pGrid = pGrid;
			st.pCell = this;
			st.nRow = y;
			st.nCol = x;
			pOwner->SendMessage(WMU_GRID_DRAWCELL, (WPARAM)&st, (LPARAM)NULL);
		}

		return;
	}

	// data
	switch(m_nType)
	{
	case CELL_TEXT:
		{
			CFont* pFont = CFont::FromHandle(m_hFont);
			CFont *pOldFont = NULL;
			if(m_hFont)
				pOldFont = pDC->SelectObject(pFont);
			else
				pOldFont = pDC->SelectObject(&pGrid->m_DefaultFont);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_colorFore);
			CRect rcText(rcCell); rcText.DeflateRect(4,0,4,0);
			pDC->DrawText(m_sText, rcText, pGrid->GetTextAlign()|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

			if(pOldFont)
				pDC->SelectObject(pOldFont);

		}
		break;

	case CELL_CALLBACK:
		{
			CString sText;
			if(pGrid->GetParent())
			{
				CPoint ptCell(x, y);
				pGrid->GetParent()->SendMessage(WMU_CALLBACK_GETTEXT, (WPARAM)&ptCell, (LPARAM)&sText);
			}

			CFont* pFont = CFont::FromHandle(m_hFont);
			CFont *pOldFont = NULL;
			if(m_hFont)
				pOldFont = pDC->SelectObject(pFont);
			else
				pOldFont = pDC->SelectObject(&pGrid->m_DefaultFont);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_colorFore);
			CRect rcText(rcCell); rcText.DeflateRect(4,0,4,0);
			pDC->DrawText(sText, rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

			if(pOldFont)
				pDC->SelectObject(pOldFont);

		}
		break;

	case CELL_BITMAP:
		{
/*			if(!m_hDib)
			{
				TRACE(_T("Grid: Drawing: Invalid Item DIB.\n"));
				return;
			}

			CDCDib dc;
			dc.SetHandle(m_hDib);

			int nSize = min(rcCell.Width(), rcCell.Height())-2;
			int nBmX = rcCell.left;// + (rcCell.Width()-nSize)/2;
			int nBmY = rcCell.top;// + (rcCell.Height()-nSize)/2;


			switch(m_nFormat)
			{
				case 0:
					pDC->StretchBlt(nBmX+1, nBmY+1, rcCell.Width()-2, rcCell.Height()-2, &dc, 0,0,dc.GetSizeX(), dc.GetSizeY(), SRCCOPY);
				break;

				case 1:
					double fAspectX = (double) rcCell.Width()/(double) dc.GetSizeX();
					double fAspectY = (double) rcCell.Height()/(double) dc.GetSizeY();
					double fAspect	= min(fAspectX,fAspectY);
						pDC -> StretchBlt(
									(int)(rcCell.left+rcCell.Width()/2 - dc.GetSizeX() * fAspect /2),
									(int)(rcCell.top+rcCell.Height()/2 - dc.GetSizeY() * fAspect /2),
									(int)((double)dc.GetSizeX() * fAspect),
									(int)((double)dc.GetSizeY() * fAspect),
									&dc,0,0,dc.GetSizeX(),dc.GetSizeY(),SRCCOPY);

				break;
			}
*/

		}
		break;

	default:
		break;
	}	
}


/////////////////////////////////////////////////////////////////////////////
// CIndexing

CIndexing::CIndexing(CWndGrid*	pWndGrid, int nColumn)
{
	m_nColumn = nColumn;
	m_nCount = 0;
	m_pWndGrid = pWndGrid;
}

CIndexing::~CIndexing()
{

}

void	CIndexing::SetCount(int nCount)
{
	m_arIndexes.SetSize(nCount);

	if(nCount > m_nCount)
		for(int i=m_nCount; i<nCount; i++)
			m_arIndexes[i] = i+m_pWndGrid->m_nFixedRows;

	m_nCount = nCount;
}

BOOL	CIndexing::AddValue(LPCTSTR szVal, int nIndex)
{
	if(m_pWndGrid->IsRowFixed(nIndex))
		return true;

	// find old position of the index and remove it
	FindAndRemoveIndex(nIndex);

	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = m_arIndexes.GetSize()-1;
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find the index to which the element to be inserted*/
	while(nFirst <= nLast)
	{
		CString tmp="";
		int nRow = m_arIndexes[nMiddle];
		if(nRow != -1)
			tmp = m_pWndGrid->m_pCells[nRow*m_pWndGrid->m_nColCount+m_nColumn].m_sText;

		/* check whether the element already exists*/ 
		if(tmp == szVal)
		{
			m_arIndexes.InsertAt(nMiddle, nIndex);
			return true; //
		}

		/* check for great or less*/
		if(tmp > szVal)
			nLast = nMiddle - 1;
		else if(tmp < szVal)
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}
	
	/* the nFirst actually indicates the index */
	ASSERT(nFirst >= 0 && nFirst <= m_nCount);

	m_arIndexes.InsertAt(nFirst, nIndex);
	m_nCount = m_arIndexes.GetSize();
	return true;
}

void CIndexing::FindAndRemoveIndex(int nIndex)
{
	int nPos = -1;
	for(int i=0; i<m_arIndexes.GetSize(); i++)
		if(m_arIndexes[i] == nIndex)
		{
			nPos = i;
			break;
		}
	
	if(nPos == -1)
		return;

	m_arIndexes.RemoveAt(nPos);
	m_nCount = m_arIndexes.GetSize();
}

int		CIndexing::GetAscendingSortedIndex(int nIndex)
{
	nIndex -= m_pWndGrid->m_nFixedRows;
	return m_arIndexes[nIndex];
}

int		CIndexing::GetDescendingSortedIndex(int nIndex)
{
	nIndex -= m_pWndGrid->m_nFixedRows;
	return m_arIndexes[m_arIndexes.GetSize()-nIndex-1];
}

void	CIndexing::UpdateWhenFixedRowCountIsChanged()
{
	int nOldFixed = m_pWndGrid->m_nRowCount - m_nCount;
	int nNewFixed = m_pWndGrid->m_nFixedRows;

	if(nNewFixed < nOldFixed)
	{
		for(int i=nNewFixed; i<nOldFixed; i++)
			AddValue(m_pWndGrid->m_pCells[i*m_pWndGrid->m_nColCount+m_nColumn].m_sText, i);
	}
	else
	{
		for(int i=nOldFixed; i<nNewFixed; i++)
			FindAndRemoveIndex(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWndGrid

CWndGrid::CWndGrid()
{
	m_nRowCount = 0;
	m_nColCount = 0;
	m_nFixedRows = 0;
	m_nFixedCols = 0;
	m_pCells = NULL;
	m_colorOutOfGridBk = RGB(255,255,255); 
	m_colorNonFixedBk = RGB(255,255,255);//GetSysColor(COLOR_WINDOW);
	m_colorFixedBk = GetSysColor(COLOR_BTNFACE);
	m_colorGrid = RGB(192,192,192);
	m_colorGridFixed = RGB(128,128,128);
	m_colorSelection = RGB(169,178,202);
	m_nGridLineWidth = 1;
	m_nFixedGridLineWidth = 1;
	m_nDefaultSizeX = 70;
	m_nDefaultSizeY = 20;
	m_pColumnSizes = NULL;
	m_pRowSizes = NULL;
	m_nMouseMode = MM_NONE;
	m_nActiveCellRow = -1;
	m_nActiveCellRow = -1;
	m_bEditing = false;
	m_bLocked = false;
	m_nSortColumn = -1;
	m_bSortAscending = true;
	m_bWholeRowSel = false;
	m_bScrollBars = true;
	m_nTextAlign = DT_LEFT;
}

CWndGrid::~CWndGrid()
{
}

BOOL CWndGrid::Create(CWnd* pParent)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = wndClass.hIconSm = NULL;
	wndClass.hInstance = AfxGetApp()->m_hInstance;
	wndClass.lpszClassName = _T("Grid");
	wndClass.lpszMenuName = NULL;
	wndClass.lpfnWndProc = ::DefWindowProc;

	RegisterClassEx(&wndClass);

	if(!CWnd::Create(_T("Grid"), _T("Grid Window"), 
					 WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL, 
					 CRect(0,0,0,0),pParent, 1223))
		return false;

	return true;
}

int		CWndGrid::AddRow()
{
	// Cells
	CCell* pCell = new CCell[(m_nRowCount+1)*m_nColCount];
	if(!pCell)
	{
		TRACE(_T("Grid: AddRow: Memory allocation error:\n"));
		return -1;
	}
	int* pRowSizes = new int[m_nRowCount+1];
	if(!pRowSizes)
	{
		TRACE(_T("Grid: AddRow: Memory allocation error:\n"));
		return -1;
	}

	for(int y=0; y<m_nRowCount; y++)
	{
		pRowSizes[y] = m_pRowSizes[y];
		for(int x=0; x<m_nColCount; x++)
			pCell[y*m_nColCount+x] = m_pCells[y*m_nColCount+x];
	}

	pRowSizes[m_nRowCount] = m_nDefaultSizeY;
	for(int x=0; x<m_nFixedCols; x++)
		pCell[m_nRowCount*m_nColCount+x].m_colorBk = m_colorFixedBk;
	
	delete [] m_pCells;
	m_pCells = pCell;
	delete [] m_pRowSizes;
	m_pRowSizes = pRowSizes;

	m_nRowCount++;
	UpdateScrollSizes();
	Invalidate();
	return m_nRowCount-1;
}

int		CWndGrid::AddColumn()
{
	// Cells
	CCell* pCell = new CCell[m_nRowCount*(m_nColCount+1)];
	if(!pCell)
	{
		TRACE(_T("Grid: AddColumn: Memory allocation error:\n"));
		return -1;
	}
	int* pColSizes = new int[m_nColCount+1];
	if(!pColSizes)
	{
		TRACE(_T("Grid: AddColumn: Memory allocation error:\n"));
		return -1;
	}

	for(int x=0; x<m_nColCount; x++)
	{
		pColSizes[x] = m_pColumnSizes[x];
		for(int y=0; y<m_nRowCount; y++)
			pCell[y*(m_nColCount+1)+x] = m_pCells[y*m_nColCount+x];
	}

	pColSizes[m_nColCount] = m_nDefaultSizeX;
	for(int y=0; y<m_nFixedRows; y++)
		pCell[y*(m_nColCount+1)+m_nColCount].m_colorBk = m_colorFixedBk;
	
	delete [] m_pCells;
	m_pCells = pCell;
	delete [] m_pColumnSizes;
	m_pColumnSizes = pColSizes;

	m_nColCount++;
	UpdateScrollSizes();
	Invalidate();
	return m_nColCount-1;
}

BOOL	CWndGrid::InsertRow(int nIndex)
{
	if(nIndex >= m_nRowCount)
		return AddRow();

	nIndex = max(0, nIndex);

	// Cells
	CCell* pCell = new CCell[(m_nRowCount+1)*m_nColCount];
	if(!pCell)
	{
		TRACE(_T("Grid: InsertRow: Memory allocation error:\n"));
		return false;
	}
	int* pRowSizes = new int[m_nRowCount+1];
	if(!pRowSizes)
	{
		TRACE(_T("Grid: InsertRow: Memory allocation error:\n"));
		return false;
	}

	for(int ys=0,yd=0; ys<m_nRowCount; ys++,yd++)
	{
		if(yd==nIndex)
		{
			pRowSizes[yd] = m_nDefaultSizeY;

			int nFixedCols = nIndex<m_nFixedRows ? m_nColCount : m_nFixedCols;
			for(int x=0; x<nFixedCols; x++)
				pCell[yd*m_nColCount+x].m_colorBk = m_colorFixedBk;

			ys--;
			continue;
		}

		for(int x=0; x<m_nColCount; x++)
			pCell[yd*m_nColCount+x] = m_pCells[ys*m_nColCount+x];

		pRowSizes[yd] = m_pRowSizes[ys];
	}

	delete [] m_pCells;
	m_pCells = pCell;
	delete [] m_pRowSizes;
	m_pRowSizes = pRowSizes;

	if(nIndex<m_nFixedRows)
		++m_nFixedRows;
	m_nRowCount++;
	UpdateScrollSizes();
	Invalidate();
	return true;	
}

BOOL	CWndGrid::InsertColumn(int nIndex)
{
	if(nIndex >= m_nColCount)
		return AddColumn();

	nIndex = max(0, nIndex);

	// Cells
	CCell* pCell = new CCell[m_nRowCount*(m_nColCount+1)];
	if(!pCell)
	{
		TRACE(_T("Grid: InsertColumn: Memory allocation error:\n"));
		return false;
	}
	int* pColSizes = new int[m_nColCount+1];
	if(!pColSizes)
	{
		TRACE(_T("Grid: InsertColumn: Memory allocation error:\n"));
		return false;
	}

	for(int xs=0,xd=0; xs<m_nColCount; xs++,xd++)
	{
		if(xd==nIndex)
		{
			pColSizes[xd] = m_nDefaultSizeX;
			int nFixedRows = nIndex<m_nFixedCols ? m_nRowCount : m_nFixedRows;
			for(int y=0; y<nFixedRows; y++)
				pCell[y*(m_nColCount+1)+xd].m_colorBk = m_colorFixedBk;		
		
			xs--;
			continue;
		}
		for(int y=0; y<m_nRowCount; y++)
			pCell[y*(m_nColCount+1)+xd] = m_pCells[y*m_nColCount+xs];

		pColSizes[xd] = m_pColumnSizes[xs];
	}

	delete [] m_pCells;
	m_pCells = pCell;
	delete [] m_pColumnSizes;
	m_pColumnSizes = pColSizes;

	if(nIndex<m_nFixedCols)
		++m_nFixedCols;
	m_nColCount++;
	UpdateScrollSizes();
	Invalidate();
	return true;
}

BOOL	CWndGrid::DeleteRow(int nRow)
{
	m_wndEdit.ShowWindow(SW_HIDE);
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();

	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: DeleteRow: row index is out of range.\n"));
		return false;
	}

	// shift cells
	for(int y=nRow+1; y<m_nRowCount; y++)
		for(int x=0; x<m_nColCount; x++)
			m_pCells[(y-1)*m_nColCount+x] = m_pCells[y*m_nColCount+x];	

	// shift row sizes
	for(int y=nRow+1; y<m_nRowCount; y++)
		m_pRowSizes[y-1] = m_pRowSizes[y];

	// if row was fixed, decrement fixed row count
	if(nRow < m_nFixedRows)
		--m_nFixedRows;
	
	m_nRowCount--;
	UpdateScrollSizes();
	Invalidate();
	return true;
}

BOOL	CWndGrid::DeleteColumn(int nColumn)
{
	m_wndEdit.ShowWindow(SW_HIDE);
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: DeleteColumn: column index is out of range.\n"));
		return false;
	}

	// Cells
	CCell* pCell = new CCell[m_nRowCount*(m_nColCount-1)];
	if(!pCell)
	{
		TRACE(_T("Grid: DeleteColumn: Memory allocation error:\n"));
		return false;
	}

	for(int xs=0, xd=0; xs<m_nColCount; xs++)
	{
		if(xs==nColumn)
			continue;
		for(int y=0; y<m_nRowCount; y++)
			pCell[y*(m_nColCount-1)+xd] = m_pCells[y*m_nColCount+xs];
		++xd;
	}
	
	delete [] m_pCells;
	m_pCells = pCell;

	// shift column sizes
	for(int x=nColumn+1; x<m_nColCount; x++)
		m_pColumnSizes[x-1] = m_pColumnSizes[x];

	// if column was fixed, decerement fixed column count
	if(nColumn < m_nFixedCols)
		--m_nFixedCols;

	m_nColCount--;
	UpdateScrollSizes();
	Invalidate();
	return true;
}

BOOL	CWndGrid::SetRowCount(int nCount)
{
	m_wndEdit.ShowWindow(SW_HIDE);
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();

	if(nCount == m_nRowCount)
		return true;

	// Cells
	CCell* pCell = new CCell[nCount*m_nColCount];
	if(!pCell)
	{
		TRACE(_T("Grid: SetRowCount: Memory allocation error:\n"));
		return false;
	}

	for(int x=0; x<m_nColCount; x++)
		for(int y=0; y<min(nCount,m_nRowCount); y++)
			pCell[y*m_nColCount+x] = m_pCells[y*m_nColCount+x];
	
	delete [] m_pCells;
	m_pCells = pCell;

	// row sizes
	int* pRowSizes = new int[nCount];
	if(!pRowSizes)
	{
		TRACE(_T("Grid: SetRowCount: Memory allocation error:\n"));
		return false;
	}

	for(int i=0; i<min(nCount, m_nRowCount); i++)
		pRowSizes[i] = m_pRowSizes[i];
	if(nCount > m_nRowCount)
		for(int i=m_nRowCount; i<nCount; i++)
			pRowSizes[i] = m_nDefaultSizeY;

	delete [] m_pRowSizes;
	m_pRowSizes = pRowSizes;

	m_nRowCount = nCount;

	for(int x=0; x<m_nColCount; x++)
		for(int y=0; y<m_nFixedRows; y++)
			m_pCells[y*m_nColCount+x].m_colorBk = m_colorFixedBk;
	for(int x=0; x<m_nFixedCols; x++)
		for(int y=0; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_colorBk = m_colorFixedBk;

	UpdateScrollSizes();
	Invalidate();
	return true;
}

BOOL	CWndGrid::SetColumnCount(int nCount)
{
	m_wndEdit.ShowWindow(SW_HIDE);
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();

	if(nCount == m_nColCount)
		return true;

	// Cells
	CCell* pCell = new CCell[m_nRowCount*nCount];
	if(!pCell)
	{
		TRACE(_T("Grid: SetColumnCount: Memory allocation error:\n"));
		return false;
	}

	for(int x=0; x<min(nCount, m_nColCount); x++)
		for(int y=0; y<m_nRowCount; y++)
			pCell[y*nCount+x] = m_pCells[y*m_nColCount+x];

	delete [] m_pCells;
	m_pCells = pCell;

	// Column sizes
	int *pColSizes = new int[nCount];
	if(!pColSizes)
	{
		TRACE(_T("Grid: SetColumnCount: Memory allocation error:\n"));
		return false;
	}
	for(int i=0; i<min(nCount, m_nColCount); i++)
		pColSizes[i] = m_pColumnSizes[i];
	if(nCount > m_nColCount)
		for(int i=m_nColCount; i<nCount; i++)
			pColSizes[i] = m_nDefaultSizeX;

	delete [] m_pColumnSizes;
	m_pColumnSizes = pColSizes;

	m_nColCount = nCount;
	if(m_nSortColumn >= m_nColCount)
		m_nSortColumn = -1;
	
	for(int x=0; x<m_nColCount; x++)
		for(int y=0; y<m_nFixedRows; y++)
			m_pCells[y*m_nColCount+x].m_colorBk = m_colorFixedBk;
	for(int x=0; x<m_nFixedCols; x++)
		for(int y=0; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_colorBk = m_colorFixedBk;

	UpdateScrollSizes();
	Invalidate();
	return true;
}

BOOL	CWndGrid::SetFixedRowCount(int nCount)
{
	if(m_nFixedRows == nCount)
		return true;
	nCount = min(nCount, m_nRowCount);

	if(nCount < m_nFixedRows)
	{
		for(int x=m_nFixedCols; x<m_nColCount; x++)
			for(int y=nCount; y<m_nFixedRows; y++)
				m_pCells[y*m_nColCount+x].m_colorBk = m_colorNonFixedBk;
	}
	else
	{
		for(int x=0; x<m_nColCount; x++)
			for(int y=m_nFixedRows; y<nCount; y++)
				m_pCells[y*m_nColCount+x].m_colorBk = m_colorFixedBk;
	}

	m_nFixedRows = nCount;
	return true;
}

BOOL	CWndGrid::SetFixedColumnCount(int nCount)
{
	if(m_nFixedCols == nCount)
		return true;
	nCount = min(nCount, m_nColCount);

	if(nCount < m_nFixedCols)
	{
		for(int x=nCount; x<m_nFixedCols; x++)
			for(int y=m_nFixedRows; y<m_nRowCount; y++)
				m_pCells[y*m_nColCount+x].m_colorBk = m_colorNonFixedBk;
	}
	else
	{
		for(int x=m_nFixedCols; x<nCount; x++)
			for(int y=0; y<m_nRowCount; y++)
				m_pCells[y*m_nColCount+x].m_colorBk = m_colorFixedBk;
	}

	m_nFixedCols = nCount;
	return true;
}

int		CWndGrid::GetRowCount()
{
	return m_nRowCount;
}

int		CWndGrid::GetColumnCount()
{
	return m_nColCount;
}

int		CWndGrid::GetFixedRowCount()
{
	return m_nFixedRows;
}

int		CWndGrid::GetFixedColumnCount()
{
	return m_nFixedCols;
}

BOOL	CWndGrid::SetColumnSize(int nColumn, int nSize)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetColumnSize: column index is out of range.\n"));
		return false;
	}	

	m_pColumnSizes[nColumn] = nSize;
	return true;
}

BOOL	CWndGrid::SetRowSize(int nRow, int nSize)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetRowSize: row index is out of range.\n"));
		return false;
	}

	m_pRowSizes[nRow] = nSize;
	return true;
}

int		CWndGrid::GetColumnSize(int nColumn)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetColumnSize: column index is out of range.\n"));
		return 0;
	}	

	return m_pColumnSizes[nColumn];
}

int		CWndGrid::GetRowSize(int nRow)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetRowSize: row index is out of range.\n"));
		return false;
	}

	return m_pRowSizes[nRow];
}

BOOL	CWndGrid::SetOutOfGridBkColor(COLORREF color)
{
	m_colorOutOfGridBk = color;
	return true;
}

BOOL	CWndGrid::SetNonFixedCellsBkColor(COLORREF color)
{
	for(int x=m_nFixedCols; x<m_nColCount; x++)
		for(int y=m_nFixedRows; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_colorBk = color;

	return true;
}

BOOL	CWndGrid::SetNonFixedCellsForeColor(COLORREF color)
{
	for(int x=m_nFixedCols; x<m_nColCount; x++)
		for(int y=m_nFixedRows; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_colorFore = color;

	return true;
}

BOOL	CWndGrid::SetFixedCellsBkColor(COLORREF color)
{
	int x, y;
	for(x=0; x<m_nFixedCols; x++)
		for(y=0; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_colorBk = color;

	for(y=0; y<m_nFixedRows; y++)
		for(x=0; x<m_nColCount; x++)
			m_pCells[y*m_nColCount+x].m_colorBk = color;

	return true;
}

BOOL	CWndGrid::SetFixedCellsForeColor(COLORREF color)
{
	int x, y;
	for(x=0; x<m_nFixedCols; x++)
		for(y=0; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_colorFore = color;

	for(y=0; y<m_nFixedRows; y++)
		for(x=0; x<m_nColCount; x++)
			m_pCells[y*m_nColCount+x].m_colorFore = color;

	return true;
}

BOOL	CWndGrid::SetRowBkColor(int nRow, COLORREF color)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetRowBkColor: row index is out of range.\n"));
		return false;
	}

	for(int x=m_nFixedCols; x<m_nColCount; x++)
		m_pCells[nRow*m_nColCount+x].m_colorBk = color;

	return true;
}

BOOL	CWndGrid::SetRowForeColor(int nRow, COLORREF color)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetRowForeColor: row index is out of range.\n"));
		return false;
	}

	for(int x=m_nFixedCols; x<m_nColCount; x++)
		m_pCells[nRow*m_nColCount+x].m_colorFore = color;

	return true;
}

BOOL	CWndGrid::SetColumnBkColor(int nColumn, COLORREF color)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetColumnBkColor: column index is out of range.\n"));
		return false;
	}

	for(int y=m_nFixedRows; y<m_nRowCount; y++)
		m_pCells[y*m_nColCount+nColumn].m_colorBk = color;

	return true;
}

BOOL	CWndGrid::SetColumnForeColor(int nColumn, COLORREF color)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetColumnForeColor: column index is out of range.\n"));
		return false;
	}

	for(int y=m_nFixedRows; y<m_nRowCount; y++)
		m_pCells[y*m_nColCount+nColumn].m_colorFore = color;

	return true;
}

BOOL	CWndGrid::SetItemBkColor(int nRow, int nColumn, COLORREF color)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemBkColor: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemBkColor: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_colorBk = color;
	return true;
}

BOOL	CWndGrid::SetItemForeColor(int nRow, int nColumn, COLORREF color)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemForeColor: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemForeColor: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_colorFore = color;
	return true;
}

COLORREF	CWndGrid::GetItemBkColor(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemBkColor: row index is out of range.\n"));
		return 0;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemBkColor: column index is out of range.\n"));
		return 0;
	}

	return m_pCells[nRow*m_nColCount + nColumn].m_colorBk;
}

COLORREF	CWndGrid::GetItemForeColor(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemForeColor: row index is out of range.\n"));
		return 0;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemForeColor: column index is out of range.\n"));
		return 0;
	}

	return m_pCells[nRow*m_nColCount + nColumn].m_colorFore;
}

// font management
BOOL	CWndGrid::SetCellsFont(HFONT hFont)
{
	for(int x=0; x<m_nColCount; x++)
		for(int y=0; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_hFont = hFont;

	return true;
}

BOOL	CWndGrid::SetNonFixedCellsFont(HFONT hFont)
{
	for(int x=m_nFixedCols; x<m_nColCount; x++)
		for(int y=m_nFixedRows; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_hFont = hFont;

	return true;	
}

BOOL	CWndGrid::SetFixedCellsFont(HFONT hFont)
{
	int x, y;
	for(x=0; x<m_nFixedCols; x++)
		for(y=0; y<m_nRowCount; y++)
			m_pCells[y*m_nColCount+x].m_hFont = hFont;

	for(y=0; y<m_nFixedRows; y++)
		for(x=0; x<m_nColCount; x++)
			m_pCells[y*m_nColCount+x].m_hFont = hFont;

	return true;
}

BOOL	CWndGrid::SetRowFont(int nRow, HFONT hFont)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetRowFont: row index is out of range.\n"));
		return false;
	}

	for(int x=0; x<m_nColCount; x++)
		m_pCells[nRow*m_nColCount+x].m_hFont = hFont;

	return true;
}

BOOL	CWndGrid::SetColumnFont(int nColumn, HFONT hFont)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetColumnFont: column index is out of range.\n"));
		return false;
	}


	for(int y=0; y<m_nRowCount; y++)
		m_pCells[y*m_nColCount+nColumn].m_hFont = hFont;

	return true;
}

BOOL	CWndGrid::SetItemFont(int nRow, int nColumn, HFONT hFont)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemFont: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemFont: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_hFont = hFont;
	return true;
}

HFONT	CWndGrid::GetItemFont(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemFont: row index is out of range.\n"));
		return NULL;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemFont: column index is out of range.\n"));
		return NULL;
	}

	return m_pCells[nRow*m_nColCount + nColumn].m_hFont;
}

// text management
BOOL	CWndGrid::SetItemText(int nRow, int nColumn, LPCTSTR lpszText)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemText: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemText: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_sText = lpszText;
	m_pCells[nRow*m_nColCount + nColumn].m_nType = CELL_TEXT;

	return true;
}

BOOL	CWndGrid::SetItemDIB(int nRow, int nColumn, HANDLE hDib)
{
/*	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemDIB: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemDIB: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_hDib = CGlobalFunctions::CopyHandle(hDib);
	if(hDib)
		m_pCells[nRow*m_nColCount + nColumn].m_nType = CELL_BITMAP;
	else
		m_pCells[nRow*m_nColCount + nColumn].m_nType = CELL_EMPTY;*/
	return true;
}

BOOL	CWndGrid::SetItemBitmap(int nRow, int nColumn, HBITMAP hBitmap) 
{
/*	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemBitmap: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemBitmap: column index is out of range.\n"));
		return false;
	}

	HANDLE hDib = CDib::DibFromBitmap(hBitmap,NULL);
	m_pCells[nRow*m_nColCount + nColumn].m_hDib = hDib;
	if(hDib)
		m_pCells[nRow*m_nColCount + nColumn].m_nType = CELL_BITMAP;
	else
		m_pCells[nRow*m_nColCount + nColumn].m_nType = CELL_EMPTY;*/
	return true;
}

BOOL	CWndGrid::SetItemOwnerDraw(int nRow, int nColumn, BOOL bOwnerDraw)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemOwnerDraw: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemOwnerDraw: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_bOwnerDraw = bOwnerDraw;
	return true;
}

BOOL	CWndGrid::GetItemOwnerDraw(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemOwnerDraw: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemOwnerDraw: column index is out of range.\n"));
		return false;
	}

	return m_pCells[nRow*m_nColCount + nColumn].m_bOwnerDraw;
}

BOOL	CWndGrid::ClearItem(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: ClearItem: row index is out of range.\n"));
		return false;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: ClearItem: column index is out of range.\n"));
		return false;
	}

	m_pCells[nRow*m_nColCount + nColumn].m_sText = "";
	m_pCells[nRow*m_nColCount + nColumn].m_hDib = NULL;
	m_pCells[nRow*m_nColCount + nColumn].m_nType = CELL_EMPTY;
	return true;
}

BOOL	CWndGrid::ClearRow(int nRow)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: ClearRow: row index is out of range.\n"));
		return false;
	}

	for(int x=0; x<m_nColCount; x++)
	{
		m_pCells[nRow*m_nColCount+x].m_sText = "";
		m_pCells[nRow*m_nColCount+x].m_hDib = NULL;
		m_pCells[nRow*m_nColCount+x].m_nType = CELL_EMPTY;
	}

	return true;
}

BOOL	CWndGrid::ClearColumn(int nColumn)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: ClearColumn: column index is out of range.\n"));
		return false;
	}

	for(int y=0; y<m_nRowCount; y++)
	{
		m_pCells[y*m_nColCount+nColumn].m_sText = "";
		m_pCells[y*m_nColCount+nColumn].m_hDib = NULL;
		m_pCells[y*m_nColCount+nColumn].m_nType = CELL_EMPTY;
	}

	return true;
}

BOOL	CWndGrid::ClearAll()
{
	for(int x=0; x<m_nColCount; x++)
		for(int y=0; y<m_nRowCount; y++)
		{
			m_pCells[y*m_nColCount+x].m_sText = "";
			m_pCells[y*m_nColCount+x].m_hDib = NULL;
			m_pCells[y*m_nColCount+x].m_nType = CELL_EMPTY;
		}

	return true;
}

BOOL	CWndGrid::ClearAllFixed()
{
	for(int x=0; x<m_nFixedCols; x++)
		for(int y=0; y<m_nFixedRows; y++)
		{
			m_pCells[y*m_nColCount+x].m_sText = "";
			m_pCells[y*m_nColCount+x].m_hDib = NULL;
			m_pCells[y*m_nColCount+x].m_nType = CELL_EMPTY;
		}

	return true;
}

BOOL	CWndGrid::ClearAllNonFixed()
{
	for(int x=m_nFixedCols; x<m_nColCount; x++)
		for(int y=m_nFixedRows; y<m_nRowCount; y++)
		{
			m_pCells[y*m_nColCount+x].m_sText = "";
			m_pCells[y*m_nColCount+x].m_hDib = NULL;
			m_pCells[y*m_nColCount+x].m_nType = CELL_EMPTY;
		}

	return true;
}

CString	CWndGrid::GetItemText(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemText: row index is out of range.\n"));
		return "";
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemText: column index is out of range.\n"));
		return "";
	}

	//if(m_pCells[nRow*m_nColCount + nColumn].m_nType != CELL_TEXT)
	//	return "";

	return m_pCells[nRow*m_nColCount + nColumn].m_sText;
}

double CWndGrid::GetItemDouble(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemDouble: row index is out of range.\n"));
		return -VAL_UNDEF;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemDouble: column index is out of range.\n"));
		return -VAL_UNDEF;
	}

	if(	m_pCells[nRow*m_nColCount + nColumn].m_nType != CELL_TEXT ||
		m_pCells[nRow*m_nColCount + nColumn].m_sText.IsEmpty())
		return -VAL_UNDEF;

	TCHAR* stop;
	return _tcstod(m_pCells[nRow*m_nColCount + nColumn].m_sText, &stop);
}

HANDLE	CWndGrid::GetItemDIB(int nRow, int nColumn)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemDIB: row index is out of range.\n"));
		return NULL;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemDIB: column index is out of range.\n"));
		return NULL;
	}

	if(m_pCells[nRow*m_nColCount + nColumn].m_nType != CELL_BITMAP)
		return NULL;

	return m_pCells[nRow*m_nColCount + nColumn].m_hDib;
}

HBITMAP		CWndGrid::GetItemBitmap(int nRow, int nColumn)
{
/*	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemBitmap: row index is out of range.\n"));
		return NULL;
	}

	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemBitmap: column index is out of range.\n"));
		return NULL;
	}

	if(m_pCells[nRow*m_nColCount + nColumn].m_nType != CELL_BITMAP)
		return NULL;

	CDCDib	DibDC;
	DibDC.SetHandle(m_pCells[nRow*m_nColCount + nColumn].m_hDib);
	return DibDC.GetHBITMAP();*/
	return NULL;
}

void	CWndGrid::SetItemAllowEdit(int nRow, int nCol, BOOL bVal)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemAllowEdit: row index is out of range.\n"));
		return;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemAllowEdit: column index is out of range.\n"));
		return;
	}
	
	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: SetItemAllowEdit: Fixed row items cannot have Combo Values.\n"));
		return;
	}

	if(IsColumnFixed(nCol))
	{
		TRACE(_T("Grid: SetItemAllowEdit: Fixed column items cannot be editable.\n"));
		return;
	}

	CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];
	pCell->m_bAllowEdit = bVal;
}

BOOL	CWndGrid::GetItemAllowEdit(int nRow, int nCol)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemAllowEdit: row index is out of range.\n"));
		return false;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemAllowEdit: column index is out of range.\n"));
		return false;
	}

	CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];
	return pCell->m_bAllowEdit;
}

void	CWndGrid::SetColumnAllowEdit(int nCol, BOOL bVal)
{
	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: SetColumnAllowEdit: column index is out of range.\n"));
		return;
	}

	if(IsColumnFixed(nCol))
	{
		TRACE(_T("Grid: SetColumnAllowEdit: Fixed column items cannot be editable.\n"));
		return;
	}

	for(int row=m_nFixedRows; row<m_nRowCount; row++)
	{
		CCell* pCell = &m_pCells[row*m_nColCount+nCol];
		pCell->m_bAllowEdit = bVal;
	}

}

void	CWndGrid::SetRowAllowEdit(int nRow, BOOL bVal)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetRowAllowEdit: row index is out of range.\n"));
		return;
	}
	
	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: SetRowAllowEdit: Fixed row items cannot be editable.\n"));
		return;
	}

	for(int col=m_nFixedCols; col<m_nColCount; col++)
	{
		CCell* pCell = &m_pCells[nRow*m_nColCount+col];
		pCell->m_bAllowEdit = bVal;
	}
}

void	CWndGrid::SetAllItemsAllowEdit(BOOL bVal)
{
	for(int row=m_nFixedRows; row<m_nRowCount; row++)
		for(int col=m_nFixedCols; col<m_nColCount; col++)
		{
			CCell* pCell = &m_pCells[row*m_nColCount+col];
			pCell->m_bAllowEdit = bVal;
		}
}

void	CWndGrid::SetItemAllowComboBox(int nRow, int nCol, BOOL bVal)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetItemAllowComboBox: row index is out of range.\n"));
		return;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: SetItemAllowComboBox: column index is out of range.\n"));
		return;
	}
	
	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: SetItemAllowComboBox: Fixed row items cannot have Combo Values.\n"));
		return;
	}

	if(IsColumnFixed(nCol))
	{
		TRACE(_T("Grid: SetItemAllowComboBox: Fixed column items cannot have Combo Values.\n"));
		return;
	}

	CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];
	pCell->m_bAllowCombo = bVal;
	if(bVal)
		pCell->m_nType = CELL_TEXT;

}

BOOL	CWndGrid::GetItemAllowComboBox(int nRow, int nCol)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: GetItemAllowComboBox: row index is out of range.\n"));
		return false;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: GetItemAllowComboBox: column index is out of range.\n"));
		return false;
	}

	CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];
	return pCell->m_bAllowCombo;
}

void	CWndGrid::SetColumnAllowComboBox(int nCol, BOOL bVal)
{
	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: SetColumnAllowComboBox: column index is out of range.\n"));
		return;
	}

	if(IsColumnFixed(nCol))
	{
		TRACE(_T("Grid: SetColumnAllowComboBox: Fixed column items cannot have Combo Values.\n"));
		return;
	}

	for(int row=m_nFixedRows; row<m_nRowCount; row++)
	{
		CCell* pCell = &m_pCells[row*m_nColCount+nCol];
		pCell->m_bAllowCombo = bVal;
		if(bVal)
			pCell->m_nType = CELL_TEXT;
	}

}

void	CWndGrid::SetRowAllowComboBox(int nRow, BOOL bVal)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SetRowAllowComboBox: row index is out of range.\n"));
		return;
	}
	
	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: SetRowAllowComboBox: Fixed row items cannot have Combo Values.\n"));
		return;
	}

	for(int col=m_nFixedCols; col<m_nColCount; col++)
	{
		CCell* pCell = &m_pCells[nRow*m_nColCount+col];
		pCell->m_bAllowCombo = bVal;
		if(bVal)
			pCell->m_nType = CELL_TEXT;
	}

}

int		CWndGrid::AddComboString(int nRow, int nCol, LPCTSTR szValue)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: AddComboString: row index is out of range.\n"));
		return -1;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: AddComboString: column index is out of range.\n"));
		return -1;
	}
	
	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: AddComboString: Fixed row items cannot have Combo Values.\n"));
		return -1;
	}

	if(IsColumnFixed(nCol))
	{
		TRACE(_T("Grid: AddComboString: Fixed column items cannot have Combo Values.\n"));
		return -1;
	}

	CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];
	return pCell->m_pComboValues->Add(CString(szValue));
}

void	CWndGrid::ResetComboContent(int nRow, int nCol)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: ResetComboContent: row index is out of range.\n"));
		return;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: ResetComboContent: column index is out of range.\n"));
		return;
	}
	
	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: ResetComboContent: Fixed row items cannot have Combo Values.\n"));
		return;
	}

	if(IsColumnFixed(nCol))
	{
		TRACE(_T("Grid: ResetComboContent: Fixed column items cannot have Combo Values.\n"));
		return;
	}

	CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];
	pCell->m_pComboValues->RemoveAll();
}

void	CWndGrid::SetDefaultColorNonFixedBk(COLORREF color)
{
	m_colorNonFixedBk = color;
}

void	CWndGrid::SetDefaultColorFixedBk(COLORREF color)
{
	m_colorFixedBk = color;
}

void	CWndGrid::SetDefaultColorGridNonFixed(COLORREF color)
{
	m_colorGrid = color;
}

void	CWndGrid::SetDefaultColorGridFixed(COLORREF color)
{
	m_colorGridFixed = color;	
}

void	CWndGrid::SetDefaultWidthNonFixedGridLine(int nWidth)
{
	m_nGridLineWidth = nWidth;
}

void	CWndGrid::SetDefaultWidthFixedGridLine(int nWidth)
{
	m_nFixedGridLineWidth = nWidth;
}

void	CWndGrid::SetDefaultColorSelection(COLORREF color)
{
	m_colorSelection = color;
}

void	CWndGrid::SetDefaultColumnSize(int nSize)
{
	if(nSize <= 0)
		return;

	m_nDefaultSizeX = nSize;
}

void	CWndGrid::SetDefaultRowSize(int nSize)
{
	if(nSize <= 0)
		return;

	m_nDefaultSizeY = nSize;
}

void	CWndGrid::SetDefaultFont(LOGFONT* pLF)
{
	if(!pLF)
	{
		TRACE(_T("Grid: SetDefaultFont: Invalid Font.\n"));
		return;		
	}

	if(m_DefaultFont.m_hObject)
		m_DefaultFont.DeleteObject();

	m_DefaultFont.CreateFontIndirect(pLF);
}

COLORREF	CWndGrid::GetDefaultColorNonFixedBk()
{
	return m_colorNonFixedBk;	
}

COLORREF	CWndGrid::GetDefaultColorFixedBk()
{
	return m_colorFixedBk;
}

COLORREF	CWndGrid::GetDefaultColorGridNonFixed()
{
	return m_colorGrid;
}

COLORREF	CWndGrid::GetDefaultColorGridFixed()
{
	return m_colorGridFixed;
}

COLORREF	CWndGrid::GetDefaultColorSelection()
{
	return m_colorSelection;
}

int			CWndGrid::GetDefaultWidthNonFixedGridLine()
{
	return m_nGridLineWidth;
}

int			CWndGrid::GetDefaultWidthFixedGridLine()
{
	return m_nFixedGridLineWidth;
}

int			CWndGrid::GetDefaultColumnSize()
{
	return m_nDefaultSizeX;
}

int			CWndGrid::GetDefaultRowSize()
{
	return m_nDefaultSizeY;
}

void		CWndGrid::GetDefaultFont(LOGFONT* pLF)
{
	if(!pLF)
	{
		TRACE(_T("Grid: GetDefaultFont: Invalid Parameter.\n"));
		return;		
	}

	m_DefaultFont.GetLogFont(pLF);
}

BOOL	CWndGrid::SelectItem(int nRow, int nCol, BOOL bRedraw)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SelectItem: row index is out of range.\n"));
		return false;
	}

	if(nCol < 0 || nCol >= m_nColCount)
	{
		TRACE(_T("Grid: SelectItem: column index is out of range.\n"));
		return false;
	}

	if(IsColumnFixed(nCol) || IsRowFixed(nRow))
	{
		TRACE(_T("Grid: SelectItem: Unable to select Item in Fixed Row or Column.\n"));
		return false;
	}

	m_Selection.Set(nCol, nRow);
	
	if(bRedraw)
		Invalidate();
	return true;
}

BOOL	CWndGrid::SelectRange(int nStartRow, int nStartCol, int nEndRow, int nEndCol, BOOL bRedraw)
{
	nStartRow = max(m_nFixedRows, min(m_nRowCount, nStartRow));
	nEndRow = max(m_nFixedRows, min(m_nRowCount, nEndRow));
	nStartCol = max(m_nFixedCols, min(m_nColCount, nStartCol));
	nEndCol = max(m_nFixedCols, min(m_nColCount, nEndCol));

	m_Selection.Set(nStartCol,nStartRow,nEndCol,nEndRow);

	if(bRedraw)
		Invalidate();
	return true;
}

BOOL	CWndGrid::SelectRow(int nRow, BOOL bRedraw)
{
	if(nRow < 0 || nRow >= m_nRowCount)
	{
		TRACE(_T("Grid: SelectRow: row index is out of range.\n"));
		return false;
	}

	if(IsRowFixed(nRow))
	{
		TRACE(_T("Grid: SelectRow: Unable to select Items in Fixed Row.\n"));
		return false;
	}

	m_Selection.Set(m_nFixedCols, nRow, m_nColCount, nRow);

	if(bRedraw)
		Invalidate();
	return true;
}

BOOL	CWndGrid::SelectColumn(int nColumn, BOOL bRedraw)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SelectColumn: column index is out of range.\n"));
		return false;
	}

	if(IsColumnFixed(nColumn))
	{
		TRACE(_T("Grid: SelectColumn: Unable to select Items in Fixed Column.\n"));
		return false;
		
	}

	m_Selection.Set(nColumn, m_nFixedRows, nColumn, m_nRowCount);

	if(bRedraw)
		Invalidate();
	return true;
}

BOOL	IsString(CString str)
{
	for(int i=0;i<str.GetLength();i++)
		if(	str[i]=='0' ||
			str[i]=='1' ||
			str[i]=='2' ||
			str[i]=='3' ||
			str[i]=='4' ||
			str[i]=='5' ||
			str[i]=='6' ||
			str[i]=='7' ||
			str[i]=='8' ||
			str[i]=='9' ||
			str[i]=='.' ||
			str[i]=='e' ||
			str[i]=='E' ||
			str[i]=='+' ||
			str[i]=='-')
			continue;
		else
			return TRUE;
	return FALSE;
}

BOOL CWndGrid::IsStringRow(int nRow)
{
	BOOL bString = FALSE;
	int nSizeCol = GetColumnCount();
	for(int nCol = 1;nCol < nSizeCol;nCol++)
	{
		CString str = GetItemText(nRow,nCol);
		str.TrimLeft();
		str.TrimRight();
		if(IsString(str))
			return TRUE;
	}
	return FALSE;

}

BOOL CWndGrid::HideSelection(BOOL bRedraw)
{
	m_Selection.Set(-1,-1);
	if(bRedraw)
		Invalidate();
	return true;
}

BOOL	CWndGrid::SetSelection(int nStartRow, int nStartCol, int nEndRow, int nEndCol)
{
	m_Selection.nStartY = nStartRow;
	m_Selection.nStartX = nStartCol;
	m_Selection.nEndY = nEndRow;
	m_Selection.nEndX = nEndCol;

	m_nActiveCellCol = nStartCol;
	m_nActiveCellRow = nStartRow;

	ScrollTopToRow(nStartRow);
	ScrollLeftToColumn(nStartCol);
	Invalidate();

	return true;
}

BOOL	CWndGrid::GetSelectionRange(int &nStartRow, int& nStartCol, int &nEndRow, int &nEndCol)
{
	nStartRow = m_Selection.nStartY;
	nStartCol = m_Selection.nStartX;
	nEndRow = m_Selection.nEndY;
	nEndCol = m_Selection.nEndX;

	return true;
}

BOOL	CWndGrid::GetSelectionNormalizedRange(int &nStartRow, int& nStartCol, int &nEndRow, int &nEndCol)
{
	nStartRow = min(m_Selection.nStartY, m_Selection.nEndY);
	nStartCol = min(m_Selection.nStartX, m_Selection.nEndX);
	nEndRow = max(m_Selection.nStartY, m_Selection.nEndY);
	nEndCol = max(m_Selection.nStartX, m_Selection.nEndX);

	return true;
}

BOOL	CWndGrid::CopySelectionToClipboard()
{
	int nX1, nX2, nY1, nY2;
	GetSelectionNormalizedRange(nY1,nX1,nY2,nX2);
	
	CString sCopy;
	for(int y=nY1; y<=nY2; y++)
	{
		if(y > nY1)
			sCopy += TCHAR(13);//"\r";

		for(int x=nX1; x<=nX2; x++)
		{
			if(x>nX1)
				sCopy += "	";
			sCopy += m_pCells[y*m_nColCount+x].m_sText;
		}
	}

	int nSize = (sCopy.GetLength()+1)*sizeof(TCHAR);
	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, nSize);
	TCHAR* szCopy = (TCHAR*)GlobalLock(hMem);
	_tcscpy(szCopy, sCopy.GetBuffer(0));
	sCopy.ReleaseBuffer();

	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	
	return true;
}

BOOL	CWndGrid::PasteFromClipboard()
{
	return false;
}

// external scrolling
BOOL	CWndGrid::ScrollTopToRow(int nRow)
{
	nRow -= m_nFixedRows;

	int nMin, nMax;
	GetScrollRange(SB_VERT, &nMin, &nMax);
	nRow = max(nMin, min(nMax, nRow));

	int nVisibleStart = GetFirstVisibleRow();
	int nVisibleEnd = GetLastVisibleRow();
	if( !(nRow >= nVisibleStart && nRow <= nVisibleEnd) )
	{
		SetScrollPos(SB_VERT, nRow);
		Invalidate();
	}

	return true;
}

BOOL	CWndGrid::ScrollLeftToColumn(int nColumn)
{
	nColumn -= m_nFixedCols;

	int nMin, nMax;
	GetScrollRange(SB_HORZ, &nMin, &nMax);
	nColumn = max(nMin, min(nMax, nColumn));

	int nVisibleStart = GetFirstVisibleColumn();
	int nVisibleEnd = GetLastVisibleColumn();
	if( !(nColumn >= nVisibleStart && nColumn <= nVisibleEnd) )
	{
		SetScrollPos(SB_HORZ, nColumn);
		Invalidate();
	}
	return true;
}

int		CWndGrid::GetFirstVisibleRow()
{
	return GetScrollPos(SB_VERT)+m_nFixedRows;
}

int		CWndGrid::GetFirstVisibleColumn()
{
	return GetScrollPos(SB_HORZ)+m_nFixedCols;
}

int		CWndGrid::GetLastVisibleRow()
{
	CRect rc;
	GetClientRect(rc);

	int nScrollY = GetScrollPos(SB_VERT);

	int nY = 0;
	int nIndex = 0;
	while(nY <= rc.bottom && nIndex < m_nRowCount)
	{
		if(!IsRowFixed(nIndex) && nIndex-m_nFixedRows < nScrollY)
		{
			++nIndex;
			continue;
		}
		nY += m_pRowSizes[nIndex];
		nIndex++;
	}

	return nIndex;
}

int		CWndGrid::GetLastVisibleColumn()
{
	CRect rc;
	GetClientRect(rc);

	int nScrollX = GetScrollPos(SB_HORZ);

	int nX = 0;
	int nIndex = 0;
	while(nX <= rc.right && nIndex < m_nColCount)
	{
		if(!IsColumnFixed(nIndex) && nIndex-m_nFixedCols < nScrollX)
		{
			++nIndex;
			continue;
		}
		nX += m_pColumnSizes[nIndex];
		nIndex++;
	}

	return nIndex;
}

void	CWndGrid::DrawEmptyGrid(CDC* pDC, CRect rc)
{
	if(m_nGridLineWidth==0)
	{
		pDC->FrameRect(rc,&CBrush(m_colorOutOfGridBk));
		return;
	}

	int nScrollX = GetScrollPos(SB_HORZ);
	int nScrollY = GetScrollPos(SB_VERT);
	int nX, nY, nIndex;

	CPen pen; pen.CreatePen(PS_DOT, 1, m_colorGrid);
	CPen* pOldPen = pDC->SelectObject(&pen);

	// vertical
	nX = 0;
	nIndex = 0;
	while (nX <= rc.right)
	{
		if(!IsColumnFixed(nIndex) && nIndex-m_nFixedCols < nScrollX)
		{
			++nIndex;
			continue;
		}

		pDC->MoveTo(nX, 0);
		pDC->LineTo(nX, rc.bottom);

		if(nIndex < m_nColCount)
			nX += m_pColumnSizes[nIndex++];
		else
			nX += m_nDefaultSizeX;
	}

	// horizontal
	nY = 0;
	nIndex = 0;
	while(nY <= rc.bottom)
	{
		if(!IsRowFixed(nIndex) && nIndex-m_nFixedRows < nScrollY)
		{
			++nIndex;
			continue;
		}

		pDC->MoveTo(0, nY);
		pDC->LineTo(rc.right, nY);

		if(nIndex < m_nRowCount)
			nY += m_pRowSizes[nIndex++];
		else
			nY += m_nDefaultSizeY;
	}

	if(pOldPen)
		pDC->SelectObject(pOldPen);

	pDC->FrameRect(rc,&CBrush(m_colorOutOfGridBk));
}

void	CWndGrid::Draw(CDC* pDC, CRect rc)
{
	int nScrollX = GetScrollPos(SB_HORZ);
	int nScrollY = GetScrollPos(SB_VERT);

	pDC->FillSolidRect(rc, m_colorOutOfGridBk);
	DrawEmptyGrid(pDC, rc);

	BOOL bIsSelection = false;
	CRect rcSel(rc.right,rc.bottom,0,0);

	int nCurY = 0;
	for(int y=0; y<m_nRowCount; y++)
	{
		if(!IsRowFixed(y) && y-m_nFixedRows < nScrollY)
			continue;

		int nDY = m_pRowSizes[y];
		int nCurX = 0;
		for(int x=0; x<m_nColCount; x++)
		{
			if(!IsColumnFixed(x) && x-m_nFixedCols < nScrollX)
				continue;

			int nRealY = y;//GetRowAfterSorting(y);
			
			CCell* pCell = &m_pCells[y*m_nColCount+x];
			int nDX = m_pColumnSizes[x];
			CRect rcCell(nCurX, nCurY, nCurX+nDX, nCurY+nDY);

			// Draw cell
			pCell->Draw(this, x, y, pDC, rcCell);

			// selection rect
			if(m_Selection.InRange(x,y) && !IsColumnFixed(x) && !IsRowFixed(y))
			{
				bIsSelection = true;
				if(nCurX < rcSel.left)
					rcSel.left = nCurX;
				if(nCurX+nDX > rcSel.right)
					rcSel.right = nCurX+nDX;
				if(nCurY < rcSel.top)
					rcSel.top = nCurY;
				if(nCurY+nDY > rcSel.bottom)
					rcSel.bottom = nCurY+nDY;
			}

			nCurX += nDX;
			if(nCurX > rc.right)
				break;
		}

		nCurY += nDY;
		if(nCurY > rc.bottom)
			break;
	}

	if(bIsSelection)
	{
		CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(0,0,0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBr = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rcSel);
		if(pOldPen)
			pDC->SelectObject(pOldPen);
		if(pOldBr)
			pDC->SelectObject(pOldBr);
	}

	pDC->FrameRect(rc,&CBrush(RGB(200,200,200)));
}

void	CWndGrid::UpdateScrollSizes()
{
	if(!m_bScrollBars)
	{
		SetScrollRange(SB_HORZ, 0, 0);
		SetScrollRange(SB_VERT, 0, 0);
		return;
	}

	CRect rc;
	GetClientRect(rc);

	// horizontal
	int nTemp=0;
	int nIndex=m_nColCount;
	int nMaxSize = GetMax(m_pColumnSizes, m_nColCount);
	for(int i=0; i<m_nColCount; i++)
	{
		nTemp += nMaxSize; //m_pColumnSizes[i];
		if(nTemp > rc.right)
		{
			nIndex = i;
			break;
		}
	}
	int iMax = m_nColCount-nIndex;
	int iOldScroll = GetScrollPos(SB_HORZ);
	int iScrollPos = min(iOldScroll, iMax);
	SetScrollRange(SB_HORZ, 0, iMax);
	SetScrollPos(SB_HORZ, iScrollPos);

	// vertical
	nTemp=0;
	nIndex=m_nRowCount;
	nMaxSize = GetMax(m_pRowSizes, m_nRowCount);
	for(int i=0; i<m_nRowCount; i++)
	{
		nTemp += nMaxSize; //m_pRowSizes[i];
		if(nTemp > rc.bottom)
		{
			nIndex = i;
			break;
		}
	}
	iMax = m_nRowCount-nIndex;
	iOldScroll = GetScrollPos(SB_VERT);
	iScrollPos = min(iOldScroll, iMax);
	SetScrollRange(SB_VERT, 0, iMax);
	SetScrollPos(SB_VERT, iScrollPos);
}

CRect	CWndGrid::GetItemRect(int nRow, int nCol)
{
	if(nRow < 0 || nRow >= m_nRowCount || nCol < 0 || nCol >= m_nColCount)
		return CRect(0,0,0,0);

	int i, nX1=0, nX2, nY1=0, nY2;
	for(i=0; i<nCol; i++)
		nX1 += m_pColumnSizes[i];
	nX2 = nX1 + m_pColumnSizes[i];

	for(i=0; i<nRow; i++)
		nY1 += m_pRowSizes[i];
	nY2 = nY1 + m_pRowSizes[i];

	return CRect(nX1, nY1, nX2, nY2);
}

int		CWndGrid::HitTest(CPoint pt, int &nRow, int &nCol, CRect *pCellRect)
{
	nRow = nCol = -1;
	int nRet = HIT_NOWHERE;

	int nScrollX = GetScrollPos(SB_HORZ);
	int nScrollY = GetScrollPos(SB_VERT);

	int nX, nOldX, nY, nOldY, nIndex;
	CRect rc;
	GetClientRect(rc);

	// define column
	nX = nOldX = 0;
	nIndex = 0;
	while (nX <= rc.right && nIndex < m_nColCount)
	{
		if(!IsColumnFixed(nIndex) && nIndex-m_nFixedCols < nScrollX)
		{
			++nIndex;
			continue;
		}
		nX += m_pColumnSizes[nIndex];
		
		// check for vertical line hitting
		if(pt.x >= nX-3 && pt.x <= nX+3)
		{
			nCol = nIndex;
			nRet |= HIT_ONCELL | HIT_ONLINEVERT;

			if(pCellRect)
			{
				pCellRect->left = nX-m_pColumnSizes[nIndex];;
				pCellRect->right = nX;
			}
		}

		// check for cell hitting
		if(pt.x > nOldX+3 && pt.x < nX-3)
		{
			nCol = nIndex;
			nRet |= HIT_ONCELL;

			if(pCellRect)
			{
				pCellRect->left = nX-m_pColumnSizes[nIndex];;
				pCellRect->right = nX;
			}
		}

		nOldX = nX;
		nIndex++;
	}

	// define row
	nY = nOldY = 0;
	nIndex = 0;
	while(nY <= rc.bottom && nIndex < m_nRowCount)
	{
		if(!IsRowFixed(nIndex) && nIndex-m_nFixedRows < nScrollY)
		{
			++nIndex;
			continue;
		}
		nY += m_pRowSizes[nIndex];

		// check for horizontal line hitting
		if(pt.y >= nY-3 && pt.y <= nY+3)
		{
			nRow = nIndex;
			nRet |= HIT_ONCELL | HIT_ONLINEHORZ;

			if(pCellRect)
			{
				pCellRect->top = nY-m_pRowSizes[nIndex];;
				pCellRect->bottom = nY;
			}
		}

		// check for cell hitting
		if(pt.y > nOldY+3 && pt.y < nY-3)
		{
			nRow = nIndex;
			nRet |= HIT_ONCELL;

			if(pCellRect)
			{
				pCellRect->top = nY-m_pRowSizes[nIndex];;
				pCellRect->bottom = nY;
			}
		}

		nOldY = nY;
		nIndex++;
	}

	return nRet;
}

int		CWndGrid::GetRowAfterSorting(int nRow)
{
	if(IsRowFixed(nRow) || m_nSortColumn == -1)
		return nRow;
	
	int nRet = 0;
	return nRet;
}

void	CWndGrid::CloseActiveEditControl(BOOL bUpdate)
{
	if(m_bEditing)
	{
		m_wndEdit.ShowWindow(SW_HIDE);
		m_bEditing = false;
		if(bUpdate && !m_bLocked && m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
		{
			CString sText; m_wndEdit.GetWindowText(sText);	
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_CELLCHANGED, (WPARAM)m_nActiveCellRow, (LPARAM)m_nActiveCellCol);
		}
		Invalidate();
	}	
}


BEGIN_MESSAGE_MAP(CWndGrid, CWnd)
	//{{AFX_MSG_MAP(CWndGrid)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndGrid message handlers

int CWndGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create default font
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	_tcscpy(lf.lfFaceName, _T("Arial"));
//	strcpy(lf.lfFaceName, "Courier New");
	lf.lfHeight = 15;
	m_DefaultFont.CreateFontIndirect(&lf);

	// create edit window
	m_wndEdit.Create(WS_CHILD|ES_AUTOHSCROLL, CRect(0,0,0,0), this, 1144);
	m_wndEdit.SetFont(&m_DefaultFont);

	// set default row and column counts
	SetRowCount(4);
	SetColumnCount(4);
	SetFixedRowCount(1);
	SetFixedColumnCount(1);

	srand((unsigned int)NULL);

	//SetItemText(0,1,"Item");
	/*for(int x=1; x<10; x++)
		for(int y=1; y<10000; y++)
		{
			int nn = rand()%10000;
			CString s; s.Format("%u-%u", nn, x);
			SetItemText(y,x,s);
		}*/
/*
//	this->SetFixedCellsBkColor(RGB(255,255,0));
//	this->SetNonFixedCellsBkColor(RGB(255,0,0));
//	this->SetColumnBkColor(2, RGB(169,178,202));
//	this->SetItemForeColor(1,1,RGB(255,0,255));
	SetItemText(0,0,"h/h");
	SetItemText(1,0,"1");
	SetItemText(2,0,"2");
	SetItemText(3,0,"3");
	SetItemText(4,0,"4");

	SetItemText(0,1,"Name");
	SetItemText(1,1,"David");
	SetItemText(2,1,"Petros");
	SetItemText(3,1,"Pogos");
	SetItemText(4,1,"kuku");

	SetItemText(0,2,"Value");
	SetItemText(1,2,"100");
	SetItemText(2,2,"1.258");
	SetItemText(3,2,"205");
	SetItemText(4,2,"52896");

	CBitmap *bm = new CBitmap;
	bm->LoadBitmap(IDB_HPLDMEGRID);
	SetItemBitmap(1,3,bm);

//	SelectRange(2,2,5,4);
//	ScrollTopToRow(3);

	*/	
	return 0;
}

void CWndGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc; GetClientRect(rc);
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CBitmap bm;
	bm.CreateCompatibleBitmap(&dc, rc.right, rc.bottom);
	dcMem.SelectObject(&bm);

	Draw(&dcMem, rc);	

	dc.BitBlt(0,0,rc.right,rc.bottom, &dcMem,0,0,SRCCOPY);
}

void CWndGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateScrollSizes();
}

void CWndGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();
	if(m_bEditing)
	{
		m_wndEdit.ShowWindow(SW_HIDE);
		m_bEditing = false;
		if(!m_bLocked && m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
		{
			CString sText; m_wndEdit.GetWindowText(sText);	
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_CELLCHANGED, (WPARAM)m_nActiveCellRow, (LPARAM)m_nActiveCellCol);
		}
		Invalidate();
	}

	int iScrollPos = GetScrollPos(SB_HORZ);
	int nMin, nMax;
	GetScrollRange(SB_HORZ, &nMin, &nMax);
	switch(nSBCode)
	{
	case SB_LINERIGHT:
		iScrollPos ++;
		break;
	case SB_LINELEFT:
		iScrollPos --;
		break;
	case SB_PAGERIGHT:
		iScrollPos += 3;
		break;
	case SB_PAGELEFT:
		iScrollPos -= 3;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		iScrollPos = nPos;
		break;
	}

	iScrollPos = max(nMin, min(nMax, iScrollPos));
	if(iScrollPos != GetScrollPos(SB_HORZ))
	{
		SetScrollPos(SB_HORZ, iScrollPos);
		Invalidate();
	}
	
	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CWndGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();
	if(m_bEditing)
	{
		m_wndEdit.ShowWindow(SW_HIDE);
		m_bEditing = false;
		if(!m_bLocked && m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
		{
			CString sText; m_wndEdit.GetWindowText(sText);	
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_CELLCHANGED, (WPARAM)m_nActiveCellRow, (LPARAM)m_nActiveCellCol);
		}
		Invalidate();
	}

	int iScrollPos = GetScrollPos(SB_VERT);
	int nMin, nMax;
	GetScrollRange(SB_VERT, &nMin, &nMax);
	switch(nSBCode)
	{
	case SB_LINEDOWN:
		iScrollPos ++;
		break;
	case SB_LINEUP:
		iScrollPos --;
		break;
	case SB_PAGEDOWN:
		iScrollPos += 3;
		break;
	case SB_PAGEUP:
		iScrollPos -= 3;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		iScrollPos = nPos;
		break;
	}

	iScrollPos = max(nMin, min(nMax, iScrollPos));
	if(iScrollPos != GetScrollPos(SB_VERT))
	{
		SetScrollPos(SB_VERT, iScrollPos);
		Invalidate();
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CWndGrid::PostNcDestroy() 
{	
	CWnd::PostNcDestroy();

	delete this;
}

void CWndGrid::OnDestroy() 
{
	CWnd::OnDestroy();
	
	delete [] m_pCells;	
	delete [] m_pRowSizes;
	delete [] m_pColumnSizes;
}

BOOL CWndGrid::OnEraseBkgnd(CDC* pDC) 
{
	return true;//CWnd::OnEraseBkgnd(pDC);
}

void CWndGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();
	if(m_bEditing)
	{
		m_wndEdit.ShowWindow(SW_HIDE);
		m_bEditing = false;
		if(!m_bLocked && m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
		{
			CString sText; m_wndEdit.GetWindowText(sText);	
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_CELLCHANGED, (WPARAM)m_nActiveCellRow, (LPARAM)m_nActiveCellCol);
		}
		Invalidate();
	}

	SetFocus();

	CRect rcCell;
	int nRow, nCol;
	int nHit = HitTest(point, nRow, nCol, &rcCell);
	if(nRow == -1 || nCol == -1)
	{
		m_Selection.Set(-1,-1);
		m_nActiveCellCol = m_nActiveCellRow = -1;
		Invalidate();
		if(GetParent())
			GetParent()->SendMessage(WMU_GRID_SELCHANGED, nRow, nCol);
		return;
	}

	if(nHit & HIT_ONLINEVERT && IsRowFixed(nRow))
	{
		m_nMouseMode = MM_RESIZE_COL;
		m_nResizingColumnIndex = nCol;
		m_ptOld = point;
		SetCapture();
		return;
	}

	if(nHit & HIT_ONLINEHORZ && IsColumnFixed(nCol))
	{
		m_nMouseMode = MM_RESIZE_ROW;
		m_nResizingRowIndex = nRow;
		m_ptOld = point;
		SetCapture();
		return;
	}

	if(nHit != HIT_NOWHERE && nRow != -1 && nCol != -1)
	{
		CCell* pCell = &m_pCells[nRow*m_nColCount + nCol];

		// is mode WholeRowSelect 
		if(m_bWholeRowSel)
		{
			if(IsRowFixed(nRow))
				m_Selection.Set(-1,-1);
			else
			{
				m_nActiveCellCol = nCol;
				m_nActiveCellRow = nRow;
				m_Selection.Set(m_nFixedCols, nRow, m_nColCount, nRow);
			}
			// Open Combo
			if(pCell->m_bAllowCombo && !IsRowFixed(nRow) && !IsColumnFixed(nCol))
				m_combo.Show(this, rcCell, pCell->m_pComboValues, pCell->m_sText, pCell->m_bAllowEdit);
			Invalidate();
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_SELCHANGED, nRow, nCol);
			return;
		}

		if(IsRowFixed(nRow) && !IsColumnFixed(nCol))
			m_Selection.Set(nCol, m_nFixedRows, nCol, m_nRowCount-1);
		
		if(IsColumnFixed(nCol) && !IsRowFixed(nRow))
			m_Selection.Set(m_nFixedCols, nRow, m_nColCount, nRow);

		if(!IsRowFixed(nRow) && !IsColumnFixed(nCol))
		{
			m_nActiveCellCol = nCol;
			m_nActiveCellRow = nRow;
			m_Selection.Set(nCol, nRow);
		}
		Invalidate();
		if(GetParent())
			GetParent()->SendMessage(WMU_GRID_SELCHANGED, nRow, nCol);

		// Open Combo
		if(pCell->m_bAllowCombo && !IsRowFixed(nRow) && !IsColumnFixed(nCol))
		{
			m_combo.Show(this, rcCell, pCell->m_pComboValues, pCell->m_sText, pCell->m_bAllowEdit);
			return;
		}

		m_nMouseMode = MM_SELECT;
		SetCapture();
	}
	else
	{
		m_nMouseMode = MM_NONE;
		m_Selection.Set(-1,-1);
		Invalidate();
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CWndGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_nMouseMode = MM_NONE;
	ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CWndGrid::OnMouseMove(UINT nFlags, CPoint point) 
{
	switch(m_nMouseMode)
	{
	case MM_SELECT:
		{
			int nRow, nCol;
			int nHit = HitTest(point, nRow, nCol);

			if(nHit != HIT_NOWHERE && nRow != -1 && nCol != -1)
			{
				if(IsRowFixed(nRow) && !IsColumnFixed(nCol))
					m_Selection.SetEndX(nCol);
				
				if(IsColumnFixed(nCol) && !IsRowFixed(nRow))
					m_Selection.SetEndY(nRow);

				if(!IsRowFixed(nRow) && !IsColumnFixed(nCol))
					m_Selection.SetEnd(nCol, nRow);
				
				Invalidate();
			}
		}
		break;

	case MM_RESIZE_COL:
		{
			CRect rcClient;
			GetClientRect(rcClient);
			point.x = min(point.x, rcClient.right);
			
			int nDelta = point.x - m_ptOld.x;
			m_pColumnSizes[m_nResizingColumnIndex] += nDelta;
			if(m_pColumnSizes[m_nResizingColumnIndex] < 0)
				m_pColumnSizes[m_nResizingColumnIndex] = 0;
			else
				m_ptOld = point;
			Invalidate();
			UpdateScrollSizes();
		}
		break;

	case MM_RESIZE_ROW:
		{
			CRect rcClient;
			GetClientRect(rcClient);
			point.y = min(point.y, rcClient.bottom);

			int nDelta = point.y - m_ptOld.y;
			m_pRowSizes[m_nResizingRowIndex] += nDelta;
			if(m_pRowSizes[m_nResizingRowIndex] < 0)
				m_pRowSizes[m_nResizingRowIndex] = 0;
			else
				m_ptOld = point;
			Invalidate();
			UpdateScrollSizes();
		}
		break;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CWndGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(nHitTest != HTCLIENT)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	CPoint pt;	
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	int nRow, nCol;
	int nHit = HitTest(pt, nRow, nCol);

	if(nRow == -1 || nCol == -1)
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	if(nHit & HIT_ONLINEVERT && IsRowFixed(nRow))
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_RESIZE_COL));
		return true;
	}

	if(nHit & HIT_ONLINEHORZ && IsColumnFixed(nCol))
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_RESIZE_ROW));
		return true;
	}

	if(nHit != HIT_NOWHERE && IsColumnFixed(nCol))
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_FIXEDCOL));
		return true;
	}

	if(nHit != HIT_NOWHERE && IsRowFixed(nRow))
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_FIXEDROW));
		return true;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CWndGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect rcCell;
	int nRow, nCol;
	int nHit = HitTest(point, nRow, nCol, &rcCell);
	if(nRow == -1 || nCol == -1)
		return ;

	CCell* pCell = &m_pCells[nRow*m_nColCount+nCol];

	if( pCell->m_bAllowEdit && pCell->m_nType != CELL_BITMAP && 
		nHit != HIT_NOWHERE && !IsRowFixed(nRow) && !IsColumnFixed(nCol))
	{
		rcCell.DeflateRect(1,1);
		if(m_pCells[nRow*m_nColCount+nCol].m_hFont)
			::SendMessage(m_wndEdit.m_hWnd, WM_SETFONT, (WPARAM)m_pCells[nRow*m_nColCount+nCol].m_hFont, (LPARAM)true);
		m_wndEdit.Show(rcCell, m_pCells[nRow*m_nColCount+nCol].m_sText);
		m_wndEdit.SetSel(0,-1);
		m_nActiveCellRow = nRow;
		m_nActiveCellCol = nCol;
		m_bEditing = true;
	}

	if(GetParent())
	{
		if(IsRowFixed(nRow))
			GetParent()->SendMessage(WMU_GRID_HEADER_DBLCLK, (WPARAM)nCol);
		else
			GetParent()->SendMessage(WMU_GRID_CELLDBLCLK, (WPARAM)nRow, (LPARAM)nCol);
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

LRESULT CWndGrid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WMU_SETTEXT)
	{
		if(m_nActiveCellRow == -1 || m_nActiveCellCol == -1)
			return false;

		if(lParam)
		{
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = (LPCTSTR)wParam;
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_CELLCHANGED, (WPARAM)m_nActiveCellRow, (LPARAM)m_nActiveCellCol);
		}
		
		m_wndEdit.ShowWindow(SW_HIDE);
		m_bEditing = false;
		Invalidate();
		return true;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CWndGrid::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_CHAR)
	{
		if(m_nActiveCellRow != -1 && m_nActiveCellCol != -1 && !m_bEditing)
		{
			int nRow = m_nActiveCellRow;
			int nCol = m_nActiveCellCol;

			CCell* pCell = &m_pCells[nRow*m_nColCount+nCol];

			if( pCell->m_bAllowEdit && pCell->m_nType != CELL_BITMAP && 
				!IsRowFixed(nRow) && !IsColumnFixed(nCol))
			{
				CRect rcCell = GetItemRect(nRow, nCol);
				rcCell.DeflateRect(1,1);
				if(m_pCells[nRow*m_nColCount+nCol].m_hFont)
					::SendMessage(m_wndEdit.m_hWnd, WM_SETFONT, (WPARAM)m_pCells[nRow*m_nColCount+nCol].m_hFont, (LPARAM)true);

				CString sText; sText.Format("%c", pMsg->wParam);
				m_wndEdit.Show(rcCell, sText);
				m_wndEdit.SetSel(1,1);
				
				m_bEditing = true;
			}
		}
	}

	if(pMsg->message == WM_KEYDOWN && m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
	{
		if(pMsg->wParam == VK_LEFT)
		{
			CloseEditing(true);
			if(m_nActiveCellCol > m_nFixedCols)
			{
				--m_nActiveCellCol;
				ScrollLeftToColumn(m_nActiveCellCol);

				m_Selection.Set(m_nActiveCellCol, m_nActiveCellRow);
				Invalidate();
				
				if(GetParent())
					GetParent()->SendMessage(WMU_GRID_SELCHANGED, m_nActiveCellRow, m_nActiveCellCol);
			}
			return true;
		}

		if(pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_TAB)
		{
			CloseEditing(true);
			if(m_nActiveCellCol < m_nColCount-1)
			{
				++m_nActiveCellCol;
				ScrollLeftToColumn(m_nActiveCellCol);

				m_Selection.Set(m_nActiveCellCol, m_nActiveCellRow);
				Invalidate();
				
				if(GetParent())
					GetParent()->SendMessage(WMU_GRID_SELCHANGED, m_nActiveCellRow, m_nActiveCellCol);
			}
			return true;
		}

		if(pMsg->wParam == VK_UP)
		{
			CloseEditing(true);
			if(m_nActiveCellRow > m_nFixedRows)
			{
				--m_nActiveCellRow;
				ScrollTopToRow(m_nActiveCellRow);

				m_Selection.Set(m_nActiveCellCol, m_nActiveCellRow);
				Invalidate();
				
				if(GetParent())
					GetParent()->SendMessage(WMU_GRID_SELCHANGED, m_nActiveCellRow, m_nActiveCellCol);
			}
			return true;
		}

		if(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_RETURN)
		{
			CloseEditing(true);
			if(m_nActiveCellRow < m_nRowCount-1)
			{
				++m_nActiveCellRow;
				ScrollTopToRow(m_nActiveCellRow);

				m_Selection.Set(m_nActiveCellCol, m_nActiveCellRow);
				Invalidate();
				
				if(GetParent())
					GetParent()->SendMessage(WMU_GRID_SELCHANGED, m_nActiveCellRow, m_nActiveCellCol);
			}
			return true;
		}

		if(pMsg->wParam == VK_ESCAPE)
		{
			CloseEditing(false);
			return true;
		}

	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CWndGrid::CloseEditing(BOOL bOK)
{
	if(m_combo.m_hWnd)
		m_combo.DestroyWindow();
	if(m_bEditing)
	{
		CString sText = m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText;

		m_wndEdit.ShowWindow(SW_HIDE);
		m_bEditing = false;
		if(bOK && !m_bLocked && m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
		{
			CString sText; m_wndEdit.GetWindowText(sText);	
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
			if(GetParent())
				GetParent()->SendMessage(WMU_GRID_CELLCHANGED, (WPARAM)m_nActiveCellRow, (LPARAM)m_nActiveCellCol);
		}
		if(!bOK)
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
		Invalidate();
	}
}

void CWndGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
/*	CHPLSTGridCtrlCtrl* pCtrl = (CHPLSTGridCtrlCtrl*)GetParent();
	if(m_bEditing)
	{
		m_bEditing = false;
		if(m_nActiveCellRow != -1 && m_nActiveCellCol != -1)
		{
			CString sText; m_wndEdit.GetWindowText(sText);
			m_wndEdit.ShowWindow(SW_HIDE);
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_nType = CELL_TEXT;
			m_pCells[m_nActiveCellRow*m_nColCount+m_nActiveCellCol].m_sText = sText;
			
			pCtrl->ItemChanged(m_nActiveCellRow, m_nActiveCellCol);
		}
		Invalidate();
	}

	int nRow, nCol;
	int nHit = HitTest(point, nRow, nCol);
	if(nRow == -1 || nCol == -1)
		return ;

	pCtrl->RButtonDown(nRow, nCol);


	if(	nHit != HIT_NOWHERE && nRow != -1 && nCol != -1 &&
		!m_Selection.InRange(nCol, nRow))
	{
		if(IsRowFixed(nRow) && !IsColumnFixed(nCol))
			m_Selection.Set(nCol, m_nFixedRows, nCol, m_nRowCount);
		
		if(IsColumnFixed(nCol) && !IsRowFixed(nRow))
			m_Selection.Set(m_nFixedCols, nRow, m_nColCount, nRow);

		if(!IsRowFixed(nRow) && !IsColumnFixed(nCol))
			m_Selection.Set(nCol, nRow);
		
		Invalidate();
	}

	CPoint pt;
	GetCursorPos(&pt);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_POPUP);
	CMenu* pPopup = menu.GetSubMenu(0);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this); 
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

*/
	int nRow, nCol;
	int nHit = HitTest(point, nRow, nCol);
	if(nHit != HIT_NOWHERE && nRow != -1 && nCol != -1)
	{
		if(!IsRowFixed(nRow) && !IsColumnFixed(nCol))
		{
			m_nActiveCellCol = nCol;
			m_nActiveCellRow = nRow;
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
	
}

/*void CWndGrid::OnPopupCopy() 
{
	CopySelectionToClipboard();	
}*/

BOOL CWndGrid::SortByColumns(CArray<int,int>& arColumns, CArray<BOOL, BOOL> &arModes)
{
	if(arColumns.GetSize() != arModes.GetSize())
	{
		TRACE(_T("Grid: SortByColumns: Column index array size is not equal to Mode array size:\n"));
		return false;
	}

	int nSize = arColumns.GetSize();
	for(int i=0; i<nSize; i++)
		if(!SortColumnByPrevGrouping(arColumns[i], i==0 ? -1 : arColumns[i-1], arModes[i]))
			return false;

	Invalidate();
	return true;	
}

BOOL CWndGrid::SortColumnByPrevGrouping(int nColumn, int nPrev, BOOL bAscending)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SortColumnByPrevGrouping: Column index is out of range:\n"));
		return false;
	}

	if(nPrev == -1)
		return SortByColumn(nColumn, bAscending);

	BOOL bIsPrevString = IsColumnString(nPrev);
	BOOL bIsString = IsColumnString(nColumn);

	/* there is already one or more columns sorted, we must sort now group by group */
	int nFirstRow = m_nFixedRows;
	int nLastRow = m_nFixedRows+1;
	while(1)
	{
		if(bIsPrevString)
		{
			CString sVal = CELL(nFirstRow, nPrev).m_sText;
			while(nLastRow < m_nRowCount && CELL(nLastRow, nPrev).m_sText == sVal)
				++nLastRow;
			--nLastRow;
		}
		else
		{
			double fVal = GetItemDouble(nFirstRow, nPrev);
			while(nLastRow < m_nRowCount && GetItemDouble(nLastRow, nPrev) == fVal)
				++nLastRow;
			--nLastRow;
		}

		// sort the group
		if(nLastRow != nFirstRow)
		{
			if(bIsString)
			{
				while (1)
				{
					BOOL bContinue=false;
					SortAsText(nColumn, nLastRow, bAscending, nFirstRow, nLastRow, bContinue);
					if(!bContinue)
						break;
				}
			}
			else
			{
				while (1)
				{
					BOOL bContinue=false;
					SortAsDouble(nColumn, nLastRow, bAscending, nFirstRow, nLastRow, bContinue);
					if(!bContinue)
						break;
				}
			}
		}

		nFirstRow = nLastRow + 1;
		nLastRow = nFirstRow + 1;

		if(nLastRow >= m_nRowCount)
			break;
	}

	return true;
}

BOOL CWndGrid::SortByColumn(int nColumn, BOOL bAscending)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: SortByColumn: Column index is out of range:\n"));
		return false;
	}

	BOOL bIsString = IsColumnString(nColumn);

	if(bIsString)
	{
		while (1)
		{
			BOOL bContinue=false;
			SortAsText(nColumn, m_nRowCount, bAscending, m_nFixedRows, -1, bContinue);

			if(!bContinue)
				break;
		}
	}
	else
	{
		while (1)
		{
			BOOL bContinue=false;
			SortAsDouble(nColumn, m_nRowCount, bAscending, m_nFixedRows, -1, bContinue);

			if(!bContinue)
				break;
		}
	}
	
	Invalidate();
	return true;
}

BOOL CWndGrid::SortAsText(int nColumn, int nSize, BOOL bAscending, int low, int high, BOOL &bContniue)
{
	if (high == -1) 
		high = nSize - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) 
		return FALSE;

	CCell* pItem = &CELL((lo+hi)/2, nColumn);

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if( bAscending )
			while (lo < high  && CELL(lo, nColumn).m_sText < pItem->m_sText)
				++lo;
		else
			while (lo < high && CELL(lo, nColumn).m_sText > pItem->m_sText)
				++lo;

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if( bAscending )
			while (hi > low &&  CELL(hi, nColumn).m_sText > pItem->m_sText)
				--hi;
		else
			while (hi > low && CELL(hi, nColumn).m_sText < pItem->m_sText)
				--hi;

		// If the indexes have not crossed, swap if the items are not equal
		if (lo <= hi)
		{
			// swap only if the items are not equal
			if (CELL(lo, nColumn).m_sText != CELL(hi, nColumn).m_sText)
			{
				bContniue = true;

				for(int col=0; col<m_nColCount; col++)
					ExchangeCells(&CELL(lo, col), &CELL(hi, col));
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortAsText(nColumn,nSize, bAscending, low, hi, bContniue);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortAsText(nColumn,nSize, bAscending,lo, high, bContniue);

    return true;
}

BOOL CWndGrid::SortAsDouble(int nColumn, int nSize, BOOL bAscending, int low, int high, BOOL &bContniue)
{
	if (high == -1) 
		high = nSize - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) 
		return FALSE;

	double fItem = GetItemDouble((lo+hi)/2, nColumn);

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if( bAscending )
			while (lo < high  && GetItemDouble(lo, nColumn) < fItem)
				++lo;
		else
			while (lo < high && GetItemDouble(lo, nColumn) > fItem)
				++lo;

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if( bAscending )
			while (hi > low &&  GetItemDouble(hi, nColumn) > fItem)
				--hi;
		else
			while (hi > low && GetItemDouble(hi, nColumn) < fItem)
				--hi;

		// If the indexes have not crossed, swap if the items are not equal
		if (lo <= hi)
		{
			// swap only if the items are not equal
			if (fabs(GetItemDouble(lo, nColumn) - GetItemDouble(hi, nColumn)) > FLT_EPSILON)
			{
				bContniue = true;

				for(int col=0; col<m_nColCount; col++)
					ExchangeCells(&CELL(lo, col), &CELL(hi, col));
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortAsDouble(nColumn,nSize, bAscending, low, hi, bContniue);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortAsDouble(nColumn,nSize, bAscending,lo, high, bContniue);

    return true;
}

void CWndGrid::ExchangeCellContent(CCell* pCell1, CCell* pCell2)
{
	// types
	int nType = pCell1->m_nType;
	pCell1->m_nType = pCell2->m_nType;
	pCell2->m_nType = nType;

	// strings
	CString str = pCell1->m_sText;
	pCell1->m_sText = pCell2->m_sText;
	pCell2->m_sText = str;

	// dibs
	HANDLE hDib = pCell1->m_hDib;
	pCell1->m_hDib = pCell2->m_hDib;
	pCell2->m_hDib = hDib;
}

void CWndGrid::ExchangeCells(CCell* pCell1, CCell* pCell2)
{
	// types
	int nType = pCell1->m_nType;
	pCell1->m_nType = pCell2->m_nType;
	pCell2->m_nType = nType;

	// strings
	CString str = pCell1->m_sText;
	pCell1->m_sText = pCell2->m_sText;
	pCell2->m_sText = str;

	// dibs
	HANDLE hDib = pCell1->m_hDib;
	pCell1->m_hDib = pCell2->m_hDib;
	pCell2->m_hDib = hDib;

	// fore color
	COLORREF color = pCell1->m_colorFore;
	pCell1->m_colorFore = pCell2->m_colorFore;
	pCell2->m_colorFore = color;

	// back color
	color = pCell1->m_colorBk;
	pCell1->m_colorBk = pCell2->m_colorBk;
	pCell2->m_colorBk = color;

	// font
	HFONT hFont = pCell1->m_hFont;
	pCell1->m_hFont = pCell2->m_hFont;
	pCell2->m_hFont = hFont;
}

BOOL CWndGrid::IsColumnString(int nColumn)
{
	if(nColumn < 0 || nColumn >= m_nColCount)
	{
		TRACE(_T("Grid: IsColumnString: Column index is out of range:\n"));
		return false;
	}

	for(int nRow=m_nFixedRows; nRow<m_nRowCount; nRow++)
		if(IsValueString(CELL(nRow, nColumn).m_sText))
			return true;

	return false;
}

BOOL CWndGrid::IsValueString(CString &str)
{
	for(int i=0;i<str.GetLength();i++)
		if(	str[i]=='0' ||
			str[i]=='1' ||
			str[i]=='2' ||
			str[i]=='3' ||
			str[i]=='4' ||
			str[i]=='5' ||
			str[i]=='6' ||
			str[i]=='7' ||
			str[i]=='8' ||
			str[i]=='9' ||
			str[i]=='.' ||
			str[i]=='e' ||
			str[i]=='E' ||
			str[i]=='+' ||
			str[i]=='-')
			continue;
		else
			return true;

	return false;
}

BOOL CWndGrid::SetFormat(int nRow, int nColumn, int nFormat)
{
	if(nRow < 0 || nColumn < 0 ||
		nRow >= m_nRowCount || nColumn >= m_nColCount)
		return FALSE;
	m_pCells[nRow*m_nColCount + nColumn].m_nFormat = nFormat;
	return TRUE;
}

int CWndGrid::GetMax(int* Array, int nCount)
{
	if(nCount == 0)
		return 0;
	int nMax = Array[0];

	for(int i=1; i<nCount; i++)
	{
		if(Array[i] > nMax)
			nMax = Array[i];
	}

	return nMax;
}


BOOL CWndGrid::SortTextItemsRow(int nRow, BOOL bAscending, int low, int high)
{
    if (nRow >= GetRowCount()) return FALSE;

    if (high == -1) high = GetColumnCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    CString midItem = GetItemText( nRow,(lo+hi)/2);

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && GetItemText(nRow,lo) < midItem)
                ++lo;
        else
            while (lo < high && GetItemText(nRow,lo) > midItem)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && GetItemText(nRow,hi) > midItem)
                --hi;
        else
            while (hi > low && GetItemText(nRow,hi) < midItem)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (GetItemText(nRow,lo) != GetItemText(nRow,hi))
            {
				/*CDibDC* pDCEx = GetDCEx(lo);
				if(pDCEx)
				{
					m_DCExArray[lo] = m_DCExArray[hi];
					m_DCExArray[hi] = pDCEx;
				}*/

                for (int row = 0; row < GetRowCount(); row++)
                {
                    CCell pCell = m_pCells[row+m_nColCount*lo];
                    m_pCells[row+m_nColCount*lo] = m_pCells[row+m_nColCount*hi];
                    m_pCells[row+m_nColCount*hi] = pCell;
                }
                UINT nColWidth = m_pColumnSizes[lo];
                m_pColumnSizes[lo] = m_pColumnSizes[hi];
                m_pColumnSizes[hi] = nColWidth;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortTextItemsRow(nRow, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortTextItemsRow(nRow, bAscending, lo, high);

    return TRUE;
}

BOOL CWndGrid::SortTextItemsDouble(int nCol, BOOL bAscending, int low, int high)
{
    if (nCol >= GetColumnCount()) return FALSE;

    if (high == -1) high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

	TCHAR* stop;
    double fItem = _tcstod(GetItemText( (lo+hi)/2, nCol ), &stop);

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && _tcstod(GetItemText(lo, nCol), &stop) < fItem)
                ++lo;
        else
            while (lo < high && _tcstod(GetItemText(lo, nCol), &stop) > fItem)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && _tcstod(GetItemText(hi, nCol), &stop) > fItem)
                --hi;
        else
            while (hi > low && _tcstod(GetItemText(hi, nCol), &stop) < fItem)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (_tcstod(GetItemText(lo, nCol), &stop) != _tcstod(GetItemText(hi, nCol), &stop))
            {
				/*HDIB* hDib = m_pCells[lo+m_nRowCount];
				if(pDCEx)
				{
					m_DCExArray[lo] = m_DCExArray[hi];
					m_DCExArray[hi] = pDCEx;
				}*/

                for (int col = 0; col < GetColumnCount(); col++)
                {
                  /*  CGridCell *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);*/
					CCell pCell = m_pCells[lo+m_nColCount*col];
                    m_pCells[lo+m_nColCount*col] = m_pCells[hi+m_nColCount*col];
                    m_pCells[hi+m_nColCount*col] = pCell;
                }
                UINT nRowHeight = m_pRowSizes[lo];
                m_pRowSizes[lo] = m_pRowSizes[hi];
                m_pRowSizes[hi] = nRowHeight;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortTextItemsDouble(nCol, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortTextItemsDouble(nCol, bAscending, lo, high);

    return TRUE;
}

BOOL CWndGrid::SortTextItemsDoubleRow(int nRow, BOOL bAscending, int low, int high)
{
    if (nRow >= GetRowCount()) return FALSE;

    if (high == -1) high = GetColumnCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

	TCHAR* stop;
    double fItem = _tcstod(GetItemText( nRow,(lo+hi)/2), &stop);

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && _tcstod(GetItemText(nRow,lo), &stop) < fItem)
                ++lo;
        else
            while (lo < high && _tcstod(GetItemText(nRow,lo), &stop) > fItem)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && _tcstod(GetItemText(nRow,hi), &stop) > fItem)
                --hi;
        else
            while (hi > low && _tcstod(GetItemText(nRow,hi), &stop) < fItem)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (_tcstod(GetItemText(nRow,lo ), &stop) != _tcstod(GetItemText(nRow,hi), &stop))
            {
				/*CDibDC* pDCEx = GetDCEx(lo);
				if(pDCEx)
				{
					m_DCExArray[lo] = m_DCExArray[hi];
					m_DCExArray[hi] = pDCEx;
				}*/

                for (int row = 0; row < GetRowCount(); row++)
                {
                    CCell pCell = m_pCells[row+m_nColCount*lo];
                    m_pCells[row+m_nColCount*lo] = m_pCells[row+m_nColCount*hi];
					m_pCells[row+m_nColCount*hi] = pCell;
                    
                }
                UINT nColWidth = m_pColumnSizes[lo];
                m_pColumnSizes[lo] = m_pColumnSizes[hi];
                m_pColumnSizes[hi] = nColWidth;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortTextItemsDoubleRow(nRow, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortTextItemsDoubleRow(nRow, bAscending, lo, high);

    return TRUE;
}

BOOL CWndGrid::MouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT sbCode = zDelta < 0 ? SB_LINEDOWN : SB_LINEUP;
	OnVScroll(sbCode, 0, NULL);
	return true;
}

BOOL CWndGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	UINT sbCode = zDelta < 0 ? SB_LINEDOWN : SB_LINEUP;
	OnVScroll(sbCode, 0, NULL);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
