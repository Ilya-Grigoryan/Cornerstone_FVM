#if !defined(AFX_DIALOGSELECTUNIVERSE_H__ADBB5A38_B057_4A08_8B7B_6E9AE70F1ED4__INCLUDED_)
#define AFX_DIALOGSELECTUNIVERSE_H__ADBB5A38_B057_4A08_8B7B_6E9AE70F1ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSelectUniverse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectUniverse dialog

class CDialogSelectUniverse : public CDialog
{
// Construction
public:
	CDialogSelectUniverse(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSelectUniverse)
	enum { IDD = IDD_DIALOG_SELECT_UNIVERSE };
	int		m_nSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSelectUniverse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSelectUniverse)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSELECTUNIVERSE_H__ADBB5A38_B057_4A08_8B7B_6E9AE70F1ED4__INCLUDED_)
