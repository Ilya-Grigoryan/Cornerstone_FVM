#if !defined(AFX_LISTMEETINGS_H__F2E15688_F766_4AB2_9403_8E15282E813E__INCLUDED_)
#define AFX_LISTMEETINGS_H__F2E15688_F766_4AB2_9403_8E15282E813E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListMeetings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListMeetings window

class CListMeetings : public CListBox
{
// Construction
public:
	CListMeetings();

// Attributes
public:
	CDC		m_dcIcon1;
	CFont	m_FontTicker;
	CFont	m_FontName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListMeetings)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListMeetings();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListMeetings)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct );
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTMEETINGS_H__F2E15688_F766_4AB2_9403_8E15282E813E__INCLUDED_)
