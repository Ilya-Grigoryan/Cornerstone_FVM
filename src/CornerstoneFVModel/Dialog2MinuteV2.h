#if !defined(AFX_DIALOG2MINUTEV2_H__28BE8860_F75A_4E73_9F35_72D16AB41ACA__INCLUDED_)
#define AFX_DIALOG2MINUTEV2_H__28BE8860_F75A_4E73_9F35_72D16AB41ACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog2MinuteV2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog2MinuteV2 dialog

class CDialog2MinuteV2 : public CDialog
{
// Construction
public:
	CDialog2MinuteV2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog2MinuteV2)
	enum { IDD = IDD_DIALOG_2MINUTE_V2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog2MinuteV2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog2MinuteV2)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG2MINUTEV2_H__28BE8860_F75A_4E73_9F35_72D16AB41ACA__INCLUDED_)
