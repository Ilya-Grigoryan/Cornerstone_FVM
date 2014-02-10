#if !defined(AFX_DIALOGEARNINGSMOMENTUM_H__D23A6467_D705_4095_8703_95DA75719DB8__INCLUDED_)
#define AFX_DIALOGEARNINGSMOMENTUM_H__D23A6467_D705_4095_8703_95DA75719DB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogEarningsMomentum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEarningsMomentum dialog

class CDialogEarningsMomentum : public CDialog
{
// Construction
public:
	CDialogEarningsMomentum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogEarningsMomentum)
	enum { IDD = IDD_DIALOG_REP_EARNINGS_MOMENTUM };
	CCheckListBox	m_ListFilter;
	BOOL	m_bNoIndividualStocks;
	double	m_fMCapMax;
	double	m_fMCapMin;
	CString	m_sTitle;
	int		m_nSort;
	BOOL	m_bExcel;
	int		m_nGroup;
	//}}AFX_DATA
	CArray<int,int>	m_arFilter;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEarningsMomentum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogEarningsMomentum)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEARNINGSMOMENTUM_H__D23A6467_D705_4095_8703_95DA75719DB8__INCLUDED_)
