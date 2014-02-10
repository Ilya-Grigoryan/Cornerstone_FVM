// DDialogMain.h : header file
//

#if !defined(AFX_DDIALOGMAIN_H__D1730B38_4CE1_409A_9F80_E20E151A48B4__INCLUDED_)
#define AFX_DDIALOGMAIN_H__D1730B38_4CE1_409A_9F80_E20E151A48B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDialogMain dialog

class CDialogMain : public CDialog
{
// Construction
public:
	CDialogMain(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMain)
	enum { IDD = IDD_CIP_BASELINEPROXY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDialogMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DDIALOGMAIN_H__D1730B38_4CE1_409A_9F80_E20E151A48B4__INCLUDED_)
