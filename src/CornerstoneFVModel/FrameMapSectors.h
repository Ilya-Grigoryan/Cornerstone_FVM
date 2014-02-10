#if !defined(AFX_FRAMEMAPSECTORS_H__229802EF_F794_4D42_8E47_8FEA5D6EF1FC__INCLUDED_)
#define AFX_FRAMEMAPSECTORS_H__229802EF_F794_4D42_8E47_8FEA5D6EF1FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameMapSectors.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameMapSectors frame

class CFrameMapSectors : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameMapSectors)
protected:
	CFrameMapSectors();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameMapSectors)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameMapSectors();

	// Generated message map functions
	//{{AFX_MSG(CFrameMapSectors)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEMAPSECTORS_H__229802EF_F794_4D42_8E47_8FEA5D6EF1FC__INCLUDED_)
