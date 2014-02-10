// Company.h: interface for the CCompany class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPANY_H__6944C89E_CE5E_452F_B406_E747ECC4F97B__INCLUDED_)
#define AFX_COMPANY_H__6944C89E_CE5E_452F_B406_E747ECC4F97B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "input.h"

#define _CIP_COMPANY_VERSION_			0x00011

#define NULL_VAL						0xffffffff-1
#define ROCEPS_HYTORY_SIZE				50
#define YEARS_TO_CALC_RISK_ADJUST		8

///////////////////////////////////////////////////////////////////////////////
struct CTableYearsForMeansItem
{
public:
	double			m_fRiskAdjustmentLevel;
	unsigned int	m_nYearsForROC;
	unsigned int	m_nYearsForEPS;
};
typedef CArray<CTableYearsForMeansItem, CTableYearsForMeansItem>  CTableYearsForMeans;

///////////////////////////////////////////////////////////////////////////////
struct SChangeCriteria
{
	SChangeCriteria()
		{ nPeriods=1; nDeltaRankN=nDeltaRankR=nDeltaBEV=100; fDeltaPrice=fDeltaEPSNext=fDeltaEPSThis=fDeltaDebt=0.20; }

	int		nPeriods;

	int		nDeltaRankN;
	int		nDeltaRankR;
	int		nDeltaBEV;
	double	fDeltaPrice;
	double	fDeltaEPSNext;
	double	fDeltaEPSThis;
	double	fDeltaDebt;
};

///////////////////////////////////////////////////////////////////////////////
class CHistory;
class CUniverse;
class CMacroParameters;
class _declspec(dllexport) CCompany
{
public:
	CCompany();
	~CCompany();

public:
	// Load and Save
	BOOL Load(const char* szFilePath, BOOL bReCalcUniverse=false);
	BOOL Save(const char* szFilePath);

	BOOL Load(CArchive &ar);
	BOOL Save(CArchive &ar);

// direct access to member variables
	void SetYear(unsigned int nValue)				{ m_nYear = nValue;				}
	unsigned int GetYear()							{ return m_nYear;				}

	void SetUniverse(CUniverse* pUniverse)			{ m_pUniverse = pUniverse;		}
	CUniverse* GetUniverse()						{ return m_pUniverse;			}

	unsigned int GetYearsForROC()					{ return m_nYearsForROC;		}
	unsigned int GetYearsForEPS()					{ return m_nYearsForEPS;		}

	double GetRiskAdjustment()						{ return m_fRiskAdjustment;		}
	
	void SetMeanROC(double value)					{ m_fMeanROC = value;			}

	double GetMeanROC()								{ return m_fMeanROC;			}
	double GetMeanROE()								{ return m_fMeanROE;			}
	double GetMeanEPS()								{ return m_fMeanEPS;			}
	double GetMeanROC10Years()						{ return m_f10YROC;				}
	
	double GetMeanROI()								{ return m_fMeanROI;			}
	double GetMeanROIFore()							{ return m_fMeanROIFore;		}
	double GetMeanROIRazor()						{ return m_fMeanROIRazor;		}
	double GetMeanROIRazorFore()					{ return m_fMeanROIRazorFore;	}
	
	double GetMeanROCFore()							{ return m_fMeanROCFore;		}
	double GetMeanROEFore()							{ return m_fMeanROEFore;		}
	double GetMeanEPSFore()							{ return m_fMeanEPSFore;		}
	
	double GetMeanROCRazor()						{ return m_fMeanROCRazor;		}
	double GetMeanROERazor()						{ return m_fMeanROERazor;		}
	double GetMeanEPSRazor()						{ return m_fMeanEPSRazor;		}

	double GetMeanROCRazorFore()					{ return m_fMeanROCRazorFore;	}
	double GetMeanROERazorFore()					{ return m_fMeanROERazorFore;	}
	double GetMeanEPSRazorFore()					{ return m_fMeanEPSRazorFore;	}

	unsigned int GetPhaseA()						{ return m_nPhaseA;				}
	unsigned int GetPhaseE()						{ return m_nPhaseE;				}
	double GetMagnitudeA()							{ return m_fMagnitudeA;			}
	double GetMagnitudeE()							{ return m_fMagnitudeE;			}
	double GetTotalDebt()							{ return m_fTotalDebt;			}
	double GetNetWorth()							{ return m_fNetWorth;			}
	double GetPreferred()							{ return m_fPreferred;			}
	double GetDebtRatio()							{ return m_fDebtRatio;			}
	double GetOperFixChargeCover()					{ return m_fOperFixChargeCover;	}
	double GetInterestCover()						{ return m_fInterestCover;		}
	double GetYield()								{ return m_fYield;				}
	double GetHistoricalGrowth()					{ return m_fHistoricalGrowth;	}
	double GetIndicatedGrowth()						{ return m_fIndicatedGrowth;	}
	
	double GetMultiple()							{ return m_fMultiple;			}
	double GetMultipleFore()						{ return m_fMultipleFore;		}
	double GetMultipleRazor()						{ return m_fMultipleRazor;		}
	double GetMultipleRazorFore()					{ return m_fMultipleRazorFore;	}
	
	double GetReinvestmentRate()					{ return m_fReinvestRate;		}
	double GetReinvestmentRateFore()				{ return m_fReinvestRateFore;	}
	double GetReinvestmentRateRazor()				{ return m_fReinvestRateRazor;	}
	
	double GetMaxReinvestmentRate()					{ return m_fMaxReinvestRate;	}
	double GetMaxReinvestmentRateFore()				{ return m_fMaxReinvestRateFore;}
	double GetMaxReinvestmentRateRazor()			{ return m_fMaxReinvestRateRazor;}
	
	double GetPE()									{ return m_fPE;					}
	double GetEVOverEBITDA()						{ return m_fEV_EBITDA;			}
	
	double GetAdjustmentForPayout()					{ return m_fAdjustForPayout;	}
	double GetAdjustmentForPayoutFore()				{ return m_fAdjustForPayoutFore;}
	double GetAdjustmentForPayoutRazor()			{ return m_fAdjustForPayoutRazor;}
	
	double GetGrowthRate()							{ return m_fGrowthRate;			}
	double GetGrowthRateFore()						{ return m_fGrowthRateFore;		}
	double GetGrowthRateRazor()						{ return m_fGrowthRateRazor;	}
	
	double GetAdjustedGrowthRate()					{ return m_fAdjustedGrowthRate;	}
	double GetAdjustedGrowthRateFore()				{ return m_fAdjustedGrowthRateFore;	}
	double GetAdjustedGrowthRateRazor()				{ return m_fAdjustedGrowthRateRazor;}
	double GetAdjustedGrowthRateRazorFore()			{ return m_fAdjustedGrowthRateRazorFore;}
	
	double GetAdjustedAssetsOverShare()				{ return m_fAdjustedAssets_Share; }
	double GetGrowthEstimate1()						{ return m_fGrowthEstimate1;	}
	double GetGrowthEstimate2()						{ return m_fGrowthEstimate2;	}
	double GetNetCashFlowOverShare()				{ return m_fNetCashFlow_Share;	}
	double GetDeprAndAmortOverShare()				{ return m_fDeprAmort_Share;	}
	double GetBreakEvenValue()						{ return m_fBEV;				}
	double GetPriceOverBEV()						{ return m_fPrice_BEV;			}
	double GetBookEPS()								{ return m_fBookEPS;			}
	double GetNormEPS()								{ return m_fNormEPS;			}
	
	double GetEPV()									{ return m_fEPV;				}
	double GetRAV()									{ return m_fRAV;				}
	double GetPMV()									{ return m_fPMV;				}
	double GetPriceOverEPV()						{ return m_fPrice_EPV;			}
	double GetPriceOverRAV()						{ return m_fPrice_RAV;			}
	double GetPriceOverPMV()						{ return m_fPrice_PMV;			}
	
	double GetEPVFore()								{ return m_fEPVFore;			}
	double GetRAVFore()								{ return m_fRAVFore;			}
	double GetPMVFore()								{ return m_fPMVFore;			}
	double GetPriceOverEPVFore()					{ return m_fPrice_EPVFore;		}
	double GetPriceOverRAVFore()					{ return m_fPrice_RAVFore;		}
	double GetPriceOverPMVFore()					{ return m_fPrice_PMVFore;		}
	
	double GetEPVRazor()							{ return m_fEPVRazor;			}
	double GetRAVRazor()							{ return m_fRAVRazor;			}
	double GetPMVRazor()							{ return m_fPMVRazor;			}
	double GetPriceOverEPVRazor()					{ return m_fPrice_EPVRazor;		}
	double GetPriceOverRAVRazor()					{ return m_fPrice_RAVRazor;		}
	double GetPriceOverPMVRazor()					{ return m_fPrice_PMVRazor;		}
	
	double GetEPVRazorFore()						{ return m_fEPVRazorFore;			}
	double GetRAVRazorFore()						{ return m_fRAVRazorFore;			}
	double GetPMVRazorFore()						{ return m_fPMVRazorFore;			}
	double GetPriceOverEPVRazorFore()				{ return m_fPrice_EPVRazorFore;	}
	double GetPriceOverRAVRazorFore()				{ return m_fPrice_RAVRazorFore;	}
	double GetPriceOverPMVRazorFore()				{ return m_fPrice_PMVRazorFore;	}

	double GetMomentumActual()						{ return m_fMomentumActual;		}
	double GetMomentumRazor()						{ return m_fMomentumRazor;		}

	BOOL Split(double fRatio, BOOL bOnlyHistory=true);

	CMacroParameters*	GetMacroParametersPointer();
	CMacroParameters	GetMacroParameters();
	void SetMacroParameters(CMacroParameters &Macro);

	BOOL LoadHistory(COleDateTime from, COleDateTime to);
	BOOL LoadHistory(int nNumRecords);
	BOOL PreLoadHistory();
	void ClearHistory();
	CHistory* GetHistory()							{ return m_pHistory;			}

	void SetTicker(const char* value)				{ m_sTicker = value;			}
	CString GetTicker()								{ return m_sTicker;				}

	void SetName(const char* value)					{ m_sName = value;				}
	CString GetName()								{ return m_sName;				}

	void SetIndustry(const char* value)				{ m_sIndustry = value;			}
	CString GetIndustry()							{ return m_sIndustry;			}

	void SetSector(const char* value)				{ m_sSector = value;			}
	CString GetSector()								{ return m_sSector;				}

	void SetCIQTicker(const char* value)			{ m_sCIQTicker = value;			}
	CString GetCIQTicker()							{ return m_sCIQTicker;			}

	void SetInSP500(BOOL val)						{ m_bInSP500 = val;				}
	BOOL GetInSP500()								{ return m_bInSP500;			}

	void SetRecentRollOver(BOOL val)				{ m_bRecentRollOver = val;		}
	BOOL GetRecentRollOver()						{ return m_bRecentRollOver;		}

	void SetType(unsigned int value)				{ m_nType = value;				}
	unsigned int GetType()							{ return m_nType;				}

	void SetReportDateForFundamentals(COleDateTime& value)		{ m_dtFundamentals = value;		}
	COleDateTime GetReportDateForFundamentals()					{ return m_dtFundamentals;		}

	void SetReportDateForPriceAndReturn(COleDateTime& value)	{ m_dtPrice = value;			}
	COleDateTime GetReportDateForPriceAndReturn()				{ return m_dtPrice;				}

	void SetLoadBS(BOOL value)						{ m_bLoadBS = value;			}
	BOOL GetLoadBS()								{ return m_bLoadBS;				}

	void SetLoadEPS(BOOL value)						{ m_bLoadEPS = value;			}
	BOOL GetLoadEPS()								{ return m_bLoadEPS;			}

	void SetLoadROCROE(BOOL value)					{ m_bLoadROCROE = value;		}
	BOOL GetLoadROCROE()							{ return m_bLoadROCROE;			}

	void	SetROCHistory(double* values, unsigned int nSize); 
	void	SetROCHistory(CArray<double, double>& arValues);
	unsigned int GetROCHistory(double* values);
	unsigned int GetROCHistory(CArray<double, double>& arValues);

	void	SetROEHistory(double* values, unsigned int nSize);
	void	SetROEHistory(CArray<double, double>& arValues);
	unsigned int GetROEHistory(double* values);
	unsigned int GetROEHistory(CArray<double, double>& arValues);

	void	SetEPSHistory(double* values, unsigned int nSize);
	void	SetEPSHistory(CArray<double, double>& arValues);
	unsigned int GetEPSHistory(double* values);
	unsigned int GetEPSHistory(CArray<double, double>& arValues);
	unsigned int GetSharesHistory(double* values);
	unsigned int GetSharesHistory(CArray<double, double>& arValues);

	void SetPeriodsForPhaseCalculation(unsigned int nPoint1, unsigned int nPoint2)	{ m_nPhasePoint1 = nPoint1; m_nPhasePoint2 = nPoint2;	}

	void			SetYearsForMeansTable(double* pRiskLevels, unsigned int* pYearsROC, unsigned int* pYearsEPS, unsigned int nSize);
	unsigned int	GetYearsForMeansTable(double* pRiskLevels, unsigned int* pYearsROC, unsigned int* pYearsEPS);
	void			GetYearsForMeans(double fRiskAdjustment, unsigned int &nYearsROC, unsigned int &nYearsEPS);

	void SetEPS(int year, double value)				{ m_arfEPS[year] = value;		}
	double GetEPS(int year)							{ return m_arfEPS[year];		}

	void SetROC(int year, double value)				{ m_arfROC[year] = value;		}
	double GetROC(int year)							{ return m_arfROC[year];		}

	void SetROE(int year, double value)				{ m_arfROE[year] = value;		}
	double GetROE(int year)							{ return m_arfROE[year];		}

	void SetUseDebtAdjust(BOOL value)				{ m_bUseDebtAdjust = value;		}
	BOOL GetUseDebtAdjust()							{ return m_bUseDebtAdjust;		}

	void SetEPSAdjustMultiply(double value)			{ m_fEPSAdjustMultiply = value;	}
	double GetEPSAdjustMultiply()					{ return m_fEPSAdjustMultiply;	}

	void SetEPSAdjustAdd(double value)				{ m_fEPSAdjustAdd = value;		}
	double GetEPSAdjustAdd()						{ return m_fEPSAdjustAdd;		}

	void SetPrice(double value)						{ m_fPrice = value;				}
	double GetPrice()								{ return m_fPrice;				}

	void SetReturn(double value)					{ m_fReturn = value;			}
	double GetReturn()								{ return m_fReturn;				}

	void SetAssets(double value)					{ m_fAssets = value;			}
	double GetAssets()								{ return m_fAssets;				}

	void SetNetPlant(double value)					{ m_fNetPlant = value;			}
	double GetNetPlant()							{ return m_fNetPlant;			}

	void SetIntangibles(double value)				{ m_fIntangibles = value;		}
	double GetIntangibles()							{ return m_fIntangibles;		}

	void SetCash(double value)						{ m_fCash = value;				}
	double GetCash()								{ return m_fCash;				}

	void SetLiabilities(double value)				{ m_fLiabilities = value;		}
	double GetLiabilities()							{ return m_fLiabilities;		}

	void SetCommonEquity(double value)				{ m_fCommonEquity = value;		}
	double GetCommonEquity()						{ return m_fCommonEquity;		}

	void SetShortTermDebt(double value)				{ m_fShortTermDebt = value;		}
	double GetShortTermDebt()						{ return m_fShortTermDebt;		}

	void SetLongTermDebt(double value)				{ m_fLongTermDebt = value;		}
	double GetLongTermDebt()						{ return m_fLongTermDebt;		}

	void SetPreTaxIncome(double value)				{ m_fPreTaxIncome = value;		}
	double GetPreTaxIncome()						{ return m_fPreTaxIncome;		}

	void SetInterestExpense(double value)			{ m_fInterestExpense = value;	}
	double GetInterestExpense()						{ return m_fInterestExpense;	}

	void SetDeprAndAmort(double value)				{ m_fDeprAndAmort = value;		}
	double GetDeprAndAmort()						{ return m_fDeprAndAmort;		}

	void SetSharesOut(double value)					{ m_fSharesOut = value;			}
	double GetSharesOut()							{ return m_fSharesOut;			}

	void SetDividend(double value)					{ m_fDividend = value;			}
	double GetDividend()							{ return m_fDividend;			}

	void SetEPSForecastLastYear(double value)		{ m_fEPSForecastPrevY = value;	}
	double GetEPSForecastLastYear()					{ return m_fEPSForecastPrevY;	}

	void SetEPSForecastThisYear(double value)		{ m_fEPSForecastThisY = value;	}
	double GetEPSForecastThisYear()					{ return m_fEPSForecastThisY;	}

	void SetEPSForecastNextYear(double value)		{ m_fEPSForecastNextY = value;	}
	double GetEPSForecastNextYear()					{ return m_fEPSForecastNextY;	}

	void SetEPSForecast(double thisY, double nextY)	{ m_fEPSForecastThisY = thisY; m_fEPSForecastNextY = nextY; }
	void GetEPSForecast(double &thisY, double& nextY) { thisY = m_fEPSForecastThisY; nextY = m_fEPSForecastNextY; }

	void SetAssetLife(double value)					{ m_fAssetLife = value;			}
	double GetAssetLife()							{ return m_fAssetLife;			}

	void SetBookValue(double value)					{ m_fBookValue = value;			}
	double GetBookValue()							{ return m_fBookValue;			}

	void SetDilution(double value)					{ m_fDilution = value;			}
	double GetDilution()							{ return m_fDilution;			}

	void SetSplitRatio(const char* value)			{ m_sSplitRatio = value;		}
	CString GetSplitRatio()							{ return m_sSplitRatio;			}

	void SetLastSplitDate(COleDateTime& value)		{ m_dtSplit = value;			}
	COleDateTime GetLastSplitDate()					{ return m_dtSplit;				}

	void SetRankEPV(int value)						{ m_nRankEPV = value;			}
	int GetRankEPV()								{ return m_nRankEPV;			}

	void SetRankRAV(int value)						{ m_nRankRAV = value;			}
	int GetRankRAV()								{ return m_nRankRAV;			}

	void SetRankPMV(int value)						{ m_nRankPMV = value;			}
	int GetRankPMV()								{ return m_nRankPMV;			}

	void SetRankEPVFore(int value)					{ m_nRankEPVFore = value;		}
	int GetRankEPVFore()							{ return m_nRankEPVFore;		}

	void SetRankRAVFore(int value)					{ m_nRankRAVFore = value;		}
	int GetRankRAVFore()							{ return m_nRankRAVFore;		}

	void SetRankPMVFore(int value)					{ m_nRankPMVFore = value;		}
	int GetRankPMVFore()							{ return m_nRankPMVFore;		}

	void SetRankEPVRazor(int value)					{ m_nRankEPVRazor = value;		}
	int GetRankEPVRazor()							{ return m_nRankEPVRazor;		}

	void SetRankRAVRazor(int value)					{ m_nRankRAVRazor = value;		}
	int GetRankRAVRazor()							{ return m_nRankRAVRazor;		}

	void SetRankPMVRazor(int value)					{ m_nRankPMVRazor = value;		}
	int GetRankPMVRazor()							{ return m_nRankPMVRazor;		}

	void SetRankEPVRazorFore(int value)				{ m_nRankEPVRazorFore = value;	}
	int GetRankEPVRazorFore()						{ return m_nRankEPVRazorFore;	}

	void SetRankRAVRazorFore(int value)				{ m_nRankRAVRazorFore = value;	}
	int GetRankRAVRazorFore()						{ return m_nRankRAVRazorFore;	}

	void SetRankPMVRazorFore(int value)				{ m_nRankPMVRazorFore = value;	}
	int GetRankPMVRazorFore()						{ return m_nRankPMVRazorFore;	}

	void SetRankBEV(int value)						{ m_nRankBEV = value;			}
	int GetRankBEV()								{ return m_nRankBEV;			}

	void SetRankGrowth1(int value)					{ m_nRankGrowth1 = value;		}
	int GetRankGrowth1()							{ return m_nRankGrowth1;		}

	void SetRankGrowth2(int value)					{ m_nRankGrowth2 = value;		}
	int GetRankGrowth2()							{ return m_nRankGrowth2;		}

	void SetRankMomentumActual(int value)			{ m_nRankMomentumActual = value;}
	int GetRankMomentumActual()						{ return m_nRankMomentumActual;	}

	void SetRankMomentumRazor(int value)			{ m_nRankMomentumRazor = value;	}
	int GetRankMomentumRazor()						{ return m_nRankMomentumRazor;	}

	void SetWeightsInIndices(double* values)		{ memcpy(m_arWeightsInIndices, values, NUM_WEIGHTS*sizeof(double));		}
	void GetWeightsInIndices(double* values)		{ memcpy(values, m_arWeightsInIndices, NUM_WEIGHTS*sizeof(double));		}
	
	void SetWeightInIndex(int nIndex, double val)	{ m_arWeightsInIndices[nIndex] = val;	}
	double GetWeightInIndex(int nIndex)				{ return m_arWeightsInIndices[nIndex];	}

	void SetWeightInIndex(const char* szIndex, double val);
	double GetWeightInIndex(const char* szIndex);

public:
	BOOL CreateFromRawData(SCompanyRawData& data);
	BOOL GetRawData(SCompanyRawData& data);
	
	void StoreRanks()								{ m_nRankEPVOrigin = m_nRankEPV;				}
	int GetEPVRankOriginal()						{ return m_nRankEPVOrigin;						}
	int GetEPVRankDiff()							{ return m_nRankEPV - m_nRankEPVOrigin;			}

	void CalcAll();	
	void DoSensitivityAnalysis(double fRoc, double fRoe, double fEps, double fDebtRatio, double fPrice,
					double fDiv, double fEpsNext, double fEpsThis, double fEpsLast);

	BOOL LoadLocalCopyFromAppData();
	BOOL SaveLocalCopyInAppData();
	
	void operator = (CCompany& src);
	void CopyBalanceSheetFrom(CCompany& src);
	void CopyEPSFrom(CCompany& src);

	void GetHistoryItem(SHistoryItem& item);

	double GetSplitRatioDouble();
	BOOL CheckForRecentSplit(const COleDateTime dtFrom) { return (m_dtSplit.GetStatus() == COleDateTime::valid && m_dtSplit >= dtFrom); }
	BOOL CheckForChanges(SChangeCriteria& ch);

protected:
// Internal calculations
	void CalcRiskAdjustment();			// make sure history data is set when calling this function
	void CalcYearsForMeanROCandEPS();	// make sure that risk adjustment is calculated when calling this function
	void CalcMeanROC();
	void CalcMeanROE();
	void CalcMeanEPS();
	void CalcMeanROCFor10Years();
	void CalcPhasesAndMagnitudes();
	void CalcDebtAndEquity();			
	void CalcDebtRatio();
	void CalcMeanROI();
	void CalcRatios();
	void CalcHistoricalGrowth();

protected:
	CUniverse*		m_pUniverse;		// back pointer to stock Universe
	CHistory*		m_pHistory;			// Pointer to history data

// Inputs to the Model
	CString			m_sTicker;			// Stock ticker
	CString			m_sName;			// Company name
	CString			m_sIndustry;		// Industry name
	CString			m_sSector;			// Sector name

	CString			m_sCIQTicker;		// Stock ticker in Capital IQ
	BOOL			m_bInSP500;			// Is the stock in SP500 index
	BOOL			m_bRecentRollOver;	// Was there RollOver recently

	unsigned int	m_nType;			// Regular, Bank or Utility

	unsigned int	m_nYear;			// the current fiscal year (all calculations are for [CurrY-9, ..., CurrY+1]. The last one is next year
	COleDateTime	m_dtFundamentals;	// The last reported date for Company Fundamentals
	COleDateTime	m_dtPrice;			// The last reported date for Price

	BOOL			m_bLoadBS;			// Auto Load BS Data
	BOOL			m_bLoadEPS;			// Auto Load EPS Forecasts
	BOOL			m_bLoadROCROE;		// Auto Load ROC ROE and EPS Historical Values

	double			m_arfROCOriginal[ROCEPS_HYTORY_SIZE];	// return on capital = (NetIncome + Interest Expense) / (Dept + Equity) - Not Adjusted
	double			m_arfROEOriginal[ROCEPS_HYTORY_SIZE];	// return on equity  = Net Income / Equity, some exceptions from Baseline - NotAdjusted 
	double			m_arfEPSOriginal[ROCEPS_HYTORY_SIZE];	// not GAAP EPS, but Baseline EPS - Not Adjusted
	double			m_arfSharesOriginal[ROCEPS_HYTORY_SIZE];// Shares Outstanding

	double			m_arfROC[ROCEPS_HYTORY_SIZE];			// return on capital = (NetIncome + Interest Expense) / (Dept + Equity)
	double			m_arfROE[ROCEPS_HYTORY_SIZE];			// return on equity  = Net Income / Equity, some exceptions from Baseline
	double			m_arfEPS[ROCEPS_HYTORY_SIZE];			// not GAAP EPS, but Baseline EPS - Adjusted for Banks and Utilities
	double			m_arfShares[ROCEPS_HYTORY_SIZE];		// Shares Outstanding

	BOOL			m_bUseDebtAdjust;						// Use Debt Adjustment while calculating ROI
	double			m_fEPSAdjustMultiply;					// EPS Multiply adjustment (used for utility companies)
	double			m_fEPSAdjustAdd;						// EPS Add adjustment (used for banking companies)

	double			m_fPrice;								// Stock Price from d_prc.xls
	double			m_fReturn;								// Return from d_ret.xls (Actually not used in any calculation, but keep just in case)

	double			m_fAssets;
	double			m_fLiabilities;
	double			m_fCommonEquity;		
	double			m_fNetPlant;
	double			m_fIntangibles;
	double			m_fCash;
	double			m_fShortTermDebt;
	double			m_fLongTermDebt;
	double			m_fPreTaxIncome;
	double			m_fInterestExpense;
	double			m_fDeprAndAmort;
	double			m_fSharesOut;
	double			m_fDividend;
	double			m_fEPSForecastPrevY;
	double			m_fEPSForecastThisY;
	double			m_fEPSForecastNextY;
	double			m_fBookValue;
	double			m_fDilution;

	CString			m_sSplitRatio;
	COleDateTime	m_dtSplit;

	double			m_arWeightsInIndices[NUM_WEIGHTS];

	// table to calculate Years For Means
	CTableYearsForMeans		m_TableYearsForMeans;

	// Number of years to calculate stock phase and magnitude
	unsigned int	m_nPhasePoint1;  // default 10
	unsigned int	m_nPhasePoint2;  // default 5

// variables calculated internally
	double			m_fRiskAdjustment;				// Risk Adjustment factor (1-Coeff Variation)
	unsigned int	m_nYearsForROC;					// number of years to calculate mean ROC and ROE
	unsigned int	m_nYearsForEPS;					// number of years to calculate mean EPS
													
	double			m_fMeanROC;						// Mean ROC for the years from m_nYear back to m_nYear - m_nYearsForROC
	double			m_fMeanROE;						// Mean ROE for the years from m_nYear back to m_nYear - m_nYearsForROC
	double			m_fMeanEPS;						// Mean EPS for the years from m_nYear back to m_nYear - m_nYearsForEPS
													
	double			m_f10YROC;						// 10 years mean ROC
													
	double			m_fMeanROCFore;					// Mean ROC including forecasted next year, shifted one to the right
	double			m_fMeanROEFore;					// Mean ROE including forecasted next year
	double			m_fMeanEPSFore;					// Mean EPS including forecasted next year
													
	double			m_fMeanROCRazor;				// Mean ROC Razor
	double			m_fMeanROERazor;				// Mean ROE Razor
	double			m_fMeanEPSRazor;				// Mean EPS Razor
													
	double			m_fMeanROCRazorFore;			// Mean ROC Razor Forecast
	double			m_fMeanROERazorFore;			// Mean ROE Razor Forecast
	double			m_fMeanEPSRazorFore;			// Mean EPS Razor Forecast
													
	double			m_fMeanROI;						// Mean Return on Investment. Calculated from ROC and ROE given the debt ratio
	double			m_fMeanROIFore;					// Mean Return on Investment including forecasted next year
	double			m_fMeanROIRazor;				// Mean Return on Investment Razor
	double			m_fMeanROIRazorFore;			// Mean Return on Investment Razor Forecast
													
	unsigned int	m_nPhaseA;						// The phase the company is in, going backward from CURRENT year
	unsigned int	m_nPhaseE;						// The phase the company is in, going backward from FORECAST year
	double			m_fMagnitudeA;					// The phase magnitude, going backward from CURRENT year
	double			m_fMagnitudeE;	 				// The phase magnitude, going backward from FORECAST year
													
	double			m_fTotalDebt;					// Total Debt = ShortTerm Debt + LongTerm Debt
	double			m_fNetWorth;					// All Equity (Assets - Liabilities)
	double			m_fPreferred;					// NetWorth - Common Equity
	double			m_fDebtRatio;					// (TotalDebt - Cash) / Common Equity
	double			m_fOperFixChargeCover;			// 1 + (PreTaxIncome+Depr&Amort)/Interest
	double			m_fInterestCover;				// 1 + (PreTaxIncome/Interest)
	double			m_fYield;						// Dividend / Price
	double			m_fReinvestRate;				// 1 - Dividend/EPS
	double			m_fReinvestRateFore;			// The same for Forecasted EPS
	double			m_fReinvestRateRazor;			// The same for Razor
	double			m_fReinvestRateRazorFore;		// The same for Razor
	double			m_fMaxReinvestRate;				// Blend of ReinvestRate and MaxReinvestRate from Macro Parameters
	double			m_fMaxReinvestRateFore;			// Blend of ReinvestRate and MaxReinvestRate from Macro Parameters
	double			m_fMaxReinvestRateRazor;		// Blend of ReinvestRate and MaxReinvestRate from Macro Parameters
	double			m_fMaxReinvestRateRazorFore;	// Blend of ReinvestRate and MaxReinvestRate from Macro Parameters
	double			m_fPE;							// P/E ratio = Price / Trailing 12 Month EPS
	double			m_fEV_EBITDA;					// EV/EDITDA ratio = Enterprise Value / Earnings Before Interest, Taxes, Depr & Amort
	double			m_fAdjustForPayout;				// Adjustment for Payout
	double			m_fAdjustForPayoutFore;			// Adjustment for Payout including Forectasted next year
	double			m_fAdjustForPayoutRazor;		// Adjustment for Payout for Razor
	double			m_fAdjustForPayoutRazorFore;	// Adjustment for Payout for Razor Forecast
	double			m_fGrowthRate;					// GrowthRate = ROI * ReinvestmentRate
	double			m_fGrowthRateFore;				// GrowthRate = ROI * ReinvestmentRate
	double			m_fGrowthRateRazor;				// GrowthRate = ROI * ReinvestmentRate
	double			m_fGrowthRateRazorFore;			// GrowthRate = ROI * ReinvestmentRate
	double			m_fAdjustedGrowthRate;			// Adjusted Growth rate based on minimum of GR, MO1, MO2 (see the implementation for details)
	double			m_fAdjustedGrowthRateFore;		// Adjusted Growth rate including forecasted next year 
	double			m_fAdjustedGrowthRateRazor;		// Adjusted Growth rate razor
	double			m_fAdjustedGrowthRateRazorFore;	// Adjusted Growth rate razor forecast
	double			m_fGrowthEstimate1;				// MO1 - Estimated EPS growth for 1 year = EPS_NextY/EPS_ThisY - 1
	double			m_fGrowthEstimate2;				// MO2 - Estimated EPS growth for 2 years = EPS_NextY/EPS_LastY - 1
	int				m_nRankGrowth1;					// One year growth rank
	int				m_nRankGrowth2;					// Two years growth rank
	double			m_fAssetLife;					// Net Plant / Depr & Amort
	double			m_fAdjustedAssets_Share;		// ((Assets-NetPlant-Intangibles-Liabilities)/AssetLife)/Shares Out
	double			m_fNetCashFlow_Share;			// AdjustedAssets/Share + Depr&Amort/Share
	double			m_fDeprAmort_Share;				// Depr&Amort/Shares Out
	double			m_fBEV;							// PV of net cash flow per share over the calculated asset life at the current discount rate
	double			m_fPrice_BEV;					// Price / BEV
	int				m_nRankBEV;						// Price/BEV Rank in the universe
	double			m_fBookEPS;						// Book EPS value
	double			m_fNormEPS;						// Norm EPS = the average of BookEPS and MeanEPS

	double			m_fIndicatedGrowth;				// Indicated Growth
	double			m_fHistoricalGrowth;			// Historical Growth
													
	double			m_fMultiple;					// The cornerstone of Fair Value Model
	double			m_fMultipleFore;				// Shifted to the right for one year
	double			m_fMultipleRazor;				// For Razor
	double			m_fMultipleRazorFore;			// For Razor Forecast
													
	double			m_fEPV;							// Actual EPV
	double			m_fRAV;							// Actual RAV
	double			m_fPMV;							// Actual PMV
	double			m_fPrice_EPV;					// Price over EPV
	double			m_fPrice_RAV;					// Price over RAV
	double			m_fPrice_PMV;					// Price over PMV
	int				m_nRankEPV;						// Price/EPV rank index in the universe
	int				m_nRankRAV;						// Price/RAV rank index in the universe
	int				m_nRankPMV;						// Price/PMV rank index in the universe
													
	double			m_fEPVFore;						// Forecast EPV
	double			m_fRAVFore;						// Forecast RAV
	double			m_fPMVFore;						// Forecast PMV
	double			m_fPrice_EPVFore;				// Forecast Price over EPV
	double			m_fPrice_RAVFore;				// Forecast Price over RAV
	double			m_fPrice_PMVFore;				// Forecast Price over PMV
	int				m_nRankEPVFore;					// Forecast Price/EPV rank index in the universe
	int				m_nRankRAVFore;					// Forecast Price/RAV rank index in the universe
	int				m_nRankPMVFore;					// Forecast Price/PMV rank index in the universe
													
	double			m_fEPVRazor;					// Razor EPV
	double			m_fRAVRazor;					// Razor RAV
	double			m_fPMVRazor;					// Razor PMV
	double			m_fPrice_EPVRazor;				// Razor Price over EPV
	double			m_fPrice_RAVRazor;				// Razor Price over RAV
	double			m_fPrice_PMVRazor;				// Razor Price over PMV
	int				m_nRankEPVRazor;				// Razor Price/EPV rank index in the universe
	int				m_nRankRAVRazor;				// Razor Price/RAV rank index in the universe
	int				m_nRankPMVRazor;				// Razor Price/PMV rank index in the universe
													
	double			m_fEPVRazorFore;				// Razor Forecast EPV
	double			m_fRAVRazorFore;				// Razor Forecast RAV
	double			m_fPMVRazorFore;				// Razor Forecast PMV
	double			m_fPrice_EPVRazorFore;			// Razor Forecast Price over EPV
	double			m_fPrice_RAVRazorFore;			// Razor Forecast Price over RAV
	double			m_fPrice_PMVRazorFore;			// Razor Forecast Price over PMV
	int				m_nRankEPVRazorFore;			// Razor Forecast Price/EPV rank index in the universe
	int				m_nRankRAVRazorFore;			// Razor Forecast Price/RAV rank index in the universe
	int				m_nRankPMVRazorFore;			// Razor Forecast Price/PMV rank index in the universe

	double			m_fMomentumActual;				// Forecast EPV / Actual EPV
	int				m_nRankMomentumActual;			// Rank of Actual Momentum

	double			m_fMomentumRazor;				// Razor Fore EPV / Razor EPV 
	int				m_nRankMomentumRazor;			// Rank of Razor Momentum

	int				m_nRankEPVOrigin;				// Original Rank - used in Sensitivity to get the difference

};

#endif // !defined(AFX_COMPANY_H__6944C89E_CE5E_452F_B406_E747ECC4F97B__INCLUDED_)
