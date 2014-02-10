#if !defined(AFX_DIALOGCONNECTIONS_H__58363458_F5DA_48AE_8525_C1F1FE4E645D__INCLUDED_)
#define AFX_DIALOGCONNECTIONS_H__58363458_F5DA_48AE_8525_C1F1FE4E645D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogConnections.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogConnections dialog

class CDialogConnections : public CDialog
{
// Construction
public:
	CDialogConnections(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogConnections)
	enum { IDD = IDD_DIALOG_CONNECTION };
	CString	m_sLogin1;
	CString	m_sLogin2;
	CString	m_sLogin3;
	CString	m_sPassw1;
	CString	m_sPassw2;
	CString	m_sPassw3;
	CString	m_sServer1;
	CString	m_sServer2;
	CString	m_sServer3;
	int		m_nDatabase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConnections)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConnections)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCONNECTIONS_H__58363458_F5DA_48AE_8525_C1F1FE4E645D__INCLUDED_)
