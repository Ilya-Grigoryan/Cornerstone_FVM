// ListFiles.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ListFiles.h"

#include "company.h"
#include "universe.h"

#include "sqlmeetings.h"
#include "sqlcompany.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CListFiles

CListFiles::CListFiles()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_DOC);
	BITMAP bmpInfo;
    bmp.GetBitmap(&bmpInfo);

	m_dcIcon1.CreateCompatibleDC(NULL);
	m_dcIcon1.SelectObject(&bmp);

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName,"Arial");
	lf.lfHeight = (long)15;
	lf.lfWeight = 600;
	m_FontTicker.CreateFontIndirect(&lf);

	lf.lfWeight = 500;
	m_FontName.CreateFontIndirect(&lf);
}

CListFiles::~CListFiles()
{
}


BEGIN_MESSAGE_MAP(CListFiles, CListBox)
	//{{AFX_MSG_MAP(CListFiles)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListFiles::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	SFile* pS = (SFile*)lpDrawItemStruct->itemData; 
	if(!pS)
		return;
	CString sName = pS->sName;
	
	pDC->SetBkMode(TRANSPARENT);
	
	CRect rcItem(lpDrawItemStruct->rcItem); CRect rcOriginal(rcItem);
	BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
	if(bSelected)
	{
		pDC->FillRect(&rcItem, &CBrush(RGB(225,245,255)));
		rcItem.OffsetRect(1,1);
	}
	else
		pDC->FillRect(&rcItem, &CBrush(RGB(255,255,255)));

	// draw icon
	pDC->BitBlt(rcItem.left+3,rcItem.top+1,20,20,&m_dcIcon1, 0,0,SRCCOPY);

	// draw Name
	pDC->SetTextColor(RGB(0,50,0));
	CFont* pOldFont = pDC->SelectObject(&m_FontName);
	CRect rcText(rcItem);
	rcText.left += 23;
	pDC->DrawText(sName, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);


	if(pOldFont)
		pDC->SelectObject(pOldFont);

	if(bSelected)
		pDC->Draw3dRect(&rcOriginal, RGB(200,220,230), RGB(200,220,230));
	else
		pDC->Draw3dRect(&rcOriginal, RGB(255,255,255), RGB(255,255,255) );
}

int CListFiles::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	return 0;
}

void CListFiles::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 23;
}

/////////////////////////////////////////////////////////////////////////////
// CListFiles message handlers
