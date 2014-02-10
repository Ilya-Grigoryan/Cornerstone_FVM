#if !defined(AFX_DIALOGSTOCKSALPHABETICAL_H__1020970D_0507_4FB8_BEC5_7222ED2E110F__INCLUDED_)
#define AFX_DIALOGSTOCKSALPHABETICAL_H__1020970D_0507_4FB8_BEC5_7222ED2E110F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogStocksAlphabetical.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStocksAlphabetical dialog

#include "ListCompanies.h"

class CDialogStocksAlphabetical : public CDialog
{
// Construction
public:
	CDialogStocksAlphabetical(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogStocksAlphabetical)
	enum { IDD = IDD_DIALOG_STOCK_APLHABETICAL };
	CTreeCtrl	m_TreeTypes;
	CTreeCtrl	m_TreeIndexes;
	CTreeCtrl	m_TreeRanks;
	CTreeCtrl	m_TreeSectors;
	CTreeCtrl	m_TreeIndustries;
	CTabCtrl	m_Tab;
	CButton	m_stTickBorder;
	CStatic	m_stCornerstone;
	CListCompanies	m_ListTicker;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogStocksAlphabetical)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() {}
	virtual void OnCancel() {}
	//}}AFX_VIRTUAL

	void Resize();

public:
	void SetupData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogStocksAlphabetical)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListStocks();
	afx_msg void OnChangeEditTicker();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabLists(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeIndustries(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeSectors(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeRanks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOpen();
	afx_msg void OnDblclkTreeIndexes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeTypes(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSTOCKSALPHABETICAL_H__1020970D_0507_4FB8_BEC5_7222ED2E110F__INCLUDED_)
