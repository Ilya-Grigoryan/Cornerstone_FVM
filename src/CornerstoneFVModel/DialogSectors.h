#if !defined(AFX_DIALOGSECTORS_H__6C7D6AB5_76FF_4203_8EFA_881277FE6985__INCLUDED_)
#define AFX_DIALOGSECTORS_H__6C7D6AB5_76FF_4203_8EFA_881277FE6985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSectors.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSectors dialog

class CDialogSectors : public CDialog
{
// Construction
public:
	CDialogSectors(int bIndustryOrSector, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSectors)
	enum { IDD = IDD_DIALOG_SECTORS };
	CListBox	m_List;
	//}}AFX_DATA
	CFont	m_Font;
	int	m_bIndustryOrSector;		// 0 is sector, 1 is industry (remember to switch them, confusion from DB)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSectors)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void Load();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSectors)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSECTORS_H__6C7D6AB5_76FF_4203_8EFA_881277FE6985__INCLUDED_)
