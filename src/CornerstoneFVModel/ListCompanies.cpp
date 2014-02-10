// ListCompanies.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ListCompanies.h"

#include "company.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCompanies

CListCompanies::CListCompanies()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_STOCK);
	BITMAP bmpInfo;
    bmp.GetBitmap(&bmpInfo);

	m_dcIcon1.CreateCompatibleDC(NULL);
	m_dcIcon1.SelectObject(&bmp);

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName,"MS Sans Serif");
	lf.lfHeight = (long)14;
	lf.lfWeight = 600;
	m_FontTicker.CreateFontIndirect(&lf);

	lf.lfWeight = 500;
	m_FontName.CreateFontIndirect(&lf);
}

CListCompanies::~CListCompanies()
{
}


BEGIN_MESSAGE_MAP(CListCompanies, CListBox)
	//{{AFX_MSG_MAP(CListCompanies)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListCompanies::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CCompany* pCompany = (CCompany*)lpDrawItemStruct->itemData; // Index of Bitmap
	if(pCompany == NULL) 
		return;

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

	// draw Ticker
	//pDC->SetTextColor(RGB(0,90,190));
	CFont* pOldFont = pDC->SelectObject(&m_FontTicker);
	CRect rcText(rcItem);
	rcText.left += 30;
	pDC->DrawText(pCompany->GetTicker(), rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	// draw Name
	//pDC->SetTextColor(RGB(0,128,210));
	pDC->SelectObject(&m_FontName);
	rcText.left += 45;
	CString sName; sName.Format("|   %s", pCompany->GetName());
	pDC->DrawText(sName, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	if(pOldFont)
		pDC->SelectObject(pOldFont);

	if(bSelected)
		pDC->Draw3dRect(&rcOriginal, RGB(200,220,230), RGB(200,220,230));
	else
		pDC->Draw3dRect(&rcOriginal, RGB(255,255,255), RGB(255,255,255) );
}

int CListCompanies::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	return 0;
}

void CListCompanies::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 20;
}

/////////////////////////////////////////////////////////////////////////////
// CListCompanies message handlers
