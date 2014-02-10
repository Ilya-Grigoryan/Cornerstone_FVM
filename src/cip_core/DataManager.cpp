// DataManager.cpp: implementation of the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "DataManager.h"

#include "BaseMath.h"
#include "SQLCompany.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
S_DATA_NAME_INFO _data_names[] = 
{
	{ "Date",	"",	-100	},
	{ "Return",	"Ret",	0	},
	{ "Price",	"Price",	1	},
	{ "MA",	"",	55	},
	{ "P/M",	"",	56	},
	{ "M/M",	"",	57	},
	{ "NEPVRnk",	"RankNEPV",	2	},
	{ "REPVRnk",	"RankREPV",	3	},
	{ "NRAVRnk",	"RankNRAV",	4	},
	{ "BEVRnk",	"RankBEV",	5	},
	{ "PMVRnk",	"RankPMV",	6	},
	{ "NEPV",	"NEPV",	7	},
	{ "CYFN",	"CYFN",	8	},
	{ "N/F",	"MomentN",	9	},
	{ "N/FRnk",	"RankMomentN",	10	},
	{ "REPV",	"REPV",	11	},
	{ "CYFR",	"CYFR",	12	},
	{ "R/F",	"MomentR",	13	},
	{ "R/FRnk",	"RankMomentR",	14	},
	{ "Mo1",	"Mo1",	15	},
	{ "Mo1Rnk",	"RankMo1",	16	},
	{ "Mo2",	"Mo2",	17	},
	{ "Mo2Rnk",	"RankMo2",	18	},
	{ "Mltpl",	"Multiple",	19	},
	{ "ROI",	"ROI",	20	},
	{ "Growth",	"Growth",	21	},
	{ "RiskAdj",	"Risk",	22	},
	{ "Yield",	"Yield",	23	},
	{ "P/E",	"PE",	24	},
	{ "EV/EBIDTA",	"EVEBITDA",	25	},
	{ "Payout",	"Payout",	26	},
	{ "NEPS",	"NormEPS",	27	},
	{ "BEPS",	"BookEPS",	28	},
	{ "BookV",	"BookVal",	29	},
	{ "DebtRto",	"DebtRatio",	30	},
	{ "MROC",	"MeanROC",	31	},
	{ "MROE",	"MeanROE",	32	},
	{ "MEPS",	"MeanEPS",	33	},
	{ "Asst/Shr",	"AssetsShare",	34	},
	{ "NCF/Shr",	"NetCFShare",	35	},
	{ "DA/Shr",	"DAShare",	36	},
	{ "FixCov",	"OpFixCover",	37	},
	{ "IntCov",	"IntCover",	38	},
	{ "Div",	"Dividend",	39	},
	{ "EPSNxt",	"EPSNext",	40	},
	{ "EPSThis",	"EPSThis",	41	},
	{ "EPSLast",	"EPSLast",	42	},
	{ "AsstLife",	"AssetLife",	43	},
	{ "GrowthH",	"HistGrowth",	44	},
	{ "GrowthI",	"IndGrowth",	45	},

	{ "wC30",	"c30",	46	},
	{ "wR1000V","r1000v",	47	},
	{ "wLCV",	"lcv",	48	},
	{ "wSP500",	"sp500",	49	},
	{ "wBEV",	"bev",	50	},
	{ "wC20",	"c20",	51	},
	{ "wRMID",	"rmid",	52	},
	{ "wR1000",	"r1000",	53	},
	{ "wR1000G","r1000g",	54	},
	
	{ "_END_", 	"",	-1	}

};

//////////////////////////////////////////////////////////////////////
CData::CData()
{
	m_pDates = NULL;
	m_pValues = NULL;
	m_nSize = 0;
}

CData::CData(int nSize)
{
	m_pDates = new double[nSize];
	m_pValues = new double[nSize];
	m_nSize = nSize;
}

CData::~CData()
{
	Clear();
}

void CData::Clear()
{
	if(m_pDates)
		delete [] m_pDates;
	if(m_pValues)
		delete [] m_pValues;
	m_pDates = NULL;
	m_pValues = NULL;
	m_nSize = 0;
}

void CData::SetSize(int nSize)
{
	Clear();
	
	m_pDates = new double[nSize];
	m_pValues = new double[nSize];
	m_nSize = nSize;
}

void CData::operator =(const CData& src)
{
	Clear();
	SetSize(src.m_nSize);

	strcpy(m_szToken, src.m_szToken);

	memcpy(m_pDates, src.m_pDates, m_nSize*sizeof(double));
	memcpy(m_pValues, src.m_pValues, m_nSize*sizeof(double));
}

double CData::operator [](double date)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = GetSize()-1;
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		double Temp = m_pDates[nMiddle];

		/* check whether the element already exists*/ 
		if(date == Temp)
			return m_pValues[nMiddle];

		/* check for greater or less*/
		if(date < Temp)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	return _INF_;
}

int CData::GetIndexFromDate(double date)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = GetSize()-1;
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		double Temp = m_pDates[nMiddle];

		/* check whether the element already exists*/ 
		if(date == Temp)
			return nMiddle;

		/* check for greater or less*/
		if(date < Temp)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////
CDataManager::CDataManager(CUniverse* pUniverse)
{
	m_pUniverse = pUniverse; 
}

CDataManager::~CDataManager()
{

}

const char* CDataManager::GetColName(int i)
{
	return _data_names[i].name;
}

int CDataManager::GetColIndex(int i)
{
	return _data_names[i].col;
}

int CDataManager::GetColIndexFromName(const char* name)
{
	int i=0;
	while(_data_names[i].col != -1)
	{
		if(stricmp(_data_names[i].name, name) == 0)
			return _data_names[i].col;

		++i;
	}

	return -1;
}

CString CDataManager::GetColDBNameFromName(const char* name)
{
	int i=0;
	while(_data_names[i].col != -1)
	{
		if(stricmp(_data_names[i].name, name) == 0)
			return _data_names[i].dbname;

		++i;
	}

	return "";
}

int CDataManager::GetDataForToken(const char* szToken, CData& data, COleDateTime from, COleDateTime to)
{
	// parse the token
	char szCompany[16]; 
	char szFlag[8]; 
	char szItem[16]; 

	szCompany[0] = '\0';
	szFlag[0] = '\0';
	szItem[0] = '\0';

	char* szTemp = (char*)szToken;
	char* sz = strchr(szTemp, '.');
	if(sz != NULL)
	{
		strncpy(szCompany, szTemp, (size_t)(sz-szTemp));
		szCompany[(int)(sz-szTemp)] = '\0';
		szTemp = sz+1;

		sz = strchr(szTemp, '_');
		if(sz != NULL)
		{
			strncpy(szFlag, szTemp, (size_t)(sz-szTemp));
			szFlag[(int)(sz-szTemp)] = '\0';
			szTemp = sz+1;
		}
		else
		{
			strcpy(szFlag, szTemp);
			szTemp += strlen(szTemp)+1;
		}
	}
	else
	{
		sz = strchr(szTemp, '_');
		if(sz != NULL)
		{
			strncpy(szCompany, szTemp, (size_t)(sz-szTemp));
			szCompany[(int)(sz-szTemp)] = '\0';
			szTemp = sz+1;
		}
		else
		{
			strcpy(szCompany, szTemp);
			szTemp += strlen(szTemp)+1;
		}
	}

	if(szTemp < szToken+strlen(szToken)) 
		strcpy(szItem, szTemp);
	else
		strcpy(szItem, "Price");

	if(strlen(szItem)==0)
		strcpy(szItem, "Price");

	// Averaging option
	// 1 - Simple Average
	// 2 - Weighted Ave by Price
	// 3 - Weighted Ave by Market Cap
	int nComb = 1;
	if(strlen(szFlag) > 0)
	{
		if(szFlag[strlen(szFlag)-1] == '1')
		{
			nComb = 1;
			szFlag[strlen(szFlag)-1] = '\0';
		}
		else if(szFlag[strlen(szFlag)-1] == '2')
		{
			nComb = 2;
			szFlag[strlen(szFlag)-1] = '\0';
		}
		else if(szFlag[strlen(szFlag)-1] == '3')
		{
			nComb = 3;
			szFlag[strlen(szFlag)-1] = '\0';
		}
	}
	strupr(szCompany);

	// Check for special tokens (Indices names)
	for(int i=0; i<NUM_WEIGHTS; i++)
		if(stricmp(szCompany, WeightsIndices[i])==0)
			return GetDataForIndex(szCompany, szItem, data, from, to);

	// T10 (Interest Rate)
	if(stricmp(szCompany, "T10")==0)
		return GetDataForT10(szItem, data, from, to);

	// special case Deciles
	int nQuant = -1;
	for(int i=1; i<=10; i++)
	{
		char szQ[8]; sprintf(szQ, "D%u", i);
		if(stricmp(szCompany, szQ)==0)
		{
			nQuant = i;
			break;
		}
	}
	if(nQuant != -1)
	{
		if(stricmp(szFlag, "")==0 || stricmp(szFlag, "D")==0 || stricmp(szFlag, "W")==0)
			return GetDataForDecileDynamic(nQuant, szItem, data, from, to, nComb);
		else if(stricmp(szFlag, "S")==0 || stricmp(szFlag, "C")==0)
			return GetDataForDecileStatic(nQuant, szItem, data, from, to, nComb);
		else
		{
			CString sMessage; sMessage.Format("Invalid Flag: '%s'", szFlag);
			AfxMessageBox(sMessage, MB_ICONINFORMATION);
			return 0;
		}
	}

	// special case Quantiles
	nQuant = -1;
	for(int i=1; i<=5; i++)
	{
		char szQ[8]; sprintf(szQ, "Q%u", i);
		if(stricmp(szCompany, szQ)==0)
		{
			nQuant = i;
			break;
		}
	}
	if(nQuant != -1)
	{
		if(stricmp(szFlag, "")==0 || stricmp(szFlag, "D")==0 || stricmp(szFlag, "W")==0)
			return GetDataForQuantileDynamic(nQuant, szItem, data, from, to, nComb);
		else if(stricmp(szFlag, "S")==0 || stricmp(szFlag, "C")==0)
			return GetDataForQuantileStatic(nQuant, szItem, data, from, to, nComb);
		else
		{
			CString sMessage; sMessage.Format("Invalid Flag: '%s'", szFlag);
			AfxMessageBox(sMessage, MB_ICONINFORMATION);
			return 0;
		}
	}
			
	// No flag - just a ticker data
	if(strlen(szFlag) == 0)
		return GetDataForTicker(szCompany, szItem, data, from, to);

	// Industry Weighted Index
	if(stricmp(szFlag, "I")==0)
	{	
		switch(nComb)
		{
		case 1: return GetDataForIndustryAveMethod1(szCompany, szItem, data, from, to);
		case 2:	return GetDataForIndustryAveMethod2(szCompany, szItem, data, from, to);
		case 3:	return GetDataForIndustryAveMethod3(szCompany, szItem, data, from, to);
		default: return 0;
		}
	}

	// Sector Weighted Index
	if(stricmp(szFlag, "S")==0)
	{	
		switch(nComb)
		{
		case 1:	return GetDataForSectorAveMethod1(szCompany, szItem, data, from, to);
		case 2:	return GetDataForSectorAveMethod2(szCompany, szItem, data, from, to);
		case 3:	return GetDataForSectorAveMethod3(szCompany, szItem, data, from, to);
		default: return 0;
		}
	}

	CString sMessage; sMessage.Format("Invalid Tocken: '%s'", szToken);
	AfxMessageBox(sMessage, MB_ICONINFORMATION);
	return 0;
}

void CDataManager::FreeDataCollection(CDataCollection& dataCollection)
{
	for(int i=0; i<dataCollection.GetSize(); i++)
		delete dataCollection[i];

	dataCollection.RemoveAll();
}

int CDataManager::GetDataForT10(const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	SInterestRateHistory hist;
	CSQLCompany::GetInterestRateHistory(from, to, hist);

	int nItem = -1;
	if(stricmp(szItem, "Price")==0 || stricmp(szItem, "DiscountRate")==0)
		nItem = 0;
	else if(stricmp(szItem, "WeeklyAve")==0)
		nItem = 1;
	else if(stricmp(szItem, "3MAve")==0)
		nItem = 2;
	else if(stricmp(szItem, "FridayClose")==0)
		nItem = 3;

	if(nItem == -1)
	{
		CString sMessage; sMessage.Format("Invalid Item requested: '%s'. You must request one of the following:\n\nT10_DiscountRate, T10_WeeklyAve, or T10_FridayClose.", szItem);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return 0;
	}
	
	int nSize = hist.GetSize();
	data.SetSize(nSize);
	for(int i=0; i<nSize; i++)
	{
		data.m_pDates[nSize-i-1] = hist[i].date;

		switch(nItem)
		{
			case 0:
			{
				double f1 = hist[i].fWeeklyAve;
				double f2 = hist[i].f3MAve;
				double f3 = hist[i].fFridayClose;
				double f = f1;
				if(f2 < _INF_ && f2 > f)
					f = f2;
				if(f3 < _INF_ && f3 > f)
					f = f3;
				data.m_pValues[nSize-i-1] = f;
				break;
			}
			case 1:	data.m_pValues[nSize-i-1] = hist[i].fWeeklyAve; break;
			case 2:	data.m_pValues[nSize-i-1] = hist[i].f3MAve; break;
			case 3:	data.m_pValues[nSize-i-1] = hist[i].fFridayClose; break;
		}
	} 

	return nSize;
}


int CDataManager::GetDataForTicker(const char* szTicker, const char* szItm, CData& data, COleDateTime from, COleDateTime to)
{
	// special case for cumulative Returns
	char szItem[32];
	strcpy(szItem, szItm);
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		strcpy(szItem, "Price");
		bReturns = true;
	}

	int nIndex = m_pUniverse->GetIndexByTicker(szTicker);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szTicker);
		AfxMessageBox(sMessage);
		return 0;
	}

	// Get data index from item name
	int nItem = GetColIndexFromName(szItem);
	if(nItem == -1)
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", szItem);
		AfxMessageBox(sMessage);
		return 0;
	}

	// Load the history item if needed
	CCompany* pCompany = m_pUniverse->GetCompany(nIndex);
	CHistory* pHist = pCompany->GetHistory();

	if(!pHist->IsItemAllLoaded(nItem))
		pHist->LoadItem(szTicker, szItem);

	int nStart, nEnd;
	pHist->GetStartEndIndexesForPeriod(from, to, nStart, nEnd);
	if(nStart == -1 || nEnd == -1)
	{
		CString sMessage; sMessage.Format("Invalid Dates requested for Ticker: '%s'", szTicker);
		AfxMessageBox(sMessage);
		return 0;
	}

	double fStart = _INF_;

	int nSize = nStart - nEnd + 1;
	data.SetSize(nSize);
	for(int i=nStart; i>=nEnd; i--)
	{
		if(!bReturns) // Ordinary data
		{
			data.m_pDates[nSize-(i-nEnd)-1] = pHist->GetItemPtr(i)->date;
			data.m_pValues[nSize-(i-nEnd)-1] = pHist->GetItemPtr(i)->data[nItem];
		}
		else
		{
			data.m_pDates[nSize-(i-nEnd)-1] = pHist->GetItemPtr(i)->date;
			double fVal = pHist->GetItemPtr(i)->data[nItem];
			if(fVal < _INF_)
			{
				if(fStart >= _INF_)
					fStart = fVal;
				
				double fR = 100.0*(fVal/fStart - 1);
				data.m_pValues[nSize-(i-nEnd)-1] = fR;
			}
			else
				data.m_pValues[nSize-(i-nEnd)-1] = _INF_;
		}
	} 

	return nSize;	
}

int CDataManager::GetDataForIndex(const char* szIndex, const char* szItm, CData& data, COleDateTime from, COleDateTime to)
{
	int i, j;

	// special case for cumulative Returns
	char szItem[32];
	strcpy(szItem, szItm);
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		strcpy(szItem, "Price");
		bReturns = true;
	}

	// Get data index from item name
	int nItem = GetColIndexFromName(szItem);
	if(nItem == -1)
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", szItem);
		AfxMessageBox(sMessage);
		return 0;
	}

	CArray<CCompany*,CCompany*>	arCompanies;
	int nIndex = m_pUniverse->GetCompaniesInIndex(szIndex, arCompanies);
	int nCompanies = arCompanies.GetSize();
	if(nIndex == -1 || nCompanies == 0)
		return 0;

	double fEqualWgt = (double)(1.0/(double)nCompanies);
	CArray<double,double> arStartPrices;
	
	// load the histories first
	for(j=0; j<nCompanies; j++)
	{
		CCompany* pCompany = arCompanies[j];
		CHistory* pHist = pCompany->GetHistory();
		if(!pHist->IsItemAllLoaded(nItem))
			pHist->LoadItem(pCompany->GetTicker(), szItem);
		
		if(bReturns)
			arStartPrices.Add(_INF_);
	}

	CCompany* pBaseCompany = arCompanies[0];
	CHistory* pBaseHistory = pBaseCompany->GetHistory(); 
	int nStart, nEnd;
	pBaseHistory->GetStartEndIndexesForPeriod(from, to, nStart, nEnd);
	if(nStart == -1 || nEnd == -1)
	{
		CString sMessage; sMessage.Format("Invalid Dates requested for Ticker: '%s'", pBaseCompany->GetTicker());
		AfxMessageBox(sMessage);
		return 0;
	}

	int nSize = nStart - nEnd + 1;
	data.SetSize(nSize);
	
	// walk through the dates of base company
	for(i=nStart; i>=nEnd; i--)
	{
		COleDateTime date = pBaseHistory->GetItemPtr(i)->date;

		double fSum = 0.0;
		for(j=0; j<nCompanies; j++)
		{
			CCompany* pComp = arCompanies[j];
			CHistory* pHist = pComp->GetHistory();

 			SHistoryItem* pHistItem = pHist->GetItemPtr(date);
			if(pHistItem)
			{
				// -100 is the special case for @ALL - all companies in universe, this means equal weighting
				double fWeight = (nIndex != -100) ? pComp->GetWeightInIndex(nIndex) : fEqualWgt;
				double fVal = pHistItem->data[nItem];

				if(fVal < _INF_)
				{
					if(bReturns)
					{
						if(arStartPrices[j] >= _INF_)
							arStartPrices[j] = fVal;
						fVal = 100*(fVal/arStartPrices[j] - 1);
					}
					
					fSum += fVal * fWeight;
				}
					
			}
		}

		data.m_pDates[nSize-(i-nEnd)-1]  = date;
		data.m_pValues[nSize-(i-nEnd)-1] = fSum;
	}

	return nSize;
}

int CDataManager::GetDataForDecileStatic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod)
{
	COleDateTime minDate = CSQLCompany::GetMinDateInHistories();
	if(from.GetStatus()==COleDateTime::valid && from > minDate)
		minDate = from;

	int nPercentStart = 10*(nQuant-1);
	int nPercentEnd = 10*nQuant;

	CArray<COleDateTime,COleDateTime> dates;
	int nDates = CSQLCompany::GetHistoryDates(from, to, dates);

	if(nDates == 0)
	{
		AfxMessageBox("No Information is available for the dates specified.", MB_ICONINFORMATION);
		return 0;
	}

	CArray<double,double> ids;
	int nCompanies = CSQLCompany::GetRankedValuesFromHistory(dates[0], nPercentStart, nPercentEnd, "RankNEPV", "CompanyID", ids);

	CString sColName;
	
	// special case for cumulative Returns
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		sColName = "Ret";
		bReturns = true;
	}
	else
		sColName = GetColDBNameFromName(szItem);
	if(sColName.IsEmpty())
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", sColName);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return 0;
	}

	data.SetSize(nDates);
	for(int i=0; i<nDates; i++)
	{
		COleDateTime date = dates[i];
		CArray<double,double> vals;
		int nVals = CSQLCompany::GetValuesFromHistoryByCompanyIds(date, ids, sColName, vals);

		double fAve = CMath::GetMean(vals);
		if(bReturns && fAve >= _INF_)
			fAve = 0;

		data.m_pDates[i]  = date;
		if(bReturns)
		{
			if(i==0) 
				data.m_pValues[i] = 0; 
			else
			{
				double r0 = data.m_pValues[i-1];
				data.m_pValues[i] = 100.0*( (1+r0/100)*(1+fAve/100)-1 );
			}
		}
		else
			data.m_pValues[i] = fAve;
	}

	return nDates;
}

int CDataManager::GetDataForDecileDynamic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod)
{
	int nPercentStart = 10*(nQuant-1);
	int nPercentEnd = 10*nQuant;

	CArray<COleDateTime,COleDateTime> dates;
	int nDates = CSQLCompany::GetHistoryDates(from, to, dates);

	CString sColName;
	
	// special case for cumulative Returns
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		sColName = "Ret";
		bReturns = true;
	}
	else
		sColName = GetColDBNameFromName(szItem);
	if(sColName.IsEmpty())
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", sColName);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return 0;
	}

	data.SetSize(nDates);
	for(int i=0; i<nDates; i++)
	{
		COleDateTime date = dates[i];
		CArray<double,double> vals;
		int nVals = CSQLCompany::GetRankedValuesFromHistory(date, nPercentStart, nPercentEnd, "RankNEPV", sColName, vals);

		double fAve = CMath::GetMean(vals);
		if(bReturns && fAve >= _INF_)
			fAve = 0;

		data.m_pDates[i]  = date;
		if(bReturns)
		{
			if(i==0) 
				data.m_pValues[i] = 0; 
			else
			{
				double r0 = data.m_pValues[i-1];
				data.m_pValues[i] = 100.0*( (1+r0/100)*(1+fAve/100)-1 );
			}
		}
		else
			data.m_pValues[i] = fAve;
	}

	return nDates;
}

int CDataManager::GetDataForQuantileStatic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod)
{
	COleDateTime minDate = CSQLCompany::GetMinDateInHistories();
	if(from.GetStatus()==COleDateTime::valid && from > minDate)
		minDate = from;

	int nPercentStart = 20*(nQuant-1);
	int nPercentEnd = 20*nQuant;

	CArray<COleDateTime,COleDateTime> dates;
	int nDates = CSQLCompany::GetHistoryDates(from, to, dates);

	if(nDates == 0)
	{
		AfxMessageBox("No Information is available for the dates specified.", MB_ICONINFORMATION);
		return 0;
	}

	CArray<double,double> ids;
	int nCompanies = CSQLCompany::GetRankedValuesFromHistory(dates[0], nPercentStart, nPercentEnd, "RankNEPV", "CompanyID", ids);

	CString sColName;
	
	// special case for cumulative Returns
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		sColName = "Ret";
		bReturns = true;
	}
	else
		sColName = GetColDBNameFromName(szItem);
	if(sColName.IsEmpty())
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", sColName);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return 0;
	}

	data.SetSize(nDates);
	for(int i=0; i<nDates; i++)
	{
		COleDateTime date = dates[i];
		CArray<double,double> vals;
		int nVals = CSQLCompany::GetValuesFromHistoryByCompanyIds(date, ids, sColName, vals);

		double fAve = CMath::GetMean(vals);
		if(bReturns && fAve >= _INF_)
			fAve = 0;

		data.m_pDates[i]  = date;
		if(bReturns)
		{
			if(i==0) 
				data.m_pValues[i] = 0; 
			else
			{
				double r0 = data.m_pValues[i-1];
				data.m_pValues[i] = 100.0*( (1+r0/100)*(1+fAve/100)-1 );
			}
		}
		else
			data.m_pValues[i] = fAve;
	}

	return nDates;
}

int CDataManager::GetDataForQuantileDynamic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod)
{
	int nPercentStart = 20*(nQuant-1);
	int nPercentEnd = 20*nQuant;

	CArray<COleDateTime,COleDateTime> dates;
	int nDates = CSQLCompany::GetHistoryDates(from, to, dates);

	CString sColName;
	
	// special case for cumulative Returns
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		sColName = "Ret";
		bReturns = true;
	}
	else
		sColName = GetColDBNameFromName(szItem);
	if(sColName.IsEmpty())
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", sColName);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return 0;
	}

	data.SetSize(nDates);
	for(int i=0; i<nDates; i++)
	{
		COleDateTime date = dates[i];
		CArray<double,double> vals;
		int nVals = CSQLCompany::GetRankedValuesFromHistory(date, nPercentStart, nPercentEnd, "RankNEPV", sColName, vals);

		double fAve = CMath::GetMean(vals);
		if(bReturns && fAve >= _INF_)
			fAve = 0;

		data.m_pDates[i]  = date;
		if(bReturns)
		{
			if(i==0) 
				data.m_pValues[i] = 0; 
			else
			{
				double r0 = data.m_pValues[i-1];
				data.m_pValues[i] = 100.0*( (1+r0/100)*(1+fAve/100)-1 );
			}
		}
		else
			data.m_pValues[i] = fAve;
	}

	return nDates;
}

int CDataManager::CalcDataSimpleAve(CCompany* pBaseCompany, CArray<CCompany*,CCompany*>& arCompanies, const char* szItm, CData& data, COleDateTime from, COleDateTime to)
{
	int i, j;

	// special case for cumulative Returns
	char szItem[32];
	strcpy(szItem, szItm);
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		strcpy(szItem, "Price");
		bReturns = true;
	}

	// Get data index from item name
	int nItem = GetColIndexFromName(szItem);
	if(nItem == -1)
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", szItem);
		AfxMessageBox(sMessage);
		return 0;
	}

	CHistory* pBaseHistory = pBaseCompany->GetHistory();

	// Base company
	if(!pBaseHistory->IsItemAllLoaded(nItem))
		pBaseHistory->LoadItem(pBaseCompany->GetTicker(), szItem);

	int nStart, nEnd;
	pBaseHistory->GetStartEndIndexesForPeriod(from, to, nStart, nEnd);
	if(nStart == -1 || nEnd == -1)
	{
		CString sMessage; sMessage.Format("Invalid Dates requested for Ticker: '%s'", pBaseCompany->GetTicker());
		AfxMessageBox(sMessage);
		return 0;
	}

	int nSize = nStart - nEnd + 1;
	data.SetSize(nSize);

	CArray<double,double> arStartPrices;

	// load the histories first
	for(j=0; j<arCompanies.GetSize(); j++)
	{
		CCompany* pCompany = arCompanies[j];
		if(pCompany != pBaseCompany)
		{
			CHistory* pHist = pCompany->GetHistory();
			if(!pHist->IsItemAllLoaded(nItem))
				pHist->LoadItem(pCompany->GetTicker(), szItem);
		}

		if(bReturns)
			arStartPrices.Add(_INF_);
	}

	// walk through the dates of base company
	for(i=nStart; i>=nEnd; i--)
	{
		COleDateTime date = pBaseHistory->GetItemPtr(i)->date;

		double fSum = 0.0;
		int nItems = 0;
		for(j=0; j<arCompanies.GetSize(); j++)
		{
			CCompany* pComp = arCompanies[j];
			CHistory* pHist = pComp->GetHistory();

 			SHistoryItem* pHistItem = pHist->GetItemPtr(date);
			if(pHistItem)
			{
				double fVal = pHistItem->data[nItem];
				if(fVal < _INF_)
				{
					if(bReturns)
					{
						if(arStartPrices[j] >= _INF_)
							arStartPrices[j] = fVal;
						fVal = 100*(fVal/arStartPrices[j] - 1);
					}

					fSum += fVal;
					nItems ++;
				}
			}
		}

		double fVal = _INF_;
		if(nItems > 0)
			fVal = fSum / (double)nItems;

		data.m_pDates[nSize-(i-nEnd)-1]  = date;
		data.m_pValues[nSize-(i-nEnd)-1] = fVal;
	}
	
	return nSize;
}

int CDataManager::CalcDataPriceWeightedAve(CCompany* pBaseCompany, CArray<CCompany*,CCompany*>& arCompanies, const char* szItm, CData& data, COleDateTime from, COleDateTime to)
{
	int i, j;

	// special case for cumulative Returns
	char szItem[32];
	strcpy(szItem, szItm);
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		strcpy(szItem, "Price");
		bReturns = true;
	}

	// Get data index from item name
	int nItem = GetColIndexFromName(szItem);
	if(nItem == -1)
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", szItem);
		AfxMessageBox(sMessage);
		return 0;
	}

	CHistory* pBaseHistory = pBaseCompany->GetHistory();

	// Base company
	if(!pBaseHistory->IsItemAllLoaded(nItem))
		pBaseHistory->LoadItem(pBaseCompany->GetTicker(), szItem);

	int nStart, nEnd;
	pBaseHistory->GetStartEndIndexesForPeriod(from, to, nStart, nEnd);
	if(nStart == -1 || nEnd == -1)
	{
		CString sMessage; sMessage.Format("Invalid Dates requested for Ticker: '%s'", pBaseCompany->GetTicker());
		AfxMessageBox(sMessage);
		return 0;
	}

	int nSize = nStart - nEnd + 1;
	data.SetSize(nSize);

	double fPriceSum = 0;
	CArray<double,double> arStartPrices;

	// load the histories first and calculate the summary price
	for(j=0; j<arCompanies.GetSize(); j++)
	{
		CCompany* pComp = arCompanies[j];
		if(pComp != pBaseCompany)
		{
			CHistory* pHist = pComp->GetHistory();
			if(!pHist->IsItemAllLoaded(nItem))
				pHist->LoadItem(pComp->GetTicker(), szItem);
		}
	
		double fPrice = pComp->GetPrice();
		if(fPrice < _INF_)
			fPriceSum += fPrice;

		if(bReturns)
			arStartPrices.Add(_INF_);
	}

	if(fPriceSum == 0) // just in case (no 0 dividing)
		fPriceSum = 1;

	// walk through the dates of base company
	for(i=nStart; i>=nEnd; i--)
	{
		COleDateTime date = pBaseHistory->GetItemPtr(i)->date;

		double fSum = 0.0;
		int nItems = 0;
		for(j=0; j<arCompanies.GetSize(); j++)
		{
			CCompany* pComp = arCompanies[j];
			CHistory* pHist = pComp->GetHistory();

 			SHistoryItem* pHistItem = pHist->GetItemPtr(date);
			if(pHistItem)
			{
				double fVal = pHistItem->data[nItem];
				double fPrice = pComp->GetPrice();
				if(fVal < _INF_ && fPrice < _INF_)
				{
					if(bReturns)
					{
						if(arStartPrices[j] >= _INF_)
							arStartPrices[j] = fVal;
						fVal = 100*(fVal/arStartPrices[j] - 1);
					}					

					fSum += fVal * fPrice/fPriceSum;
					nItems ++;
				}
			}
		}

		double fVal = _INF_;
		if(nItems > 0)
			fVal = fSum ;

		data.m_pDates[nSize-(i-nEnd)-1]  = date;
		data.m_pValues[nSize-(i-nEnd)-1] = fVal;
	}
	

	return nSize;
}

int CDataManager::CalcDataMCapWeightedAve(CCompany* pBaseCompany, CArray<CCompany*,CCompany*>& arCompanies, const char* szItm, CData& data, COleDateTime from, COleDateTime to)
{
	int i, j;

	// special case for cumulative Returns
	char szItem[32];
	strcpy(szItem, szItm);
	BOOL bReturns = false;
	if(stricmp(szItem, "Returns") == 0 || stricmp(szItem, "Rets") == 0)
	{
		strcpy(szItem, "Price");
		bReturns = true;
	}

	// Get data index from item name
	int nItem = GetColIndexFromName(szItem);
	if(nItem == -1)
	{
		CString sMessage; sMessage.Format("Invalid Data Item requested: '%s'", szItem);
		AfxMessageBox(sMessage);
		return 0;
	}

	// get the company (its dates will be the basis for indexing)
	CHistory* pBaseHistory = pBaseCompany->GetHistory();

	// Base company
	if(!pBaseHistory->IsItemAllLoaded(nItem))
		pBaseHistory->LoadItem(pBaseCompany->GetTicker(), szItem);

	int nStart, nEnd;
	pBaseHistory->GetStartEndIndexesForPeriod(from, to, nStart, nEnd);
	if(nStart == -1 || nEnd == -1)
	{
		CString sMessage; sMessage.Format("Invalid Dates requested for Ticker: '%s'", pBaseCompany->GetTicker());
		AfxMessageBox(sMessage);
		return 0;
	}

	int nSize = nStart - nEnd + 1;
	data.SetSize(nSize);

	double fMarketCapSum = 0;
	CArray<double,double> arStartPrices;

	// load the histories first and calculate the summary price
	for(j=0; j<arCompanies.GetSize(); j++)
	{
		CCompany* pComp = arCompanies[j];
		if(pComp != pBaseCompany)
		{
			CHistory* pHist = pComp->GetHistory();
			if(!pHist->IsItemAllLoaded(nItem))
				pHist->LoadItem(pComp->GetTicker(), szItem);
		}

		double fPrice = pComp->GetPrice();
		double fShares = pComp->GetSharesOut();
		if(fPrice < _INF_ && fShares < _INF_)
			fMarketCapSum += fPrice*fShares;

		if(bReturns)
			arStartPrices.Add(_INF_);
	}

	if(fMarketCapSum == 0) // just in case (no 0 dividing)
		fMarketCapSum = 1;

	// walk through the dates of base company
	for(i=nStart; i>=nEnd; i--)
	{
		COleDateTime date = pBaseHistory->GetItemPtr(i)->date;

		double fSum = 0.0;
		int nItems = 0;
		for(j=0; j<arCompanies.GetSize(); j++)
		{
			CCompany* pComp = arCompanies[j];
			CHistory* pHist = pComp->GetHistory();

 			SHistoryItem* pHistItem = pHist->GetItemPtr(date);
			if(pHistItem)
			{
				double fVal = pHistItem->data[nItem];
				double fPrice = pComp->GetPrice();
				double fShares = pComp->GetSharesOut();
				if(fVal < _INF_ && fPrice < _INF_ && fShares < _INF_)
				{
					if(bReturns)
					{
						if(arStartPrices[j] >= _INF_)
							arStartPrices[j] = fVal;
						fVal = 100*(fVal/arStartPrices[j] - 1);
					}	

					fSum += fVal * (fPrice*fShares)/fMarketCapSum;
					nItems ++;
				}
			}
		}

		double fVal = _INF_;
		if(nItems > 0)
			fVal = fSum ;

		data.m_pDates[nSize-(i-nEnd)-1]  = date;
		data.m_pValues[nSize-(i-nEnd)-1] = fVal;
	}
	

	return nSize;
}

int CDataManager::GetDataForIndustryAveMethod1(const char* szCompany, const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	// get the company (its dates will be the basis for indexing)
	int nIndex = m_pUniverse->GetIndexByTicker(szCompany);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szCompany);
		AfxMessageBox(sMessage);
		return 0;
	}
	CCompany* pBaseCompany = m_pUniverse->GetCompany(nIndex);
	CString sIndustry = pBaseCompany->GetIndustry();

	// get companies in the industry
	CArray<CCompany*,CCompany*>	arCompanies;
	m_pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);
	
	return CalcDataSimpleAve(pBaseCompany, arCompanies, szItem, data, from, to);
}

int CDataManager::GetDataForIndustryAveMethod2(const char* szCompany, const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	// get the company (its dates will be the basis for indexing)
	int nIndex = m_pUniverse->GetIndexByTicker(szCompany);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szCompany);
		AfxMessageBox(sMessage);
		return 0;
	}
	CCompany* pBaseCompany = m_pUniverse->GetCompany(nIndex);
	CString sIndustry = pBaseCompany->GetIndustry();

	// get companies in the industry
	CArray<CCompany*,CCompany*>	arCompanies;
	m_pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);
	
	return CalcDataPriceWeightedAve(pBaseCompany, arCompanies, szItem, data, from, to);
}

int CDataManager::GetDataForIndustryAveMethod3(const char* szCompany, const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	// get the company (its dates will be the basis for indexing)
	int nIndex = m_pUniverse->GetIndexByTicker(szCompany);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szCompany);
		AfxMessageBox(sMessage);
		return 0;
	}
	CCompany* pBaseCompany = m_pUniverse->GetCompany(nIndex);
	CString sIndustry = pBaseCompany->GetIndustry();

	// get companies in the industry
	CArray<CCompany*,CCompany*>	arCompanies;
	m_pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);
	
	return CalcDataMCapWeightedAve(pBaseCompany, arCompanies, szItem, data, from, to);
}

int CDataManager::GetDataForSectorAveMethod1(const char* szCompany, const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	// get the company (its dates will be the basis for indexing)
	int nIndex = m_pUniverse->GetIndexByTicker(szCompany);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szCompany);
		AfxMessageBox(sMessage);
		return 0;
	}
	CCompany* pBaseCompany = m_pUniverse->GetCompany(nIndex);
	CString sSector = pBaseCompany->GetSector();

	// get companies in the industry
	CArray<CCompany*,CCompany*>	arCompanies;
	m_pUniverse->GetCompaniesInSector(sSector, arCompanies);
	
	return CalcDataSimpleAve(pBaseCompany, arCompanies, szItem, data, from, to);
}

int CDataManager::GetDataForSectorAveMethod2(const char* szCompany, const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	// get the company (its dates will be the basis for indexing)
	int nIndex = m_pUniverse->GetIndexByTicker(szCompany);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szCompany);
		AfxMessageBox(sMessage);
		return 0;
	}
	CCompany* pBaseCompany = m_pUniverse->GetCompany(nIndex);
	CString sSector = pBaseCompany->GetSector();

	// get companies in the industry
	CArray<CCompany*,CCompany*>	arCompanies;
	m_pUniverse->GetCompaniesInSector(sSector, arCompanies);
	
	return CalcDataPriceWeightedAve(pBaseCompany, arCompanies, szItem, data, from, to);
}

int CDataManager::GetDataForSectorAveMethod3(const char* szCompany, const char* szItem, CData& data, COleDateTime from, COleDateTime to)
{
	// get the company (its dates will be the basis for indexing)
	int nIndex = m_pUniverse->GetIndexByTicker(szCompany);
	if(nIndex == -1)
	{
		CString sMessage; sMessage.Format("Invalid Ticker requested: '%s'", szCompany);
		AfxMessageBox(sMessage);
		return 0;
	}
	CCompany* pBaseCompany = m_pUniverse->GetCompany(nIndex);
	CString sSector = pBaseCompany->GetSector();

	// get companies in the industry
	CArray<CCompany*,CCompany*>	arCompanies;
	m_pUniverse->GetCompaniesInSector(sSector, arCompanies);
	
	return CalcDataMCapWeightedAve(pBaseCompany, arCompanies, szItem, data, from, to);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDataManager::GetDataCollection(const char* tokens, const char* szBase, CDataCollection& dataCollection, COleDateTime from, COleDateTime to)
{
	// Check for Base, if there is one, get it first
	BOOL bBase = (szBase != 0 && strlen(szBase) > 0);
	CData *pBaseData = NULL;
	if(bBase)
	{
		pBaseData = new CData;
		strcpy(pBaseData->m_szToken, szBase);
		if(!GetDataForToken(szBase, *pBaseData, from, to) || pBaseData->GetSize()==0)
		{
			delete pBaseData;
			pBaseData = NULL;
			return 0;
		}

		from = pBaseData->m_pDates[0];
		to = pBaseData->m_pDates[pBaseData->GetSize()-1];
	}

	char szToken[32];
	const char* sz = tokens;
	const char* sz0 = sz;
	int nIndex = 0;
	while(*sz != '\0')
	{
		if(*sz == ' ')
		{
			strncpy(szToken, sz0, (size_t)(sz-sz0));
			szToken[sz-sz0] = '\0';

			if(strlen(szToken) > 0)
			{
				CData* pData = new CData;
				strcpy(pData->m_szToken, szToken);
				if(GetDataForToken(szToken, *pData, from, to))
					dataCollection.Add(pData);
				else
					delete pData;
			}
			
			while(*sz != '\0' && *sz == ' ')
				++sz;
			sz0 = sz;
		}
		++sz;
	}
	
	if(sz0 < sz)
	{
		strncpy(szToken, sz0, (size_t)(sz-sz0));
		szToken[sz-sz0] = '\0';

		if(strlen(szToken) > 0)
		{
			CData* pData = new CData;
			strcpy(pData->m_szToken, szToken);
			if(GetDataForToken(szToken, *pData, from, to))
				dataCollection.Add(pData);
			else
				delete pData;
		}
	} 

	// align to base
	if(bBase)
	{
		dataCollection.Add(pBaseData);
		for(int i=0; i<dataCollection.GetSize(); i++)
			AlignDataToBase(*dataCollection[i], *pBaseData);
	}

	return 1;
}

void CDataManager::AlignDataToBase(CData& data, CData& base)
{
	for(int i=0; i<data.GetSize(); i++)
	{
		double date = data.m_pDates[i];
		int nBaseIndex = base.GetIndexFromDate(date);
		if(nBaseIndex == -1)
			data.m_pValues[i] = _INF_;
		else
			data.m_pValues[i] -= base.m_pValues[nBaseIndex];
	}
}

void CDataManager::CopyDataCollection(const CDataCollection& src, CDataCollection& target)
{
	FreeDataCollection(target);
	
	for(int i=0; i<src.GetSize(); i++)
	{
		CData *data = new CData;
		*data = *src[i];		

		target.Add(data);
	}
}