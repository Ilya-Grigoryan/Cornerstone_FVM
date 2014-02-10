#if !defined(AFX_DIALOGEPSGRAPH_H__E3324EC8_959C_4E5D_9F4E_5212236C5B89__INCLUDED_)
#define AFX_DIALOGEPSGRAPH_H__E3324EC8_959C_4E5D_9F4E_5212236C5B89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogEpsGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsGraph dialog

class CCompany;
class CViewGraph;
class CDialogEpsGraph : public CDialog
{
// Construction
public:
	CDialogEpsGraph(CCompany* pCompany, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogEpsGraph)
	enum { IDD = IDD_DIALOG_EPS_GRAPH };
	CTabCtrl	m_Tab;
	//}}AFX_DATA
	CCompany*		m_pCompany;
	CViewGraph*		m_pGraph;

	void Resize();

	void InitChartEPS();
	void InitChartROC();
	void InitChartROE();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEpsGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogEpsGraph)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEPSGRAPH_H__E3324EC8_959C_4E5D_9F4E_5212236C5B89__INCLUDED_)
