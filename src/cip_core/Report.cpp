// Report.cpp: implementation of the CReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "Report.h"

#include <math.h>
#include <limits.h>
#include "BaseMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReport::CReport()
{

}

CReport::~CReport()
{

}

void CReport::ConstructDefaultFilter(SReportFilter& filter)
{
	filter.RemoveAll();

	filter.Add(SReportFilterItem("Assets",			"Assets",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Net Plant",		"Net Plant",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Intangibles",		"Intang.",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Cash",			"Cash",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Liabilities",		"Liab.",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Long Term Debt",	"LT Debt",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Short Term Debt",	"ST Debt",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Total Debt",		"Debt",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Common Equity",	"Cmn Eqty",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Preferred Stock",	"Preferred",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Net Worth",		"Net Worth",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Debt Ratio",		"Debt Rto",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Pre-Tax Income",	"PreTax Inc",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Interest Expense","Interest",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Depr & Amort",	"D&A",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Shares Out",		"Shares",			0,  -1000000,  1000000, 0));

	filter.Add(SReportFilterItem("Price",			"Price",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Dividend",		"Div",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Asset Life",		"Asst Life",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Book Value",		"Book",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Mean ROI",		"M.ROI",			0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Mean ROC",		"M.ROC",			0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Mean ROE",		"M.ROE",			0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("EPS Next Year",	"EPS N",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("EPS This Year",	"EPS T",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("EPS Last Year",	"EPS L",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Book EPS",		"B.EPS",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Normalized EPS",	"N.EPS",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Risk Adjustment",	"RiskAdj",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Yield",			"Yield",			0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Multiple",		"Mult",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("P/E",				"P/E",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("EV/EBITDA",		"EV/EBITDA",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Reinvest Rate",	"ReInv.Rt",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Max Reinvest Rate","Mx.Reinv.Rt",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Adj. for Payout",	"Payout",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Growth Rate",		"Growth",			0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Adjusted Growth",	"Adj.Growth",		0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Oper. Fix. Cover","Op F Cover",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Interest Cover",	"Int Cov",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Adj Assts/Share",	"Asst/Shr",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Net Cahs F/Share","NCF/Shr",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Depr&Amort/Share","D&P/Shr",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("BEV",				"BEV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("BEV Rank",		"BEV Rnk",			0,  -1000000,  1000000, 1));
	filter.Add(SReportFilterItem("Price/BEV",		"Prc/BEV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Histor. Growth",	"Hist Grwt",		0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Mo 1",			"Mo 1",				0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Mo 1 Rank",		"Mo 1 Rnk",			0,  -1000000,  1000000, 1));
	filter.Add(SReportFilterItem("Mo 2",			"Mo 2",				0,  -1000000,  1000000, 2));
	filter.Add(SReportFilterItem("Mo 2 Rank",		"Mo 2 Rnk",			0,  -1000000,  1000000, 1));
	
	filter.Add(SReportFilterItem("NEPV",			"NEPV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/NEPV",		"Prc/NEPV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("NEPV Rank",		"NEPV Rnk",			0,  -1000000,  1000000, 1));
	
	filter.Add(SReportFilterItem("FN",				"FN",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/FN",		"Prc/FN",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("FN Rank",			"FN Rnk",			0,  -1000000,  1000000, 1));

	filter.Add(SReportFilterItem("REPV",			"REPV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/REPV",		"Prc/REPV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("REPV Rank",		"REPV Rnk",			0,  -1000000,  1000000, 1));

	filter.Add(SReportFilterItem("FR",				"FR",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/FR",		"Prc/FR",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("FR Rank",			"FR Rnk",			0,  -1000000,  1000000, 1));


	filter.Add(SReportFilterItem("NRAV",			"NRAV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/NRAV",		"Prc/NRAV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("NRAV Rank",		"NRAV Rnk",			0,  -1000000,  1000000, 1));
	
	filter.Add(SReportFilterItem("FRAV",			"FRAV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/FRAV",		"Prc/FRAV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("FRAV Rank",		"FRAV Rnk",			0,  -1000000,  1000000, 1));

	filter.Add(SReportFilterItem("RRAV",			"RRAV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/RRAV",		"Prc/RRAV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("RRAV Rank",		"RRAV Rnk",			0,  -1000000,  1000000, 1));

	filter.Add(SReportFilterItem("FRRAV",			"FRRAV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/FRRAV",		"Prc/FRRAV",		0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("FRRAV Rank",		"FRRAV Rnk",		0,  -1000000,  1000000, 1));


	filter.Add(SReportFilterItem("PMV"	,			"PMV",				0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("Price/PMV",		"Prc/PMV",			0,  -1000000,  1000000, 0));
	filter.Add(SReportFilterItem("PMV Rank",		"PMV Rnk",			0,  -1000000,  1000000, 1));
}

BOOL CReport::SortByColumn(SReportGroup& group, int nColumn, BOOL bAscending)
{
	while (1)
	{
		BOOL bContinue=false;
		SortAsDouble(group, nColumn, group.arItems.GetSize(), bAscending, 0, -1, bContinue);

		if(!bContinue)
			break;
	}
	return true;
}

BOOL CReport::SortAsDouble(SReportGroup& group, int nColumn, int nSize, BOOL bAscending, int low, int high, BOOL &bContniue)
{
	if (high == -1) 
		high = nSize - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) 
		return FALSE;

	double fItem = group.arItems[(lo+hi)/2]->arData[nColumn];

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if( bAscending )
			while (lo < high  && group.arItems[lo]->arData[nColumn] < fItem)
				++lo;
		else
			while (lo < high && group.arItems[lo]->arData[nColumn] > fItem)
				++lo;

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if( bAscending )
			while (hi > low &&  group.arItems[hi]->arData[nColumn] > fItem)
				--hi;
		else
			while (hi > low && group.arItems[hi]->arData[nColumn] < fItem)
				--hi;

		// If the indexes have not crossed, swap if the items are not equal
		if (lo <= hi)
		{
			// swap only if the items are not equal
			if (fabs(group.arItems[lo]->arData[nColumn] - group.arItems[hi]->arData[nColumn]) > 0.000000001)
			{
				bContniue = true;

				SReportItem* pTemp = group.arItems[lo];
				group.arItems[lo] = group.arItems[hi];
				group.arItems[hi] = pTemp;
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortAsDouble(group, nColumn,nSize, bAscending, low, hi, bContniue);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortAsDouble(group, nColumn,nSize, bAscending,lo, high, bContniue);

    return true;
}

void CReport::CalcAves(SReportGroup& group, CArray<CCompany*,CCompany*>& arCompanies, double fCapTotal, double fPriceTotal)
{
	int nSize = group.arItems.GetSize();
	if(nSize == 0)
	{
		group.pAve = 0;
		group.pCapAve = 0;
		group.pPriceAve = 0;
		return;
	}
	int nItems = group.arItems[0]->arData.GetSize();

	SReportItem* pAve = new SReportItem;
	pAve->sTicker.Format("Avg:%u", nSize);

	SReportItem* pAveCap = new SReportItem;
	pAveCap->sTicker = "Cap Wgt";

	SReportItem* pAvePrice = new SReportItem;
	pAvePrice->sTicker = "Prc Wgt";

	for(int nItem=0; nItem<nItems; nItem++)
	{
		double fAve = 0;
		double fAveCap = 0;
		double fAvePrice = 0;
		for(int i=0; i<nSize; i++)
		{
			double val = group.arItems[i]->arData[nItem];
			CCompany* pCompany = arCompanies[i];
			if(val < _INF_ && pCompany->GetPrice() < _INF_ && pCompany->GetSharesOut() < _INF_) 
			{
				//double fCap = pCompany->GetPrice()*pCompany->GetSharesOut();
				//double fPrice = pCompany->GetPrice()*pCompany->GetPrice()*pCompany->GetSharesOut();

				double fCap = pCompany->GetSharesOut();
				double fPrice = pCompany->GetPrice()*pCompany->GetSharesOut();
				
				fAve += val / (double)nSize;
				fAveCap += val * fCap / fCapTotal;
				fAvePrice += val * fPrice / fPriceTotal;
			}
		}

		pAve->arData.Add(fAve);
		pAveCap->arData.Add(fAveCap);
		pAvePrice->arData.Add(fAvePrice);
	}

	group.pAve = pAve;
	group.pCapAve = pAveCap;
	group.pPriceAve = pAvePrice;
}

void CReport::GetEBITDA_Leverage(CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
								double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReport& rep)
{
	rep.bSortByIndustry = false;
	rep.bDontPrintIndStocks = bNotIndividualNames;

	rep.sName = "EBITDA And Financial Leverage Report";
	rep.sOptCaption = szTitle;

	static char* szSort[7] = {
		"Ticker Symbol Sort", 
		"Times EBITDA Sort", 
		"Debt to Common Equity Sort",
		"Debt to Tangible Equity Sort",
		"Operating Fixed Charge Coverage Sort",
		"Fixed Charge Coverage",
		"Market Capitalization"
	};

	rep.sSortAndDate = szSort[nSort];
	rep.sSortAndDate += " - ";
	rep.sSortAndDate += COleDateTime::GetCurrentTime().Format("%m/%d/%Y");
	rep.sSortAndDate += "  (In Millions)";

	static char* szCols[19] = 
	{
		"Symbol",
		"Shares OutStnd",
		"Pre-Tax Income",
		"Interest Expense",
		"Op FC Coverg",
		"Interest Coverg",
		"Dep & Amort",
		"EBITDA",
		"Short T. Debt",
		"Long T. Debt",
		"Prefrrd Stock",
		"Comm Equity",
		"Mkt Cap",
		"Cash",
		"Entrprs Value",
		"Times EBITDA",
		"Intangibles",
		"Debt / Com.Eq",
		"Debt / Tang.Eq"
	};

	for(int i=0; i<19; i++)
		rep.arColNames.Add(szCols[i]);

	if(nGrouping == 0)
	{
		SReportGroup* pGroup = new SReportGroup;
		rep.arGroups.Add(pGroup);

		CArray<CCompany*,CCompany*> arCompanies;
		for(int i=0; i<pUniverse->GetSize(); i++)
			arCompanies.Add(pUniverse->GetCompany(i));

		GetEBITDA_LeverageGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
	}
	else if(nGrouping == 1)
	{
		int nIndustries = pUniverse->GetIndustriesCount();
		for(int i=0; i<nIndustries; i++)
		{
			CString sIndustry = pUniverse->GetIndustryName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sIndustry;
			rep.arGroups.Add(pGroup);

			GetEBITDA_LeverageGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
		}	
	}
	else if(nGrouping == 2)
	{
		int nSectors = pUniverse->GetSectorsCount();
		for(int i=0; i<nSectors; i++)
		{
			CString sSector = pUniverse->GetSectorName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInSector(sSector, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sSector;
			rep.arGroups.Add(pGroup);

			GetEBITDA_LeverageGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
		}	
	}
}

void CReport::GetEBITDA_LeverageGroup(	CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
										double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportGroup* &pGroup)
{
	// for Ave calculations
	double fCapTotal = 0;
	double fPriceTotal = 0;

	for(int i=0; i<arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = arCompanies[i];
		double fMCap = pCompany->GetPrice()*pCompany->GetSharesOut();
		if(fMCap < fMinCap || fMCap > fMaxCap)
			continue;

		if(arFilters[0] != 1) // All Companies
		{
			BOOL bIn = false;
			for(int k=0; k<NUM_WEIGHTS; k++)
			{
				double w = pCompany->GetWeightInIndex(k);
				if(w > 0 && arFilters[k+1] == 1)
				{
					bIn = true;
					break;
				}
			}

			if(!bIn)
				continue;
		}

		if(pCompany->GetPrice() < _INF_ && pCompany->GetSharesOut() < _INF_) 
		{
			fCapTotal += pCompany->GetSharesOut();
			fPriceTotal += pCompany->GetPrice()*pCompany->GetSharesOut();
		}

		SReportItem* pItem = new SReportItem;
		pGroup->arItems.Add(pItem);

		pItem->sTicker = pCompany->GetTicker();

		pItem->arData.Add(pCompany->GetSharesOut());
		pItem->arData.Add(pCompany->GetPreTaxIncome());
		pItem->arData.Add(pCompany->GetInterestExpense());
		pItem->arData.Add(pCompany->GetOperFixChargeCover());
		pItem->arData.Add(pCompany->GetInterestCover());
		pItem->arData.Add(pCompany->GetDeprAndAmort());

		double fEBITDA = pCompany->GetPreTaxIncome()+pCompany->GetInterestExpense()+pCompany->GetDeprAndAmort();
		pItem->arData.Add(fEBITDA);

		pItem->arData.Add(pCompany->GetShortTermDebt());
		pItem->arData.Add(pCompany->GetLongTermDebt());
		pItem->arData.Add(pCompany->GetPreferred());
		pItem->arData.Add(pCompany->GetCommonEquity());
		pItem->arData.Add(pCompany->GetSharesOut()*pCompany->GetPrice());
		pItem->arData.Add(pCompany->GetCash());

		double fEV = pCompany->GetSharesOut()*pCompany->GetPrice() + pCompany->GetLongTermDebt() + pCompany->GetShortTermDebt() - pCompany->GetCash();
		pItem->arData.Add(fEV);

		pItem->arData.Add((fEBITDA!=0) ? fEV/fEBITDA : _INF_);
		pItem->arData.Add(pCompany->GetIntangibles());

		if(pCompany->GetCommonEquity() < _INF_ && pCompany->GetCommonEquity()!=0)
			pItem->arData.Add( (pCompany->GetLongTermDebt()+pCompany->GetShortTermDebt()) / pCompany->GetCommonEquity() );
		else
			pItem->arData.Add(_INF_);

		if(pCompany->GetCommonEquity()<_INF_ && pCompany->GetIntangibles()<_INF_ && pCompany->GetCommonEquity()!=0 && pCompany->GetIntangibles()!=0)
			pItem->arData.Add( (pCompany->GetLongTermDebt()+pCompany->GetShortTermDebt()) / (pCompany->GetCommonEquity()-pCompany->GetIntangibles()) );
		else
			pItem->arData.Add(_INF_);
	}

	CalcAves(*pGroup, arCompanies, fCapTotal, fPriceTotal);

	if(bNotIndividualNames)
	{
		for(int i=0; i<pGroup->arItems.GetSize(); i++)
			delete pGroup->arItems[i];
		pGroup->arItems.RemoveAll();
	}
	else if(nSort > 0)
	{
		int nCol = 1;
		switch(nSort)
		{
		case 1: nCol = 14; break;
		case 2: nCol = 16; break;
		case 3: nCol = 17; break;
		case 4: nCol = 3; break;
		case 5: nCol = 4; break;
		case 6: nCol = 11; break;
		}
		SortByColumn(*pGroup, nCol, true);
	}
}

void CReport::GetEarningsMomentum(CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReport& rep)
{
	rep.bSortByIndustry = false;
	rep.bDontPrintIndStocks = bNotIndividualNames;

	rep.sName = "Earnings Momentum Report";
	rep.sOptCaption = szTitle;

	static char* szSort[5] = {
		"Ticker Symbol Sort", 
		"Prc / NEPV Sort", 
		"Prc / REPV Sort",
		"Mo 1 Rank Sort",
		"Mo 2 Rank Sort"
	};

	rep.sSortAndDate = szSort[nSort];
	rep.sSortAndDate += " - ";
	rep.sSortAndDate += COleDateTime::GetCurrentTime().Format("%m/%d/%Y");
	rep.sSortAndDate += "  (In Millions)";

	static char* szCols[17] = 
	{
		"Symbol",
		"Co. Name",
		"Price",
		"NEPV",
		"Prc / NEPV",
		"Rnk",
		"REPV",
		"Prc / REPV",
		"Rnk",
		"Mo 1",
		"Rnk",
		"Mo 2",
		"Rnk",
		"Eps L",
		"Eps T",
		"Eps N",
		"Yr"
	};

	for(int i=0; i<17; i++)
		rep.arColNames.Add(szCols[i]);

	if(nGrouping == 0)
	{
		SReportGroup* pGroup = new SReportGroup;
		rep.arGroups.Add(pGroup);

		CArray<CCompany*,CCompany*> arCompanies;
		for(int i=0; i<pUniverse->GetSize(); i++)
			arCompanies.Add(pUniverse->GetCompany(i));

		GetEarningsMomentumGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
	}
	else if(nGrouping == 1)
	{
		int nIndustries = pUniverse->GetIndustriesCount();
		for(int i=0; i<nIndustries; i++)
		{
			CString sIndustry = pUniverse->GetIndustryName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sIndustry;
			rep.arGroups.Add(pGroup);

			GetEarningsMomentumGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
		}	
	}
	else if(nGrouping == 2)
	{
		int nSectors = pUniverse->GetSectorsCount();
		for(int i=0; i<nSectors; i++)
		{
			CString sSector = pUniverse->GetSectorName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInSector(sSector, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sSector;
			rep.arGroups.Add(pGroup);

			GetEarningsMomentumGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
		}	
	}	
}

void CReport::GetEarningsMomentumGroup(	CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
										double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportGroup* &pGroup)
{
	// for Ave calculations
	double fCapTotal = 0;
	double fPriceTotal = 0;

	for(int i=0; i<arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = arCompanies[i];
		double fMCap = pCompany->GetPrice()*pCompany->GetSharesOut();
		if(fMCap < fMinCap || fMCap > fMaxCap)
			continue;

		if(arFilters[0] != 1) // All Companies
		{
			BOOL bIn = false;
			for(int k=0; k<NUM_WEIGHTS; k++)
			{
				double w = pCompany->GetWeightInIndex(k);
				if(w > 0 && arFilters[k+1] == 1)
				{
					bIn = true;
					break;
				}
			}

			if(!bIn)
				continue;
		}

		if(pCompany->GetPrice() < _INF_ && pCompany->GetSharesOut() < _INF_) 
		{
			fCapTotal += pCompany->GetSharesOut();
			fPriceTotal += pCompany->GetPrice()*pCompany->GetSharesOut();
		}

		SReportItem* pItem = new SReportItem;
		pGroup->arItems.Add(pItem);

		pItem->sTicker = pCompany->GetTicker();
		pItem->sName = pCompany->GetName();

		pItem->arData.Add(pCompany->GetPrice());

		pItem->arData.Add(pCompany->GetEPV());
		pItem->arData.Add(pCompany->GetPrice()/pCompany->GetEPV());
		pItem->arData.Add(pCompany->GetRankEPV());

		pItem->arData.Add(pCompany->GetEPVRazor());
		pItem->arData.Add(pCompany->GetPrice()/pCompany->GetEPVRazor());
		pItem->arData.Add(pCompany->GetRankEPVRazor());

		pItem->arData.Add(pCompany->GetGrowthEstimate1());
		pItem->arData.Add(pCompany->GetRankGrowth1());
		pItem->arData.Add(pCompany->GetGrowthEstimate2());
		pItem->arData.Add(pCompany->GetRankGrowth2());

		pItem->arData.Add(pCompany->GetEPSForecastLastYear());
		pItem->arData.Add(pCompany->GetEPSForecastThisYear());
		pItem->arData.Add(pCompany->GetEPSForecastNextYear());
		pItem->arData.Add(pCompany->GetYear()+1);
	}

	CalcAves(*pGroup, arCompanies, fCapTotal, fPriceTotal);

	if(bNotIndividualNames)
	{
		for(int i=0; i<pGroup->arItems.GetSize(); i++)
			delete pGroup->arItems[i];
		pGroup->arItems.RemoveAll();
	}
	else if(nSort > 0)
	{
		int nCol = 1;
		switch(nSort)
		{
		case 1: nCol = 2; break;
		case 2: nCol = 5; break;
		case 3: nCol = 7; break;
		case 4: nCol = 9; break;
		}
		SortByColumn(*pGroup, nCol, true);
	}
}

void CReport::GetValueMomentum1(CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
								double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReport& rep)
{
	rep.bSortByIndustry = false;
	rep.bDontPrintIndStocks = bNotIndividualNames;

	rep.sName = "Earnings Momentum Report";
	rep.sOptCaption = szTitle;

	static char* szSort[5] = {
		"Ticker Symbol Sort", 
		"Price / NEPV Sort", 
		"Price / Forecast Sort",
		"Price / Razor Sort", 
		"Price / Razor Forecast Sort", 
	};

	rep.sSortAndDate = szSort[nSort];
	rep.sSortAndDate += " - ";
	rep.sSortAndDate += COleDateTime::GetCurrentTime().Format("%m/%d/%Y");
	rep.sSortAndDate += "  (In Millions)";

	static char* szCols[19] = 
	{
		"Symbol",
		"Co. Name",
		"Price",
		"NEPV",
		"Prc / NEPV",
		"Rnk",
		"FN",
		"Prc / FN",
		"Rnk",
		"REPV",
		"Prc / REPV",
		"Rnk",
		"FR",
		"Prc / FR",
		"Rnk",
		"Eps L",
		"Eps T",
		"Eps N",
		"Yr"
	};

	for(int i=0; i<19; i++)
		rep.arColNames.Add(szCols[i]);

	if(nGrouping == 0)
	{
		SReportGroup* pGroup = new SReportGroup;
		rep.arGroups.Add(pGroup);

		CArray<CCompany*,CCompany*> arCompanies;
		for(int i=0; i<pUniverse->GetSize(); i++)
			arCompanies.Add(pUniverse->GetCompany(i));

		GetValueMomentumGroup1(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
	}
	else if(nGrouping == 1)
	{
		int nIndustries = pUniverse->GetIndustriesCount();
		for(int i=0; i<nIndustries; i++)
		{
			CString sIndustry = pUniverse->GetIndustryName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sIndustry;
			rep.arGroups.Add(pGroup);

			GetValueMomentumGroup1(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
		}	
	}
	else if(nGrouping == 2)
	{
		int nSectors = pUniverse->GetSectorsCount();
		for(int i=0; i<nSectors; i++)
		{
			CString sSector = pUniverse->GetSectorName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInSector(sSector, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sSector;
			rep.arGroups.Add(pGroup);

			GetValueMomentumGroup1(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, pGroup);
		}	
	}	
}

void CReport::GetValueMomentumGroup1(	CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
										double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportGroup* &pGroup)
{
	// for Ave calculations
	double fCapTotal = 0;
	double fPriceTotal = 0;

	for(int i=0; i<arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = arCompanies[i];
		double fMCap = pCompany->GetPrice()*pCompany->GetSharesOut();
		if(fMCap < fMinCap || fMCap > fMaxCap)
			continue;

		if(arFilters[0] != 1) // All Companies
		{
			BOOL bIn = false;
			for(int k=0; k<NUM_WEIGHTS; k++)
			{
				double w = pCompany->GetWeightInIndex(k);
				if(w > 0 && arFilters[k+1] == 1)
				{
					bIn = true;
					break;
				}
			}

			if(!bIn)
				continue;
		}

		if(pCompany->GetPrice() < _INF_ && pCompany->GetSharesOut() < _INF_) 
		{
			fCapTotal += pCompany->GetSharesOut();
			fPriceTotal += pCompany->GetPrice()*pCompany->GetSharesOut();
		}

		SReportItem* pItem = new SReportItem;
		pGroup->arItems.Add(pItem);

		pItem->sTicker = pCompany->GetTicker();
		pItem->sName = pCompany->GetName();

		pItem->arData.Add(pCompany->GetPrice());

		pItem->arData.Add(pCompany->GetEPV());
		pItem->arData.Add(pCompany->GetPrice()/pCompany->GetEPV());
		pItem->arData.Add(pCompany->GetRankEPV());

		pItem->arData.Add(pCompany->GetEPVFore());
		pItem->arData.Add(pCompany->GetPrice()/pCompany->GetEPVFore());
		pItem->arData.Add(pCompany->GetRankEPVFore());

		pItem->arData.Add(pCompany->GetEPVRazor());
		pItem->arData.Add(pCompany->GetPrice()/pCompany->GetEPVRazor());
		pItem->arData.Add(pCompany->GetRankEPVRazor());

		pItem->arData.Add(pCompany->GetEPVRazorFore());
		pItem->arData.Add(pCompany->GetPrice()/pCompany->GetEPVRazorFore());
		pItem->arData.Add(pCompany->GetRankEPVRazorFore());

		pItem->arData.Add(pCompany->GetEPSForecastLastYear());
		pItem->arData.Add(pCompany->GetEPSForecastThisYear());
		pItem->arData.Add(pCompany->GetEPSForecastNextYear());
		pItem->arData.Add(pCompany->GetYear()+1);
	}

	CalcAves(*pGroup, arCompanies, fCapTotal, fPriceTotal);

	if(bNotIndividualNames)
	{
		for(int i=0; i<pGroup->arItems.GetSize(); i++)
			delete pGroup->arItems[i];
		pGroup->arItems.RemoveAll();
	}
	else if(nSort > 0)
	{
		int nCol = 1;
		switch(nSort)
		{
		case 1: nCol = 2; break;
		case 2: nCol = 5; break;
		case 3: nCol = 8; break;
		case 4: nCol = 11; break;
		}
		SortByColumn(*pGroup, nCol, true);
	}
}

void CReport::GetGeneral( CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
						double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportFilter& filter, SReport& rep)
{
	rep.bSortByIndustry = false;
	rep.bDontPrintIndStocks = bNotIndividualNames;

	rep.sName = "General Report";
	rep.sOptCaption = szTitle;

	if(nSort != -1)
	{
		int n=-1; 
		for(int i=0; i<=filter.GetSize(); i++)
			if(filter[i].bInclude)
			{
				++n;
				if(n==nSort)
				{
					n=i;
					break;
				}
			}
 		rep.sSortAndDate = filter[n].szName;
	}
	else
		rep.sSortAndDate = "Ticker Symbol";
	rep.sSortAndDate += " Sort - ";
	rep.sSortAndDate += COleDateTime::GetCurrentTime().Format("%m/%d/%Y");
	rep.sSortAndDate += "  (In Millions)";

	rep.arColNames.Add("Symbol");
	for(int i=0; i<filter.GetSize(); i++)
		if(filter[i].bInclude)
			rep.arColNames.Add(filter[i].szNameShort);

	if(nGrouping == 0)
	{
		SReportGroup* pGroup = new SReportGroup;
		rep.arGroups.Add(pGroup);

		CArray<CCompany*,CCompany*> arCompanies;
		for(int i=0; i<pUniverse->GetSize(); i++)
			arCompanies.Add(pUniverse->GetCompany(i));

		GetGeneralGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, filter, pGroup);
	}
	else if(nGrouping == 1)
	{
		int nIndustries = pUniverse->GetIndustriesCount();
		for(int i=0; i<nIndustries; i++)
		{
			CString sIndustry = pUniverse->GetIndustryName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInIndustry(sIndustry, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sIndustry;
			rep.arGroups.Add(pGroup);

			GetGeneralGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, filter, pGroup);
		}	
	}
	else if(nGrouping == 2)
	{
		int nSectors = pUniverse->GetSectorsCount();
		for(int i=0; i<nSectors; i++)
		{
			CString sSector = pUniverse->GetSectorName(i);
			CArray<CCompany*,CCompany*> arCompanies;
			pUniverse->GetCompaniesInSector(sSector, arCompanies);

			SReportGroup* pGroup = new SReportGroup;
			pGroup->sGroupName = sSector;
			rep.arGroups.Add(pGroup);

			GetGeneralGroup(arCompanies, bNotIndividualNames, nSort, fMinCap, fMaxCap, arFilters, filter, pGroup);
		}	
	}
}

void CReport::GetGeneralGroup(	CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
								double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportFilter& filter, SReportGroup* &pGroup)
{
	// for Ave calculations
	double fCapTotal = 0;
	double fPriceTotal = 0;

	for(int i=0; i<arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = arCompanies[i];
		double fMCap = pCompany->GetPrice()*pCompany->GetSharesOut();
		if(fMCap < fMinCap || fMCap > fMaxCap)
			continue;

		if(arFilters[0] != 1) // All Companies
		{
			BOOL bIn = false;
			for(int k=0; k<NUM_WEIGHTS; k++)
			{
				double w = pCompany->GetWeightInIndex(k);
				if(w > 0 && arFilters[k+1] == 1)
				{
					bIn = true;
					break;
				}
			}

			if(!bIn)
				continue;
		}

		if(pCompany->GetPrice() < _INF_ && pCompany->GetSharesOut() < _INF_) 
		{
			fCapTotal += pCompany->GetSharesOut();
			fPriceTotal += pCompany->GetPrice()*pCompany->GetSharesOut();
		}

		SReportItem* pItem = new SReportItem;

		pItem->sTicker = pCompany->GetTicker();
		pItem->sName = pCompany->GetName();

		BOOL bRemove = false;
		for(int j=0; j<filter.GetSize(); j++)
		{
			if(!filter[j].bInclude)
				continue;
			
			double val = GetCompanyItem(pCompany, j);
			if( (val < _INF_) && (val < filter[j].fMin || val > filter[j].fMax))
			{
				bRemove = true;
				break;
			}

			pItem->arData.Add(val);

		}
		if(bRemove)
		{
			delete pItem;
			continue;
		}

		pGroup->arItems.Add(pItem);
	}

	CalcAves(*pGroup, arCompanies, fCapTotal, fPriceTotal);

	if(bNotIndividualNames)
	{
		for(int i=0; i<pGroup->arItems.GetSize(); i++)
			delete pGroup->arItems[i];
		pGroup->arItems.RemoveAll();
	}
	else if(nSort != -1)
	{
		SortByColumn(*pGroup, nSort, true);
	}
}

double CReport::GetCompanyItem(CCompany* pCompany, int nIndex)
{
	switch(nIndex)
	{
	case 0:				return pCompany->GetAssets();
	case 1:				return pCompany->GetNetPlant();
	case 2:				return pCompany->GetIntangibles();
	case 3:				return pCompany->GetCash();
	case 4:				return pCompany->GetLiabilities();
	case 5:				return pCompany->GetLongTermDebt();
	case 6:				return pCompany->GetShortTermDebt();
	case 7:				return pCompany->GetTotalDebt();
	case 8:				return pCompany->GetCommonEquity();
	case 9:				return pCompany->GetPreferred();
	case 10:			return pCompany->GetNetWorth();
	case 11:			return pCompany->GetDebtRatio();
	case 12:			return pCompany->GetPreTaxIncome();
	case 13:			return pCompany->GetInterestExpense();
	case 14:			return pCompany->GetDeprAndAmort();
	case 15:			return pCompany->GetSharesOut();

	case 16:			return pCompany->GetPrice();
	case 17:			return pCompany->GetDividend();
	case 18:			return pCompany->GetAssetLife();
	case 19:			return pCompany->GetBookValue();
	case 20:			return pCompany->GetMeanROI();
	case 21:			return pCompany->GetMeanROC();
	case 22:			return pCompany->GetMeanROE();
	case 23:			return pCompany->GetEPSForecastNextYear();
	case 24:			return pCompany->GetEPSForecastThisYear();
	case 25:			return pCompany->GetEPSForecastLastYear();
	case 26:			return pCompany->GetBookEPS();
	case 27:			return pCompany->GetNormEPS();
	case 28:			return pCompany->GetRiskAdjustment();
	case 29:			return pCompany->GetYield();
	case 30:			return pCompany->GetMultiple();
	case 31:			return pCompany->GetPE();
	case 32:			return pCompany->GetEVOverEBITDA();
	case 33:			return pCompany->GetReinvestmentRate();
	case 34:			return pCompany->GetMaxReinvestmentRate();
	case 35:			return pCompany->GetAdjustmentForPayout();
	case 36:			return pCompany->GetGrowthRate();
	case 37:			return pCompany->GetAdjustedGrowthRate();
	case 38:			return pCompany->GetOperFixChargeCover();
	case 39:			return pCompany->GetInterestCover();
	case 40:			return pCompany->GetAdjustedAssetsOverShare();
	case 41:			return pCompany->GetNetCashFlowOverShare();
	case 42:			return pCompany->GetDeprAndAmortOverShare();
	case 43:			return pCompany->GetBreakEvenValue();
	case 44:			return pCompany->GetRankBEV();
	case 45:			return pCompany->GetPriceOverBEV();
	case 46:			return pCompany->GetHistoricalGrowth();
	case 47:			return pCompany->GetGrowthEstimate1();
	case 48:			return pCompany->GetRankGrowth1();
	case 49:			return pCompany->GetGrowthEstimate2();
	case 50:			return pCompany->GetRankGrowth2();

	case 51:			return pCompany->GetEPV();
	case 52:			return pCompany->GetPriceOverEPV();
	case 53:			return pCompany->GetRankEPV();

	case 54:			return pCompany->GetEPVFore();
	case 55:			return pCompany->GetPriceOverEPVFore();
	case 56:			return pCompany->GetRankEPVFore();

	case 57:			return pCompany->GetEPVRazor();
	case 58:			return pCompany->GetPriceOverEPVRazor();
	case 59:			return pCompany->GetRankEPVRazor();

	case 60:			return pCompany->GetEPVRazorFore();
	case 61:			return pCompany->GetPriceOverEPVRazorFore();
	case 62:			return pCompany->GetRankEPVRazorFore();

	case 63:			return pCompany->GetRAV();
	case 64:			return pCompany->GetPriceOverRAV();
	case 65:			return pCompany->GetRankRAV();

	case 66:			return pCompany->GetRAVFore();
	case 67:			return pCompany->GetPriceOverRAVFore();
	case 68:			return pCompany->GetRankRAVFore();

	case 69:			return pCompany->GetRAVRazor();
	case 70:			return pCompany->GetPriceOverRAVRazor();
	case 71:			return pCompany->GetRankRAVRazor();

	case 72:			return pCompany->GetRAVRazorFore();
	case 73:			return pCompany->GetPriceOverRAVRazorFore();
	case 74:			return pCompany->GetRankRAVRazorFore();

	case 75:			return pCompany->GetPMV();
	case 76:			return pCompany->GetPriceOverPMV();
	case 77:			return pCompany->GetRankPMV();
	}

	return _INF_;
}
