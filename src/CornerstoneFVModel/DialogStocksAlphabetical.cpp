// DialogStocksAlphabetical.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogStocksAlphabetical.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogStocksAlphabetical dialog


CDialogStocksAlphabetical::CDialogStocksAlphabetical(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogStocksAlphabetical::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStocksAlphabetical)
	//}}AFX_DATA_INIT
}


void CDialogStocksAlphabetical::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStocksAlphabetical)
	DDX_Control(pDX, IDC_TREE_TYPES, m_TreeTypes);
	DDX_Control(pDX, IDC_TREE_INDEXES, m_TreeIndexes);
	DDX_Control(pDX, IDC_TREE_RANKS, m_TreeRanks);
	DDX_Control(pDX, IDC_TREE_SECTORS, m_TreeSectors);
	DDX_Control(pDX, IDC_TREE_INDUSTRIES, m_TreeIndustries);
	DDX_Control(pDX, IDC_TAB_LISTS, m_Tab);
	DDX_Control(pDX, IDC_STATIC_TICKBORDER, m_stTickBorder);
	DDX_Control(pDX, IDC_STATIC_CORNER, m_stCornerstone);
	DDX_Control(pDX, IDC_LIST_STOCKS, m_ListTicker);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogStocksAlphabetical, CDialog)
	//{{AFX_MSG_MAP(CDialogStocksAlphabetical)
	ON_WM_SIZE()
	ON_LBN_DBLCLK(IDC_LIST_STOCKS, OnDblclkListStocks)
	ON_EN_CHANGE(IDC_EDIT_TICKER, OnChangeEditTicker)
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LISTS, OnSelchangeTabLists)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_INDUSTRIES, OnDblclkTreeIndustries)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_SECTORS, OnDblclkTreeSectors)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_RANKS, OnDblclkTreeRanks)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_INDEXES, OnDblclkTreeIndexes)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_TYPES, OnDblclkTreeTypes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStocksAlphabetical message handlers

void CDialogStocksAlphabetical::Resize()
{
	CRect rc;
	GetClientRect(rc);

	if(m_stCornerstone.m_hWnd)
		m_stCornerstone.MoveWindow(5,5,rc.right-5, 50);

	if(m_stTickBorder.m_hWnd)
		m_stTickBorder.MoveWindow(5,5+50+4,rc.right-5, 40);

	if(m_ListTicker.m_hWnd)
		m_ListTicker.MoveWindow(5,50+45+10,rc.right-5, rc.bottom-(50+45+10+10)-22);

	if(m_TreeIndustries.m_hWnd)
		m_TreeIndustries.MoveWindow(5,50+45+10,rc.right-5, rc.bottom-(50+45+10+10)-22);

	if(m_TreeSectors.m_hWnd)
		m_TreeSectors.MoveWindow(5,50+45+10,rc.right-5, rc.bottom-(50+45+10+10)-22);

	if(m_TreeRanks.m_hWnd)
		m_TreeRanks.MoveWindow(5,50+45+10,rc.right-5, rc.bottom-(50+45+10+10)-22);

	if(m_TreeIndexes.m_hWnd)
		m_TreeIndexes.MoveWindow(5,50+45+10,rc.right-5, rc.bottom-(50+45+10+10)-22);

	if(m_TreeTypes.m_hWnd)
		m_TreeTypes.MoveWindow(5,50+45+10,rc.right-5, rc.bottom-(50+45+10+10)-22);

	if(m_Tab.m_hWnd)
		m_Tab.MoveWindow(5, rc.bottom-35, rc.right-5,30);

}

void CDialogStocksAlphabetical::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize();
}

BOOL CDialogStocksAlphabetical::OnInitDialog() 
{
	CDialog::OnInitDialog();

	theApp.m_pDialogStocks = this;

	// ImageList for Tab
	CImageList* pImageList = new CImageList();
	pImageList->Create(20,20,ILC_COLOR|ILC_MASK, 6, 4);

	pImageList->Add(AfxGetApp()->LoadIcon(IDR_MODELTYPE));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_STOCK));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_SECTOR));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_RANK));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_INDEX));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_TYPE));

	m_Tab.SetImageList(pImageList);

	m_Tab.InsertItem(0, "Tickers", 0);
	m_Tab.InsertItem(1, "Industries", 1);
	m_Tab.InsertItem(2, "Sectors", 2);
	m_Tab.InsertItem(3, "Ranks", 3);
	m_Tab.InsertItem(4, "Indexes", 4);
	m_Tab.InsertItem(5, "Types", 5);

	// Tree Industries
	pImageList = new CImageList();
	pImageList->Create(18,18,ILC_COLOR|ILC_MASK, 1, 4);
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_STOCK));

	m_TreeIndustries.SetImageList(pImageList, TVSIL_NORMAL);
	m_TreeIndustries.ShowWindow(SW_HIDE);

	// Tree Sectors
	pImageList = new CImageList();
	pImageList->Create(18,18,ILC_COLOR|ILC_MASK, 1, 4);
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_SECTOR));

	m_TreeSectors.SetImageList(pImageList, TVSIL_NORMAL);
	m_TreeSectors.ShowWindow(SW_HIDE);

	// Tree Ranks
	pImageList = new CImageList();
	pImageList->Create(18,18,ILC_COLOR|ILC_MASK, 1, 4);
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_STOCK));
	m_TreeRanks.SetImageList(pImageList, TVSIL_NORMAL);
	m_TreeRanks.ShowWindow(SW_HIDE);

	// Tree Indexes
	pImageList = new CImageList();
	pImageList->Create(18,18,ILC_COLOR|ILC_MASK, 1, 4);
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_INDEX));
	m_TreeIndexes.SetImageList(pImageList, TVSIL_NORMAL);
	m_TreeIndexes.ShowWindow(SW_HIDE);

	// Tree Indexes
	pImageList = new CImageList();
	pImageList->Create(18,18,ILC_COLOR|ILC_MASK, 1, 4);
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_INDEX));
	m_TreeTypes.SetImageList(pImageList, TVSIL_NORMAL);
	m_TreeTypes.ShowWindow(SW_HIDE);

	SetupData();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogStocksAlphabetical::SetupData()
{
	// Ticker List
	m_ListTicker.ResetContent();
	for(int i=0; i<theApp.m_Universe.GetSize(); i++)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompanyByTicker(i);
		m_ListTicker.AddString((LPCTSTR)pCompany);
	}

	// Industries
	m_TreeIndustries.DeleteAllItems();
	int nSize = theApp.m_Universe.GetIndustriesCount();
	for(int i=0; i<nSize; i++)
	{
		CString sName = theApp.m_Universe.GetIndustryName(i); 
		CArray<CCompany*,CCompany*> array;
		theApp.m_Universe.GetCompaniesInIndustry(sName, array);

		sName.Format("%s (%u)", sName, array.GetSize());
		HTREEITEM hRoot = m_TreeIndustries.InsertItem(sName,0,0);

		for(int j=0; j<array.GetSize(); j++)
		{
			CCompany* pCompany = array[j];
			CString sComp; 
			sComp.Format("%s  |  %s", pCompany->GetTicker(), pCompany->GetName());
			HTREEITEM hItem = m_TreeIndustries.InsertItem(sComp, 1,1, hRoot);
			m_TreeIndustries.SetItemData(hItem, (DWORD)pCompany);
		}
	}

	// Sectors
	m_TreeSectors.DeleteAllItems();
	nSize = theApp.m_Universe.GetSectorsCount();
	for(int i=0; i<nSize; i++)
	{
		CString sName = theApp.m_Universe.GetSectorName(i); 
		CArray<CCompany*,CCompany*> array;
		theApp.m_Universe.GetCompaniesInSector(sName, array);

		sName.Format("%s (%u)", sName, array.GetSize());
		HTREEITEM hRoot = m_TreeSectors.InsertItem(sName, 0,0);

		for(int j=0; j<array.GetSize(); j++)
		{
			CCompany* pCompany = array[j];
			CString sComp; 
			sComp.Format("%s  |  %s", pCompany->GetTicker(), pCompany->GetName());
			HTREEITEM hItem = m_TreeSectors.InsertItem(sComp, 1,1, hRoot);
			m_TreeSectors.SetItemData(hItem, (DWORD)pCompany);
		}
	}

	// Top 25 percentile
	m_TreeRanks.DeleteAllItems();
	int nAll = theApp.m_Universe.GetSize();
	HTREEITEM hRoot = m_TreeRanks.InsertItem("Companies in Top 25% by Ranking", 0,0);
	for(int i=0; i<nAll/4; i++)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompanyByRankEPV(i);
		CString sComp; 
		sComp.Format("%u - %s  |  %s", i+1, pCompany->GetTicker(), pCompany->GetName());
		HTREEITEM hItem = m_TreeSectors.InsertItem(sComp, 1,1, hRoot);
		m_TreeSectors.SetItemData(hItem, (DWORD)pCompany);
	}
	hRoot = m_TreeRanks.InsertItem("Companies in 25% - 50% by Ranking", 0,0);
	for(int i=nAll/4; i<nAll/2; i++)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompanyByRankEPV(i);
		CString sComp; 
		sComp.Format("%u - %s  |  %s", i+1, pCompany->GetTicker(), pCompany->GetName());
		HTREEITEM hItem = m_TreeSectors.InsertItem(sComp, 1,1, hRoot);
		m_TreeSectors.SetItemData(hItem, (DWORD)pCompany);
	}
	hRoot = m_TreeRanks.InsertItem("Companies in 50% - 75% by Ranking", 0,0);
	for(int i=nAll/2; i<3*nAll/4; i++)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompanyByRankEPV(i);
		CString sComp; 
		sComp.Format("%u - %s  |  %s", i+1, pCompany->GetTicker(), pCompany->GetName());
		HTREEITEM hItem = m_TreeSectors.InsertItem(sComp, 1,1, hRoot);
		m_TreeSectors.SetItemData(hItem, (DWORD)pCompany);
	}
	hRoot = m_TreeRanks.InsertItem("Companies in Bottom 25% by Ranking", 0,0);
	for(int i=3*nAll/4; i<nAll; i++)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompanyByRankEPV(i);
		CString sComp; 
		sComp.Format("%u - %s  |  %s", i+1, pCompany->GetTicker(), pCompany->GetName());
		HTREEITEM hItem = m_TreeSectors.InsertItem(sComp, 1,1, hRoot);
		m_TreeSectors.SetItemData(hItem, (DWORD)pCompany);
	}

	// Indexes
	m_TreeIndexes.DeleteAllItems();
	for(int i=0; i<NUM_WEIGHTS; i++)
	{
		CString sName = WeightsIndices[i]; 
		CArray<CCompany*,CCompany*> array;
		theApp.m_Universe.GetCompaniesInIndex(sName, array);

		sName.Format("%s (%u)", WeightsIndices[i], array.GetSize());
		HTREEITEM hRoot = m_TreeIndexes.InsertItem(sName, 0,0);

		for(int j=0; j<array.GetSize(); j++)
		{
			CCompany* pCompany = array[j];
			CString sComp; 
			sComp.Format("%s - %s [%.2f%%]", pCompany->GetTicker(), pCompany->GetName(), pCompany->GetWeightInIndex(i)*100.0);
			HTREEITEM hItem = m_TreeIndexes.InsertItem(sComp, 1,1, hRoot);
			m_TreeIndexes.SetItemData(hItem, (DWORD)pCompany);
		}
	}

	// Types
	m_TreeTypes.DeleteAllItems();
	static char *szTypes[] = {"Regular Company", "Bank", "Insurance", "Utility", "Homebuilder", "Diversified Financial", "REIT" };

	for(int i=0; i<7; i++)
	{
		CString sName = WeightsIndices[i]; 
		CArray<CCompany*,CCompany*> array;
		theApp.m_Universe.GetCompaniesInType(i, array);

		sName.Format("%s (%u)", szTypes[i], array.GetSize());
		HTREEITEM hRoot = m_TreeTypes.InsertItem(sName, 0,0);

		for(int j=0; j<array.GetSize(); j++)
		{
			CCompany* pCompany = array[j];
			CString sComp; 
			sComp.Format("%s - %s", pCompany->GetTicker(), pCompany->GetName());
			HTREEITEM hItem = m_TreeTypes.InsertItem(sComp, 1,1, hRoot);
			m_TreeTypes.SetItemData(hItem, (DWORD)pCompany);
		}
	}
}

void CDialogStocksAlphabetical::OnDblclkListStocks() 
{
	int nSel = m_ListTicker.GetCurSel();
	CCompany* pCompany = (CCompany*)m_ListTicker.GetItemData(nSel);

	theApp.NewViewGeneral(pCompany);
}

void CDialogStocksAlphabetical::OnChangeEditTicker() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TICKER);
	if(!pEdit || !pEdit->m_hWnd)
		return;

	CString sTicker;
	pEdit->GetWindowText(sTicker);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker, false);
	
	if(nIndex == -1)
		return;

	m_ListTicker.SetCurSel(nIndex);
}

void CDialogStocksAlphabetical::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDialogStocksAlphabetical::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocus = GetFocus();
		if(pFocus && ::IsWindow(pFocus->m_hWnd) && pFocus->GetDlgCtrlID() == IDC_EDIT_TICKER)
		{
			int nSel = m_ListTicker.GetCurSel();
			if(nSel == -1)
			{
				AfxMessageBox("Please select a stock.", MB_OK);
				return true;
			}

			CCompany* pCompany = (CCompany*)m_ListTicker.GetItemData(nSel);

			theApp.NewViewGeneral(pCompany);
		}
		return false;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogStocksAlphabetical::OnDestroy() 
{
	CDialog::OnDestroy();
	
	CImageList* pImageList = m_Tab.GetImageList();
	if(pImageList)
		delete pImageList;	

	pImageList = m_TreeIndustries.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;

	pImageList = m_TreeSectors.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;

	pImageList = m_TreeRanks.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;

	pImageList = m_TreeIndexes.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;

	pImageList = m_TreeTypes.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;
}

void CDialogStocksAlphabetical::OnSelchangeTabLists(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = m_Tab.GetCurSel();

	m_ListTicker.ShowWindow(SW_HIDE);
	m_TreeIndustries.ShowWindow(SW_HIDE);
	m_TreeSectors.ShowWindow(SW_HIDE);
	m_TreeRanks.ShowWindow(SW_HIDE);
	m_TreeIndexes.ShowWindow(SW_HIDE);
	m_TreeTypes.ShowWindow(SW_HIDE);

	switch(nSel)
	{
	case 0: m_ListTicker.ShowWindow(SW_SHOW); break;
	case 1: m_TreeIndustries.ShowWindow(SW_SHOW); break;
	case 2: m_TreeSectors.ShowWindow(SW_SHOW); break;
	case 3: m_TreeRanks.ShowWindow(SW_SHOW); break;
	case 4: m_TreeIndexes.ShowWindow(SW_SHOW); break;
	case 5: m_TreeTypes.ShowWindow(SW_SHOW); break;
	}
	
	*pResult = 0;
}

void CDialogStocksAlphabetical::OnDblclkTreeIndustries(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_TreeIndustries.GetSelectedItem();
	if(!hItem)
		return;

	CCompany* pCompany = (CCompany*)m_TreeIndustries.GetItemData(hItem);
	if(!pCompany)
		return;

	theApp.NewViewGeneral(pCompany);
	*pResult = 0;
}

void CDialogStocksAlphabetical::OnDblclkTreeSectors(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_TreeSectors.GetSelectedItem();
	if(!hItem)
		return;

	CCompany* pCompany = (CCompany*)m_TreeSectors.GetItemData(hItem);
	if(!pCompany)
		return;

	theApp.NewViewGeneral(pCompany);
	*pResult = 0;
}

void CDialogStocksAlphabetical::OnDblclkTreeRanks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_TreeRanks.GetSelectedItem();
	if(!hItem)
		return;

	CCompany* pCompany = (CCompany*)m_TreeRanks.GetItemData(hItem);
	if(!pCompany)
		return;

	theApp.NewViewGeneral(pCompany);
	*pResult = 0;
}

void CDialogStocksAlphabetical::OnButtonOpen() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TICKER);
	CString sTicker;
	pEdit->GetWindowText(sTicker);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker, false);
	
	if(nIndex == -1)
		return;

	m_ListTicker.SetCurSel(nIndex);
	
	CCompany* pCompany = (CCompany*)m_ListTicker.GetItemData(nIndex);
	theApp.NewViewGeneral(pCompany);
}

void CDialogStocksAlphabetical::OnDblclkTreeIndexes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_TreeIndexes.GetSelectedItem();
	if(!hItem)
		return;

	CCompany* pCompany = (CCompany*)m_TreeIndexes.GetItemData(hItem);
	if(!pCompany)
		return;

	theApp.NewViewGeneral(pCompany);
	*pResult = 0;
}

void CDialogStocksAlphabetical::OnDblclkTreeTypes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hItem = m_TreeTypes.GetSelectedItem();
	if(!hItem)
		return;

	CCompany* pCompany = (CCompany*)m_TreeTypes.GetItemData(hItem);
	if(!pCompany)
		return;

	theApp.NewViewGeneral(pCompany);
	*pResult = 0;
}
