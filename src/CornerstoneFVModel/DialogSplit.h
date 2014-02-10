#if !defined(AFX_DIALOGSPLIT_H__9F1D7FAE_9647_40B4_974A_15F51E8EE353__INCLUDED_)
#define AFX_DIALOGSPLIT_H__9F1D7FAE_9647_40B4_974A_15F51E8EE353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSplit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSplit dialog

class CDialogSplit : public CDialog
{
// Construction
public:
	CDialogSplit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSplit)
	enum { IDD = IDD_DIALOG_SPLIT };
	int		m_nBefore;
	int		m_nAfter;
	//}}AFX_DATA
	double	m_fRate;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSplit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSplit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSPLIT_H__9F1D7FAE_9647_40B4_974A_15F51E8EE353__INCLUDED_)
