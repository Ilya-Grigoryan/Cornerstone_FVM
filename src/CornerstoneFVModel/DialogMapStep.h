#if !defined(AFX_DIALOGMAPSTEP_H__1A612908_BB65_4262_A73E_6CAEDD22860E__INCLUDED_)
#define AFX_DIALOGMAPSTEP_H__1A612908_BB65_4262_A73E_6CAEDD22860E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMapStep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMapStep dialog

class CDialogMapStep : public CDialog
{
// Construction
public:
	CDialogMapStep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMapStep)
	enum { IDD = IDD_DIALOG_MAP_STEP };
	int		m_nStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMapStep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMapStep)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMAPSTEP_H__1A612908_BB65_4262_A73E_6CAEDD22860E__INCLUDED_)
