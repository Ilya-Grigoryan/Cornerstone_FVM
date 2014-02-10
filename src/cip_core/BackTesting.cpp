// BackTesting.cpp: implementation of the CBackTesting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "BackTesting.h"
#include "DialogProgressEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBackTestingItem::CBackTestingItem()
{
	m_fPrice = _INF_;
}

CBackTestingItem::CBackTestingItem(const char* szTicker, const COleDateTime& date)
{
	strcpy(m_szTicker, szTicker);
	m_Date = date;

	m_fPrice = _INF_;
	m_fScore = _INF_;
}

CBackTestingItem::~CBackTestingItem()
{

}


//////////////////////////////////////////////////////////////////////
// CBackTesting
CBackTesting::CBackTesting()
{

}

CBackTesting::~CBackTesting()
{
	ClearMappingLists();
	RemoveAllItems();
}

void CBackTesting::CalcItems()
{
	for(int i=0; i<m_arItems.GetSize(); i++)
		m_arItems[i]->Calc();
}

BOOL CBackTesting::CreateEnvironment(const COleDateTime& dtStart, const COleDateTime& dtEnd, const COleDateTimeSpan& AdditionalHist,
									 const CStringArray& arTickers, 
									 const COleDateTimeSpan& PeriodStatement,
									 const COleDateTimeSpan& PeriodRebalance,
									 FnPtItemCreator FnCreator)
{
	if(dtStart >= dtEnd)
		return false;

	m_StartDate = dtStart;
	m_EndDate = dtEnd;
	m_AdditionalHistory = AdditionalHist;

	m_PeriodStatement = PeriodStatement;
	m_PeriodRebalance = PeriodRebalance;

	COleDateTime dt0 = dtStart - AdditionalHist;
	int nTickers = arTickers.GetSize();
	double nTotalDays = (COleDateTimeSpan(dtEnd-dt0)).GetTotalDays();
	double nPeriodDays = PeriodStatement.GetTotalDays();
	int nCount = (int)((nTickers*nTotalDays)/nPeriodDays);

	////////////////////////////////////////////////////
	int nStep = 0;
	m_pDlgProgress = new CDialogProgressEx;
	m_pDlgProgress->Create();

	m_pDlgProgress->SetRange(0, nCount);
	m_pDlgProgress->SetStep(1);
	m_pDlgProgress->SetPos(0);

	for(int i=0; i<nTickers; i++)
	{
		CString sTicker = arTickers[i];
		
		for(COleDateTime d=dt0; d<=dtEnd; d+=PeriodStatement)
		{
			if(nStep % 10 == 0)
				m_pDlgProgress->SetPos(nStep);
			++nStep;

			CBackTestingItem* pItem = (*FnCreator)(this);
			strcpy(pItem->m_szTicker, sTicker);
			pItem->m_Date  = d;
			pItem->Load();
			
			m_arItems.Add(pItem);  // add to the raw array; seperate sorted lists hold special access to data based on dates or tickers
			AddItemToListDates(d, pItem);
			AddItemToListTickers(sTicker, pItem);
		}
	}

	m_pDlgProgress->DestroyWindow();
	delete m_pDlgProgress;

	return true;
}

void CBackTesting::AddItemToListDates(const COleDateTime& date, const CBackTestingItem* pItem)
{
	CListEx<CString>* pList = NULL;
	int nIndex = m_ListByDates.Lookup(date);
	
	if(nIndex == -1)
	{
		pList = new CListEx<CString>;
		m_ListByDates.Add(date, (void*)pList);
	}
	else
		pList = (CListEx<CString>*)m_ListByDates.m_ItemData[nIndex]->m_pData;
	
	pList->Add(pItem->GetTicker(), (void*)pItem);
}

void CBackTesting::AddItemToListTickers(const char* szTicker, const CBackTestingItem* pItem)
{
	CListEx<COleDateTime>* pList = NULL;
	int nIndex = m_ListByTickers.Lookup(szTicker);

	if(nIndex == -1)
	{
		pList = new CListEx<COleDateTime>;
		m_ListByTickers.Add(szTicker, (void*)pList);
	}
	else
		pList = (CListEx<COleDateTime>*)m_ListByTickers.m_ItemData[nIndex]->m_pData;

	pList->Add(pItem->GetDate(), (void*)pItem);
}

void CBackTesting::ClearMappingLists()
{
	// delete the list of lists (RemoveAll delete the list item objects but doesn't touch the objects inside)
	for(int i=0; i<m_ListByDates.m_ItemData.GetSize(); i++)
	{
		CListEx<CString>* pList = (CListEx<CString>*)m_ListByDates.m_ItemData[i]->m_pData;
		pList->RemoveAll();
		delete pList;
	}
	m_ListByDates.RemoveAll();

	for(int i=0; i<m_ListByTickers.m_ItemData.GetSize(); i++) 
	{
		CListEx<COleDateTime>* pList = (CListEx<COleDateTime>*)m_ListByTickers.m_ItemData[i]->m_pData;
		pList->RemoveAll();
		delete pList;
	}
	m_ListByTickers.RemoveAll();
}

void CBackTesting::RemoveAllItems()
{
	for(int i=0; i<m_arItems.GetSize(); i++)
		delete m_arItems[i];
	m_arItems.RemoveAll();
}

CBackTestingItem* CBackTesting::GetItem(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_arItems.GetSize())
		return NULL;
	return m_arItems[nIndex];
}

CBackTestingItem* CBackTesting::GetItem(const char* szTicker, const COleDateTime& date)
{
	// first look up by tickers
	int nIndex = m_ListByTickers.Lookup(szTicker);
	if(nIndex == -1)  // this is an unknown ticker, no data for this ticker at all
		return NULL;

	CListEx<COleDateTime>* pListForTicker = (CListEx<COleDateTime>*)m_ListByTickers.m_ItemData[nIndex]->m_pData;
	
	// then lookup by date
	nIndex = pListForTicker->Lookup(date);
	if(nIndex == -1)  // no data for the ticker for the specific date
		return NULL;

	CBackTestingItem* pItem = (CBackTestingItem*)pListForTicker->m_ItemData[nIndex]->m_pData;
	return pItem;
}

//////////////////////////////////////////////////////////////////////
// CIRRItem
#include "enginemsexcel.h"
CEngineMSExcel *g_pExcel = NULL;

CIRRItem::CIRRItem() : CBackTestingItem()
{
	if(!g_pExcel)
	{
		g_pExcel = new CEngineMSExcel;
		g_pExcel->InitBaseline();
	}

}

CIRRItem::CIRRItem(const char* szTicker, const COleDateTime& date) : CBackTestingItem(szTicker, date)
{
	
}

BOOL CIRRItem::Load()
{
	COleVariant var;
	var = g_pExcel->GetBaselineData(m_szTicker, "Daily Closing Price", m_Date.Format("%m/%d/%Y"));
	m_fPrice = (var.vt == VT_R8) ? var.dblVal : _INF_;

	var = g_pExcel->GetBaselineData(m_szTicker, "Annual Free Cash Flow Per Share", m_Date.Format("%m/%d/%Y"));
	m_fFCF = (var.vt == VT_R8) ? var.dblVal : _INF_;


	m_fRisk = 1;
	m_fIRR = 10;
	return true; 
}

void CIRRItem::Calc()
{
	if(m_fPrice == 0 || m_fPrice >= _INF_ || m_fFCF >= _INF_)
		m_fScore = _INF_;
	
	m_fScore = m_fFCF / m_fPrice;
}

double CIRRItem::Get(const char* szWhat)
{
	if(stricmp(szWhat, "Risk")==0)
		return m_fRisk;

	if(stricmp(szWhat, "IRR")==0)
		return m_fIRR;
	
	return _INF_;
}

double	CIRRItem::Get(int nWhat)
{
	switch(nWhat)
	{
	case 0: return m_fRisk;
	case 1: return m_fIRR;
	}

	return _INF_;	
}

CBackTestingItem* CIRRItem::Creator(CBackTesting* pBackTesting)
{
	CIRRItem* pItem = new CIRRItem();
	pItem->m_pBackTesting = pBackTesting;
	return pItem;
}