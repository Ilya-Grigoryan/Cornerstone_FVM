#if !defined(AFX_INPUT_H__4C0F1AFC_56C6_4BED_BA7D_A1854A307B4C__INCLUDED_)
#define AFX_INPUT_H__4C0F1AFC_56C6_4BED_BA7D_A1854A307B4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define MAX_NUM_COMPANIES_IN_UNIVERSE	5000
#define NUM_YEARS_FOR_EPS			11
#define NUM_YEARS_FOR_ROC			13

#define NUM_COLUMNS_IN_HISTORY_EXCEL		19		// Total 22 - 3
#define NUM_COLUMNS_IN_HISTORY_DB			55		// + 36 new items

#define NUM_WEIGHTS							9

//////////////////////////////////////////////////////////////////////////////////////
static char *WeightsIndices[NUM_WEIGHTS] = 
{
	"c30",
	"r1000v",
	"lcv",
	"sp500",
	"bev",
	"c20",
	"rmid",
	"r1000",
	"r1000g"
};

//////////////////////////////////////////////////////////////////////////////////////
struct SCompanyWeightsInIndices
{
	char sTicker[8];
	double data[NUM_WEIGHTS];
};
typedef CArray<SCompanyWeightsInIndices, SCompanyWeightsInIndices>	SUniverseWeightsInIndices;

//////////////////////////////////////////////////////////////////////////////////
struct SCompanyDataPricesAndReturns
{
	char			sTicker[8];
	COleDateTime	date;
	double			fPrice;
	double			fReturn;
};
typedef CArray<SCompanyDataPricesAndReturns, SCompanyDataPricesAndReturns>	SUniverseDataPricesAndReturns;

//////////////////////////////////////////////////////////////////////////////////
struct SCompanyDataEpsRocRoe
{
	char			sTicker[8];
	int				nYear0;
	double			arEps[NUM_YEARS_FOR_EPS];
	double			arRoc[NUM_YEARS_FOR_ROC];
	double			arRoe[NUM_YEARS_FOR_ROC];
	double			arShares[NUM_YEARS_FOR_ROC];
	double			fAdjustMult; 
	double			fAdjustAdd;
};
typedef CArray<SCompanyDataEpsRocRoe, SCompanyDataEpsRocRoe>	SUniverseDataEpsRocRoe;


//////////////////////////////////////////////////////////////////////////////////
struct SCompanyDataShares
{
	char			sTicker[8];
	int				nYear0;
	double			arShares[NUM_YEARS_FOR_EPS];
};
typedef CArray<SCompanyDataShares, SCompanyDataShares>	SUniverseDataShares;


//////////////////////////////////////////////////////////////////////////////////
struct SCompanyDataFundamentals
{
	SCompanyDataFundamentals()
	{ type = -1; bLoadBS = bLoadEPS = bLoadROCROE = bUseDebtAdjust = -1; bRollOver = 0;}

	char			sTicker[8];
	char			sName[64];
	char			sIndustry[64];
	char			sSector[64];

	char			sCIQTicker[64];

	unsigned int	type;
	unsigned int	nLastReport;
	BOOL			bInSP;
	BOOL			bRollOver;

	double			fPrice;

	COleDateTime	dtReport;
	double			fShares;
	double			fFixed;
	double			fDeprAmort;
	double			fAssets;
	double			fIntang; 
	double			fLiabilities;
	double			fShortDebt;
	double			fLongDebt;
	double			fCommonEquity;
	double			fCash;
	double			fInterest;
	double			fPreTaxIncome;
	double			fEpsLastYear;
	double			fEpsLast12Months;
	double			fEpsThisYForecast;
	double			fEpsNextYForecast;
	double			fDiv;
	double			fBookVal;
	char			sSplit[64];
	COleDateTime	dtSplit;

	BOOL			bLoadBS;
	BOOL			bLoadEPS;
	BOOL			bLoadROCROE;
	BOOL			bUseDebtAdjust;

	SCompanyWeightsInIndices	Weights;
};
typedef CArray<SCompanyDataFundamentals, SCompanyDataFundamentals>	SUniverseDataFundamentals;


////////////////////////////////////////////////////////////////////////////////////
struct SCompanyRawData
{
	SCompanyDataFundamentals		Fundamentals;
	SCompanyDataEpsRocRoe			EpsRocRoe;
	SCompanyDataPricesAndReturns	PricesAndReturns;
	SCompanyWeightsInIndices		WeightsInIndices;
	SCompanyDataShares				Shares;
};
typedef CArray<SCompanyRawData, SCompanyRawData>	SUniverseRawData;


//////////////////////////////////////////////////////////////////////////////////////
static char *HistCols[] = 
{
	"Ret",
	"Price",
	"RankNEPV",
	"RankREPV",
	"RankNRAV",
	"RankBEV",
	"RankPMV",
	"NEPV",
	"CYFN",
	"MomentN",
	"RankMomentN",
	"REPV",
	"CYFR",
	"MomentR",
	"RankMomentR",
	"Mo1",
	"RankMo1",
	"Mo2",
	"RankMo2",

	"Multiple",
	"ROI",
	"Growth",
	"Risk",
	"Yield",
	"PE",
	"EVEBITDA",
	"Payout",
	"NormEPS",
	
	"BookEPS",
	"BookVal",
	"DebtRatio",
	"MeanROC",
	"MeanROE",
	"MeanEPS",
	"AssetsShare",
	"NetCFShare",
	"DAShare",
	"OpFixCover",
	"IntCover",

	"Dividend",
	"EPSNext",
	"EPSThis",
	"EPSLast",
	"AssetLife",
	"HistGrowth",
	"IndGrowth",

	"c30",
	"r1000v",
	"lcv",
	"sp500",
	"bev",
	"c20",
	"rmid",
	"r1000",
	"r1000g"
};

struct SHistoryItem
{
	COleDateTime	date;
	double			data[NUM_COLUMNS_IN_HISTORY_DB+3];

	// Access To Items By Name
	void SetItem(const char* name, double val)
	{
		for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+3; i++)
			if(strcmp(name, HistCols[i])==0)
				data[i] = val;
	}
	
	double GetItem(const char* name)
	{
		for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB+3; i++)
			if(strcmp(name, HistCols[i])==0)
				return data[i];
		return 0;
	}
	
	// Direct Access To Items
	void		SetReturn(double val)				{   data[0] = val;				}
	double		GetReturn()							{	return data[0];				}

	void		SetPrice(double val)				{   data[1] = val;				}
	double		GetPrice()							{	return data[1];				}

	void		SetRankNEPV(double val)				{   data[2] = val;				}
	double		GetRankNEPV()						{	return data[2];				}

	void		SetRankREPV(double val)				{   data[3] = val;				}
	double		GetRankREPV()						{	return data[3];				}

	void		SetRankNRAV(double val)				{   data[4] = val;				}
	double		GetRankNRAV()						{	return data[4];				}

	void		SetRankBEV(double val)				{   data[5] = val;				}
	double		GetRankBEV()						{	return data[5];				}

	void		SetRankPMV(double val)				{   data[6] = val;				}
	double		GetRankPMV()						{	return data[6];				}

	void		SetNEPV(double val)					{   data[7] = val;				}
	double		GetNEPV()							{	return data[7];				}

	void		SetCYFN(double val)					{   data[8] = val;				}
	double		GetCYFN()							{	return data[8];				}

	void		SetMomentN(double val)				{   data[9] = val;				}
	double		GetMomentN()						{	return data[9];				}

	void		SetRankMomentN(double val)			{   data[10] = val;				}
	double		GetRankMomentN()					{	return data[10];			}

	void		SetREPV(double val)					{   data[11] = val;				}
	double		GetREPV()							{	return data[11];			}

	void		SetCYFR(double val)					{   data[12] = val;				}
	double		GetCYFR()							{	return data[12];			}

	void		SetMomentR(double val)				{   data[13] = val;				}
	double		GetMomentR()						{	return data[13];			}

	void		SetRankMomentR(double val)			{   data[14] = val;				}
	double		GetRankMomentR()					{	return data[14];			}

	void		SetMO1(double val)					{   data[15] = val;				}
	double		GetMO1()							{	return data[15];			}

	void		SetRankMO1(double val)				{   data[16] = val;				}
	double		GetRankMO1()						{	return data[16];			}

	void		SetMO2(double val)					{   data[17] = val;				}
	double		GetMO2()							{	return data[17];			}

	void		SetRankMO2(double val)				{   data[18] = val;				}
	double		GetRankMO2()						{	return data[18];			}

	void		SetMultiple(double val)				{   data[19] = val;				}
	double		GetMultiple()						{	return data[19];			}

	void		SetROI(double val)					{   data[20] = val;				}
	double		GetROI()							{	return data[20];			}

	void		SetGrowth(double val)				{   data[21] = val;				}
	double		GetGrowth()							{	return data[21];			}

	void		SetRisk(double val)					{   data[22] = val;				}   
	double		GetRisk()							{	return data[22];			}

	void		SetYield(double val)				{   data[23] = val;				}
	double		GetYield()							{	return data[23];			}

	void		SetPE(double val)					{   data[24] = val;				}
	double		GetPE()								{	return data[24];			}

	void		SetEVEBITDA(double val)				{   data[25] = val;				}
	double		GetEVEBITDA()						{	return data[25];			}

	void		SetPayout(double val)				{   data[26] = val;				}
	double		GetPayout()							{	return data[26];			}

	void		SetNEPS(double val)					{   data[27] = val;				}
	double		GetNEPS()							{	return data[27];			}
};

//////////////////////////////////////////////////////////////////////////////////////
struct SHistory
{
	char									sTicker[8];
	int										arLoaded[NUM_COLUMNS_IN_HISTORY_DB+3];
	CArray<SHistoryItem, SHistoryItem>		arItems;
};

//////////////////////////////////////////////////////////////////////////////////////
struct SInterestRate
{
	COleDateTime	date;
	double			fWeeklyAve;
	double			f3MAve;
	double			fFridayClose;
};
typedef CArray<SInterestRate, SInterestRate>	SInterestRateHistory;

//////////////////////////////////////////////////////////////////////////////////////
struct SUser
{
	
};
typedef CArray<SInterestRate, SInterestRate>	SInterestRateHistory;

#endif // !defined(AFX_INPUT_H__4C0F1AFC_56C6_4BED_BA7D_A1854A307B4C__INCLUDED_)