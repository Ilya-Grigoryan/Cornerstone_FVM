#if !defined(AFX_WNDGRID_H__0A443002_2F9D_4A0A_A90F_E8EE2C2BAFC3__INCLUDED_)
#define AFX_WNDGRID_H__0A443002_2F9D_4A0A_A90F_E8EE2C2BAFC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////////////////////
// WndGrid.h : header file
//
// 22.05.2003
// D.D.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#include "wndedit.h"
#include "combo.h"

#define VAL_UNDEF			1.0E308

#define CELL_EMPTY			100
#define CELL_TEXT			101
#define CELL_BITMAP			102
#define CELL_DOUBLE			103
#define CELL_CALLBACK		104

#define HIT_NOWHERE			0
#define HIT_ONCELL			0x00000001
#define HIT_ONLINEVERT		0x00000002
#define HIT_ONLINEHORZ		0x00000004

#define MM_NONE				0
#define MM_SELECT			1
#define MM_RESIZE_COL		2
#define MM_RESIZE_ROW		3

#define WMU_CALLBACK_GETTEXT	WM_USER + 1185
#define WMU_GRID_CELLCHANGED	WM_USER + 1187
#define WMU_GRID_HEADER_DBLCLK	WM_USER + 1189
#define WMU_GRID_CELLDBLCLK		WM_USER + 1191
#define WMU_GRID_SELCHANGED		WM_USER + 1193
#define WMU_GRID_DRAWCELL		WM_USER + 1195

class CWndGrid;
/////////////////////////////////////////////////////////////////////////////
class CRange
{
public:
	CRange()	{	nStartX = nStartY = nEndX = nEndY = -1;	}
	~CRange()	{ }

	BOOL InRange(int x, int y)
	{
		return (x>=min(nStartX,nEndX) && x<=max(nStartX,nEndX) && y>=min(nStartY,nEndY) && y<=max(nStartY,nEndY));
	}

	BOOL InRangeX(int x)
	{
		return (x>=min(nStartX,nEndX) && x<=max(nStartX,nEndX));
	}

	BOOL InRangeY(int y)
	{
		return (y>=min(nStartY,nEndY) && y<=max(nStartY,nEndY));
	}

	void Set(int startX, int startY, int endX, int endY)
	{
		nStartX = startX; nStartY = startY; nEndX = endX; nEndY = endY;
	}

	void Set(int nX, int nY)
	{
		nStartX = nEndX = nX;
		nStartY = nEndY = nY;
	}

	void SetStart(int nX, int nY)
	{
		nStartX = nX; nStartY = nY;
	}

	void SetEnd(int nX, int nY)
	{
		nEndX = nX; nEndY = nY;
	}

	void SetStartX(int nX)	{nStartX = nX;}
	void SetEndX(int nX)	{nEndX = nX;}
	void SetStartY(int nY)	{nStartY = nY;}
	void SetEndY(int nY)	{nEndY = nY;}

	BOOL IsFirstCell(int x, int y)
	{
		return (x==nStartX && y==nStartY);
	}

public:
	int nStartX;
	int nStartY;
	int nEndX;
	int nEndY;
};

/////////////////////////////////////////////////////////////////////////////
class CCell
{
public:
	CCell();
	~CCell();

	void operator = (CCell& cell);
	void Draw(CWndGrid* pGrid, int x, int y, CDC* pDC, CRect rcCell);

public:
	int			m_nType;
	CString		m_sText;
	HANDLE		m_hDib;

	COLORREF	m_colorFore;
	COLORREF	m_colorBk;

	HFONT		m_hFont;
	int			m_nFormat;

	BOOL		m_bAllowEdit;
	BOOL		m_bAllowCombo;

	BOOL		m_bOwnerDraw;
	
	CStringArray	*m_pComboValues;
};

struct STRUCT_OWNERDRAW_INFO
{
	CDC*		pDC;
	CRect		rcDraw;

	CWndGrid*	pGrid;
	CCell*		pCell;
	int			nRow;
	int			nCol;
};

/////////////////////////////////////////////////////////////////////////////
// CIndexing class
class CWndGrid;
class CIndexing
{
public:
	CIndexing(CWndGrid*	pWndGrid, int nColumn);
	~CIndexing();

	void	SetCount(int nCount);
	BOOL	AddValue(LPCTSTR szVal, int nIndex);
	int		GetAscendingSortedIndex(int nIndex);
	int		GetDescendingSortedIndex(int nIndex);
	void	UpdateWhenFixedRowCountIsChanged();

protected:
	void	FindAndRemoveIndex(int nIndex);
	void	InsertAt(int nIndex, int nVal);

public:
	int					m_nColumn;
	CArray<int,int>		m_arIndexes;
	int					m_nCount;
	int					m_nDataCount;
	CWndGrid*			m_pWndGrid;
};

/////////////////////////////////////////////////////////////////////////////
// CWndGrid window
class CWndGrid : public CWnd
{
	friend class CIndexing;
	friend class CCell;
	friend class CCombo;
	friend class CWndEdit;
// Construction
public:
	CWndGrid();
	virtual ~CWndGrid();

	BOOL Create(CWnd* pParent);

public:
	// row and column count
	BOOL	SetRowCount(int nCount);
	BOOL	SetColumnCount(int nCount);
	BOOL	SetFixedRowCount(int nCount);
	BOOL	SetFixedColumnCount(int nCount);

	int		GetRowCount();
	int		GetColumnCount();
	int		GetFixedRowCount();
	int		GetFixedColumnCount();

	int		AddRow();
	int		AddColumn();
	BOOL	InsertRow(int nIndex);
	BOOL	InsertColumn(int nIndex);
	BOOL	DeleteRow(int nRow);
	BOOL	DeleteColumn(int nColumn);

	// row and column sizes
	BOOL	SetColumnSize(int nColumn, int nSize);
	BOOL	SetRowSize(int nRow, int nSize);
	int		GetColumnSize(int nColumn);
	int		GetRowSize(int nRow);

	// color management
	BOOL	SetOutOfGridBkColor(COLORREF color);
	BOOL	SetNonFixedCellsBkColor(COLORREF color);
	BOOL	SetNonFixedCellsForeColor(COLORREF color);
	BOOL	SetFixedCellsBkColor(COLORREF color);
	BOOL	SetFixedCellsForeColor(COLORREF color);
	BOOL	SetRowBkColor(int nRow, COLORREF color);
	BOOL	SetRowForeColor(int nRow, COLORREF color);
	BOOL	SetColumnBkColor(int nColumn, COLORREF color);
	BOOL	SetColumnForeColor(int nColumn, COLORREF color);
	BOOL	SetItemBkColor(int nRow, int nColumn, COLORREF color);
	BOOL	SetItemForeColor(int nRow, int nColumn, COLORREF color);
	
	COLORREF	GetItemBkColor(int nRow, int nColumn);
	COLORREF	GetItemForeColor(int nRow, int nColumn);

	// font management
	BOOL	SetCellsFont(HFONT hFont);
	BOOL	SetNonFixedCellsFont(HFONT hFont);
	BOOL	SetFixedCellsFont(HFONT hFont);
	BOOL	SetRowFont(int nRow, HFONT hFont);
	BOOL	SetColumnFont(int nColumn, HFONT hFont);
	BOOL	SetItemFont(int nRow, int nColumn, HFONT hFont);
	HFONT	GetItemFont(int nRow, int nColumn);

	// text and bitmap management
	BOOL	SetItemText(int nRow, int nColumn, LPCTSTR lpszText);
	BOOL	SetItemDIB(int nRow, int nColumn, HANDLE hDib);
	BOOL	SetItemBitmap(int nRow, int nColumn, HBITMAP hBitmap);
	BOOL	ClearItem(int nRow, int nColumn);
	BOOL	ClearRow(int nRow);
	BOOL	ClearColumn(int nColumn);
	BOOL	ClearAll();
	BOOL	ClearAllFixed();
	BOOL	ClearAllNonFixed();

	CString		GetItemText(int nRow, int nColumn);
	double		GetItemDouble(int nRow, int nColumn);
	HANDLE		GetItemDIB(int nRow, int nColumn);
	HBITMAP		GetItemBitmap(int nRow, int nColumn);

	// owner draw items
	BOOL	SetItemOwnerDraw(int nRow, int nColumn, BOOL bOwnerDraw);
	BOOL	GetItemOwnerDraw(int nRow, int nColumn);

	// editbox
	void	SetItemAllowEdit(int nRow, int nCol, BOOL bVal);
	BOOL	GetItemAllowEdit(int nRow, int nCol);
	void	SetColumnAllowEdit(int nCol, BOOL bVal);
	void	SetRowAllowEdit(int nRow, BOOL bVal);
	void	SetAllItemsAllowEdit(BOOL bVal);

	// combobox items management
	void	SetItemAllowComboBox(int nRow, int nCol, BOOL bVal);
	BOOL	GetItemAllowComboBox(int nRow, int nCol);
	void	SetColumnAllowComboBox(int nCol, BOOL bVal);
	void	SetRowAllowComboBox(int nRow, BOOL bVal);
	int		AddComboString(int nRow, int nCol, LPCTSTR szValue);
	void	ResetComboContent(int nRow, int nCol);

	// default colors, font and sizes
	void	SetDefaultColorNonFixedBk(COLORREF color);
	void	SetDefaultColorFixedBk(COLORREF color);
	void	SetDefaultColorGridNonFixed(COLORREF color);
	void	SetDefaultColorGridFixed(COLORREF color);
	void	SetDefaultWidthNonFixedGridLine(int nWidth);
	void	SetDefaultWidthFixedGridLine(int nWidth);
	void	SetDefaultColorSelection(COLORREF color);
	void	SetDefaultColumnSize(int nSize);
	void	SetDefaultRowSize(int nSize);
	void	SetDefaultFont(LOGFONT* pLF);
	
	COLORREF	GetDefaultColorNonFixedBk();
	COLORREF	GetDefaultColorFixedBk();
	COLORREF	GetDefaultColorGridNonFixed();
	COLORREF	GetDefaultColorGridFixed();
	COLORREF	GetDefaultColorSelection();
	int			GetDefaultWidthNonFixedGridLine();
	int			GetDefaultWidthFixedGridLine();
	int			GetDefaultColumnSize();
	int			GetDefaultRowSize();
	void		GetDefaultFont(LOGFONT* pLF);

	void		SetTextAlign(unsigned int nAlign)	{ m_nTextAlign = nAlign;	}
	unsigned int GetTextAlign()						{ return m_nTextAlign;		}

	// selection
	BOOL	SelectItem(int nRow, int nCol, BOOL bRedraw=true);
	BOOL	SelectRange(int nStartRow, int nStartCol, int nEndRow, int nEndCol, BOOL bRedraw=true);
	BOOL	SelectRow(int nRow, BOOL bRedraw=true);
	BOOL	SelectColumn(int nColumn, BOOL bRedraw=true);
	BOOL	HideSelection(BOOL bRedraw=true);
	BOOL	SetSelection(int nStartRow, int nStartCol, int nEndRow, int nEndCol);
	BOOL	GetSelectionRange(int &nStartRow, int& nStartCol, int &nEndRow, int &nEndCol);
	BOOL	GetSelectionNormalizedRange(int &nStartRow, int& nStartCol, int &nEndRow, int &nEndCol);
	int		GetActiveRow()	{return m_nActiveCellRow; }
	int		GetActiveCol()	{return m_nActiveCellCol; }
	BOOL	CopySelectionToClipboard();
	BOOL	PasteFromClipboard();

	// external scrolling
	void	SetEnableScrollBars(BOOL bEnable)	{ m_bScrollBars = bEnable;	}
	BOOL	GetEnableScrollBars()				{ return m_bScrollBars;		}
	BOOL	ScrollTopToRow(int nRow);
	BOOL	ScrollLeftToColumn(int nColumn);
	int		GetFirstVisibleRow();
	int		GetFirstVisibleColumn();
	int		GetLastVisibleRow();
	int		GetLastVisibleColumn();

	// miscelations
	void	SetLocked(BOOL bVal)			{m_bLocked = bVal;}
	BOOL	GetLocked()						{return m_bLocked;}
	void	SetWholeRowSelect(BOOL bVal)	{m_bWholeRowSel = bVal;}
	BOOL	GetWholeRowSelect()				{return m_bWholeRowSel;}

	BOOL	IsColumnFixed(int nColumn)		{return nColumn>=0 && nColumn<m_nFixedCols;}
	BOOL	IsRowFixed(int nRow)			{return nRow>=0 && nRow<m_nFixedRows;}
	int		HitTest(CPoint pt, int &nRow, int &nCol, CRect *pCellRect=NULL);
	int		GetRowAfterSorting(int nRow);
	void	CloseActiveEditControl(BOOL bUpdate);
	BOOL	MouseWheel(UINT nFlags, short zDelta, CPoint pt);

	BOOL	SortByColumn(int nColumn, BOOL bAscending);
	BOOL	SortByColumns(CArray<int,int>& arColumns, CArray<BOOL, BOOL> &arModes);

	BOOL	SetFormat(int nRow, int nColumn, int nFormat);

	BOOL	SortTextItemsRow(int nRow, BOOL bAscending, int low, int high);
	BOOL	SortTextItemsDouble(int nCol, BOOL bAscending, int low, int high);
	BOOL	SortTextItemsDoubleRow(int nRow, BOOL bAscending, int low, int high);
	BOOL	IsStringRow(int nRow);

	CRect	GetItemRect(int nRow, int nCol);

public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndGrid)
	protected:
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:
	void	Draw(CDC* pDC, CRect rc);

protected:
	void	DrawEmptyGrid(CDC* pDC, CRect rc);
	
	void	UpdateScrollSizes();
	BOOL	IsValueString(CString &str);
	BOOL	IsColumnString(int nColumn);
	BOOL	SortAsText(int nColumn, int nSize, BOOL bAscending, int low, int high, BOOL &bContniue);
	BOOL	SortAsDouble(int nColumn, int nSize, BOOL bAscending, int low, int high, BOOL &bContniue);
	BOOL	SortColumnByPrevGrouping(int nColumn, int nPrev, BOOL bAscending);
	void	ExchangeCellContent(CCell* pCell1, CCell* pCell2);
	void	ExchangeCells(CCell* pCell1, CCell* pCell2);

	void	CloseEditing(BOOL bOK);

	static int GetMax(int* Array, int nCount);

public:
	int			m_nActiveCellRow;
	int			m_nActiveCellCol;

protected:
	int			m_nRowCount;
	int			m_nColCount;
	int			m_nFixedRows;
	int			m_nFixedCols;
	CCell*		m_pCells;
	COLORREF	m_colorOutOfGridBk;
	COLORREF	m_colorNonFixedBk;
	COLORREF	m_colorFixedBk;
	COLORREF	m_colorGrid;
	COLORREF	m_colorGridFixed;
	COLORREF	m_colorSelection;
	int			m_nGridLineWidth;
	int			m_nFixedGridLineWidth;
	int			m_nDefaultSizeX;
	int			m_nDefaultSizeY;
	CFont		m_DefaultFont;
	int*		m_pColumnSizes;
	int*		m_pRowSizes;
	CRange		m_Selection;
	BOOL		m_bLocked;
	BOOL		m_bWholeRowSel;
	BOOL		m_bScrollBars;
	unsigned int m_nTextAlign;

	int			m_nMouseMode;
	CPoint		m_ptOld;
	int			m_nResizingColumnIndex;
	int			m_nResizingRowIndex;

	/* this two vars are for indexing and not used yet */
	int			m_nSortColumn;
	BOOL		m_bSortAscending;

	CWndEdit	m_wndEdit;
	BOOL		m_bEditing;
	CCombo		m_combo;

protected:
	//{{AFX_MSG(CWndGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDGRID_H__0A443002_2F9D_4A0A_A90F_E8EE2C2BAFC3__INCLUDED_)
