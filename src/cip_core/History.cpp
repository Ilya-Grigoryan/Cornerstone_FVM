// History.cpp: implementation of the CHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "History.h"

#include "Company.h"
#include "SQLCompany.h"

#include "DataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistory::CHistory()
{
	m_bLoaded = false;
	m_bAllLoaded = false;

	for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+3; i++)
		arLoaded[i] = 0;

	m_pCompany = NULL;
	
	m_From.SetStatus(COleDateTime::null);
	m_To.SetStatus(COleDateTime::null);
}

CHistory::CHistory(CCompany* pCompany)
{
	m_bLoaded = false;
	m_pCompany = pCompany;

	for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+3; i++)
		arLoaded[i] = 0;

	m_From.SetStatus(COleDateTime::null);
	m_To.SetStatus(COleDateTime::null);
}

CHistory::~CHistory()
{

}

BOOL CHistory::Load(const char* szTicker, int nNumRecords)
{
	Clear();

	strcpy(sTicker, szTicker);
	if(!CSQLCompany::GetHistory(szTicker, *this, nNumRecords))
		return false;

	m_bLoaded = true;
	if(nNumRecords == -1)
		m_bAllLoaded = true;

	for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+3; i++)
		arLoaded[i] = GetSize();

	CalcMAvgValues(); 
	return true; 
}

BOOL CHistory::Load(const char* szTicker, COleDateTime from, COleDateTime to)
{
	Clear();

	strcpy(sTicker, szTicker);
	m_From = from;
	m_To = to;

	if(!CSQLCompany::GetHistory(szTicker, *this, from, to))
		return false;

	m_bLoaded = true;
	if(from.GetStatus() != COleDateTime::valid && to.GetStatus() != COleDateTime::valid)
		m_bAllLoaded = true;
	
	for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+3; i++)
		arLoaded[i] = GetSize();

	CalcMAvgValues();
	return true;
}

BOOL CHistory::LoadItem(const char* szTicker, const char* szItem, int nNumRecords)
{
	int nItem = CDataManager::GetColIndexFromName(szItem);
	if(nItem == -1)
		return false;

	BOOL bVirtual = false;
	int nFillItem = nItem;

	if(nFillItem >= NUM_COLUMNS_IN_HISTORY_DB)
	{
		nFillItem = 1;
		bVirtual = true;
	}

	if(!CSQLCompany::FillHistoryItem(szTicker, nFillItem, *this, nNumRecords))
		return false;

	arLoaded[nItem] = (nNumRecords == -1) ? GetSize() : nNumRecords;
	
	if(bVirtual)
		CalcMAvgValues();
	return true;
}

BOOL CHistory::PreLoad(const char* szTicker)
{
	Clear();

	strcpy(sTicker, szTicker);
	
	if(!CSQLCompany::GetHistoryDates(szTicker, *this))
		return false;

	return true;
}

void CHistory::Clear()
{
	arItems.RemoveAll();
	m_From.SetStatus(COleDateTime::null);
	m_To.SetStatus(COleDateTime::null);

	m_bLoaded = m_bAllLoaded = false;
}

void CHistory::GetStartEndIndexesForPeriod(COleDateTime from, COleDateTime to, int& nStart, int &nEnd)
{
	if(arItems.GetSize() == 0)
	{
		nStart = nEnd = -1;
		return;
	}
	
	// start
	if(from.GetStatus() != COleDateTime::valid || from <= arItems[arItems.GetUpperBound()].date)
		nStart = arItems.GetUpperBound();
	else
		nStart = GetItemIndex(from, false);
	
	// end
	if(to.GetStatus() != COleDateTime::valid || to >= arItems[0].date)
		nEnd = 0;
	else
		nEnd = GetItemIndex(to, false);
}

int CHistory::GetItemIndex(COleDateTime date, BOOL bExact)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = arItems.GetUpperBound();
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		COleDateTime Temp = arItems[nMiddle].date;

		/* check whether the element already exists*/ 
		if(date == Temp)
			return nMiddle;

		/* check for greater or less*/
		if(Temp < date)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	if(bExact)
		return -1;

	return nFirst;
}

SHistoryItem* CHistory::GetItemPtr(COleDateTime date)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = arItems.GetUpperBound();
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		COleDateTime Temp = arItems[nMiddle].date;

		/* check whether the element already exists*/ 
		if(date == Temp)
			return &arItems[nMiddle];

		/* check for greater or less*/
		if(Temp < date)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	return NULL;
}

int	CHistory::GetIndexByDate(COleDateTime date)
{
	int nIndex = -1;
	for(int i=0; i<GetSize(); i++)
	{
		if(arItems[i].date == date)
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}

void CHistory::CalcMAvgValues()
{
	if(!m_pCompany)
		return;

	int nItems = arItems.GetSize();
	if(nItems == 0)
		return;

	int nPeriods = m_pCompany->GetMacroParametersPointer()->GetPeriodsMA();

	double *data = new double[nItems];
	for(int i=0; i<nItems; i++)
		data[i] = arItems[i].data[1];

	int first = 0;
	int last = min(first + nPeriods, nItems);
	while(first < nItems)
	{
		double fMA = CMath::GetMean(&data[first], last-first); 
		
		arItems[first].data[NUM_COLUMNS_IN_HISTORY_DB] = fMA;
		arItems[first].data[NUM_COLUMNS_IN_HISTORY_DB+1] = arItems[first].data[1] / fMA;

		if(first > 0)
			arItems[first-1].data[NUM_COLUMNS_IN_HISTORY_DB+2] = arItems[first-1].data[NUM_COLUMNS_IN_HISTORY_DB] / arItems[first].data[NUM_COLUMNS_IN_HISTORY_DB];

		++first;
		last = min(first + nPeriods, nItems);
	}
	arItems[nItems-1].data[NUM_COLUMNS_IN_HISTORY_DB+2] = _INF_; 

	// Forecast
	last = min(nPeriods-1, nItems);
	m_fPriceFore = CMath::GetMean(data, last); 
	m_fMAFore = m_fPriceFore;
	m_fPMFore = 1.0;
	m_fMMFore = m_fMAFore / arItems[0].data[NUM_COLUMNS_IN_HISTORY_DB];
	m_fRetFore = 100.0 * (m_fPriceFore / arItems[0].data[1] - 1);

	delete [] data;
}
