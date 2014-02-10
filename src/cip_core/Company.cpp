// Company.cpp: implementation of the CCompany class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "Company.h"
#include "Universe.h"
#include "History.h"
#include "SQLCompany.h"
#include "BaseMath.h"
#include "DataManager.h"

#include <math.h>
#include "shlobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_coreApp		theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompany::CCompany()
{
	// Set history values to NULL
	unsigned int i;
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		m_arfROC[i] = NULL_VAL;
	
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		m_arfROE[i] = NULL_VAL;
	
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		m_arfEPS[i] = NULL_VAL;

	// Use Debt Adjustment
	m_bUseDebtAdjust = true;

	// EPS Internal Adjustments
	m_fEPSAdjustMultiply = 1.0;
	m_fEPSAdjustAdd = 0.0;

	// Set default years_for_means table
	double			riskLevels[]= {0.5,	0.65,	0.8,	0.85,	1};
	unsigned int	yearsROC[]	= {7,	6,		5,		5,		5};
	unsigned int	yearsEPS[]	= {7,	6,		5,		4,		3};
	SetYearsForMeansTable(riskLevels, yearsROC, yearsEPS, 5);

	// default risk adjustment and years to calculate means
	m_fRiskAdjustment = 0.01;
	m_nYearsForROC = 7; 
	m_nYearsForEPS = 7;

	// default phase points
	m_nPhasePoint1 = 10;
	m_nPhasePoint2 = 5;

	// default dilution
	m_fDilution = 1.0;

	// default - download all from DB
	m_bLoadBS = true;
	m_bLoadEPS = true;

	m_nType = 0;
	m_pHistory = NULL;

	m_nYear = 0;
	memset(m_arWeightsInIndices, 0, sizeof(double)*NUM_WEIGHTS);
}

CCompany::~CCompany()
{
	ClearHistory();
}

///////////////////////////////////////////////////////////
BOOL CCompany::Save(CArchive &ar)
{
	// version number
	ar << (DWORD)_CIP_COMPANY_VERSION_;

	// general data
	ar << m_sTicker;
	ar << m_sName;
	ar << m_sIndustry;
	ar << m_sSector;

	ar << m_nYear;
	ar << m_dtFundamentals;
	ar << m_dtPrice;

	ar << m_nType;
	ar << m_bLoadBS;
	ar << m_bLoadEPS;

	// roc, roe and eps history
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
	{
		ar << m_arfROC[i];
		ar << m_arfROE[i];
		ar << m_arfEPS[i];
	}

	// data
	ar << m_bUseDebtAdjust;
	ar << m_fEPSAdjustMultiply;
	ar << m_fEPSAdjustAdd;
	 
	ar << m_fPrice;
	ar << m_fReturn; 

	// balance sheet 
	ar << m_fAssets;
	ar << m_fLiabilities;
	ar << m_fCommonEquity;	
	ar << m_fNetPlant;
	ar << m_fIntangibles;
	ar << m_fCash;
	ar << m_fShortTermDebt;
	ar << m_fLongTermDebt;
	ar << m_fPreTaxIncome;
	ar << m_fInterestExpense;
	ar << m_fDeprAndAmort;
	ar << m_fSharesOut;
	ar << m_fDividend;
	ar << m_fEPSForecastPrevY;
	ar << m_fEPSForecastThisY;
	ar << m_fEPSForecastNextY;
	ar << m_fBookValue;
	ar << m_fDilution;

	ar << m_sSplitRatio;
	ar << m_dtSplit;

	return true;
}

BOOL CCompany::Load(CArchive &ar)
{
	// version number
	DWORD dwVersion;
	ar >> dwVersion;
	if(dwVersion != _CIP_COMPANY_VERSION_)
	{
		TRACE("Wrong version of company data.\n");
		return false;
	}

	// general data
	ar >> m_sTicker;
	ar >> m_sName;
	ar >> m_sIndustry;
	ar >> m_sSector;

	ar >> m_nYear;
	ar >> m_dtFundamentals;
	ar >> m_dtPrice;

	ar >> m_nType;
	ar >> m_bLoadBS;
	ar >> m_bLoadEPS;

	// roc, roe and eps history
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
	{
		ar >> m_arfROC[i];
		ar >> m_arfROE[i];
		ar >> m_arfEPS[i];
	}

	// data
	ar >> m_bUseDebtAdjust;
	ar >> m_fEPSAdjustMultiply;
	ar >> m_fEPSAdjustAdd;
	
	ar >> m_fPrice;
	ar >> m_fReturn;

	// balance sheet
	ar >> m_fAssets;
	ar >> m_fLiabilities;
	ar >> m_fCommonEquity;	
	ar >> m_fNetPlant;
	ar >> m_fIntangibles;
	ar >> m_fCash;
	ar >> m_fShortTermDebt;
	ar >> m_fLongTermDebt;
	ar >> m_fPreTaxIncome;
	ar >> m_fInterestExpense;
	ar >> m_fDeprAndAmort;
	ar >> m_fSharesOut;
	ar >> m_fDividend;
	ar >> m_fEPSForecastPrevY;
	ar >> m_fEPSForecastThisY;
	ar >> m_fEPSForecastNextY;
	ar >> m_fBookValue;
	ar >> m_fDilution;

	ar >> m_sSplitRatio;
	ar >> m_dtSplit;

	return true;
}

BOOL CCompany::Load(const char* szFilePath, BOOL bReCalcUniverse)
{
	CFile file;
	if(!file.Open(szFilePath, CFile::modeRead))
	{
		CString sErr; sErr.Format("Unable to open file: '%s'.", szFilePath);
		//::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		return false;
	}

	CArchive ar(&file, CArchive::load);

	if(!Load(ar))
		return false;

	if(bReCalcUniverse && m_pUniverse)
		m_pUniverse->CalcAll();
	else
		CalcAll();

	return true;
}

BOOL CCompany::Save(const char* szFilePath)
{
	CFile file;
	if(!file.Open(szFilePath, CFile::modeCreate|CFile::modeWrite))
	{
		CString sErr; sErr.Format("Unable to create file: '%s'", szFilePath);
		//::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		return false;
	}

	CArchive ar(&file, CArchive::store);

	return Save(ar);
}

BOOL CCompany::Split(double fRatio, BOOL bOnlyHistory)
{
	const int modIndexes[16] = {1,7,8,11,12,27,28,29,33,34,35,36,39,40,41,42};
	if(!bOnlyHistory)
	{
		m_fSharesOut *= fRatio;
		m_fPrice /= fRatio;
		m_fEPSForecastNextY /= fRatio;
		m_fEPSForecastThisY /= fRatio;
		m_fEPSForecastPrevY /= fRatio;
		m_fDividend /= fRatio;
		m_fBookValue /= fRatio;

		for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
		{
			if(m_arfEPS[i] < _INF_)
				m_arfEPS[i] /= fRatio;

			if(m_arfEPSOriginal[i] < _INF_)
				m_arfEPSOriginal[i] /= fRatio;
		}
		m_fEPSAdjustAdd /= fRatio;
	}

	// Modify the history
	LoadHistory(-1);
	for(int i=0; i<m_pHistory->GetSize(); i++)
	{
		SHistoryItem *item = m_pHistory->GetItemPtr(i);

		for(int j=0; j<16; j++)
			if(item->data[modIndexes[j]] < _INF_)
				item->data[modIndexes[j]] /= fRatio;

		/*item->data[1] /= fRatio;	// m_fPrice
		item->data[7] /= fRatio;	// m_fEPV;
		item->data[8] /= fRatio;	// m_fEPVFore;
		item->data[11] /= fRatio;	// m_fEPVRazor;
		item->data[12] /= fRatio;	// m_fEPVRazorFore;
		item->data[27] /= fRatio;	// m_fNormEPS;
		item->data[28] /= fRatio;	// m_fBookEPS;
		item->data[29] /= fRatio;	// m_fBookValue;
		item->data[33] /= fRatio;	// m_fMeanEPS;
		item->data[34] /= fRatio;	// m_fAdjustedAssets_Share;
		item->data[35] /= fRatio;	// m_fNetCashFlow_Share;
		item->data[36] /= fRatio;	// m_fDeprAmort_Share;
		item->data[39] /= fRatio;	// m_fDividend;
		item->data[40] /= fRatio;	// m_fEPSForecastNextY;
		item->data[41] /= fRatio;	// m_fEPSForecastThisY;
		item->data[42] /= fRatio;	// m_fEPSForecastPrevY;*/
	}

	// Save into DB
	SCompanyRawData data;
	if(!GetRawData(data))
		return false;

	if(!CSQLCompany::UpdateCompany(data, true, true))
		return false;
	
	return CSQLCompany::UpdateHistory(*m_pHistory, NUM_COLUMNS_IN_HISTORY_DB);
}

/////////////////////////////////////////////////////////////////////
CMacroParameters* CCompany::GetMacroParametersPointer()
{
	return &theApp.m_Macro;
}

CMacroParameters CCompany::GetMacroParameters()
{
	return theApp.m_Macro;
}

void CCompany::SetMacroParameters(CMacroParameters &Macro)
{
	theApp.m_Macro = Macro;
}

BOOL CCompany::LoadHistory(COleDateTime from, COleDateTime to)
{
	if(!m_pHistory)
		m_pHistory = new CHistory(this);

	return m_pHistory->Load(GetTicker(), from, to);
}

BOOL CCompany::LoadHistory(int nNumRecords)
{
	if(!m_pHistory)
		m_pHistory = new CHistory(this);

	return m_pHistory->Load(GetTicker(), nNumRecords);
}

BOOL CCompany::PreLoadHistory()
{
	if(!m_pHistory)
		m_pHistory = new CHistory(this);

	return m_pHistory->PreLoad(GetTicker());
}

void CCompany::ClearHistory()
{
	if(m_pHistory)
	{
		//m_pHistory->Clear();
		delete m_pHistory;
		m_pHistory = NULL;
	}
}

/////////////////////////////////////////////////////////////////////
void CCompany::SetROCHistory(double* values, unsigned int nSize)
{
	if(nSize > ROCEPS_HYTORY_SIZE)
	{
		theApp.m_Exception.SetException(CIP_ERR_HISTORY_OVERFLOW);
		return;
	}

	unsigned int i;
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		m_arfROC[i] = m_arfROCOriginal[i] = NULL_VAL;

	for(i=0; i<nSize; i++)
	{
		double val = values[i];;
		if(val < 0) 
			val = 0; 
		else if(val < _INF_ && val > 50) 
			val = 50;
		m_arfROC[ROCEPS_HYTORY_SIZE-nSize+i] = m_arfROCOriginal[ROCEPS_HYTORY_SIZE-nSize+i] = val;
	}
}

void CCompany::SetROCHistory(CArray<double, double>& arValues)
{
	SetROCHistory(arValues.GetData(), arValues.GetSize());
}

unsigned int CCompany::GetROCHistory(double* values)
{
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
		values[i] = m_arfROC[i];
	return ROCEPS_HYTORY_SIZE;
}

unsigned int CCompany::GetROCHistory(CArray<double, double>& arValues)
{
	double values[ROCEPS_HYTORY_SIZE];
	unsigned int nSize = GetROCHistory(values);

	arValues.SetSize(nSize);
	for(unsigned int i=0; i<nSize; i++)
		arValues[i] = values[i];

	return nSize;
}


///////////////////////////////////////////////////////////////////////////////
void CCompany::SetROEHistory(double* values, unsigned int nSize)
{
	if(nSize > ROCEPS_HYTORY_SIZE)
	{
		theApp.m_Exception.SetException(CIP_ERR_HISTORY_OVERFLOW);
		return;
	}

	unsigned int i;
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		m_arfROE[i] = m_arfROEOriginal[i] = NULL_VAL;

	for(i=0; i<nSize; i++)
	{
		double val = values[i];;
		if(val < 0) 
			val = 0; 
		else if(val < _INF_ && val > 50) 
			val = 50;
		m_arfROE[ROCEPS_HYTORY_SIZE-nSize+i] = m_arfROEOriginal[ROCEPS_HYTORY_SIZE-nSize+i] = val;
	}
}

void CCompany::SetROEHistory(CArray<double, double>& arValues)
{
	SetROEHistory(arValues.GetData(), arValues.GetSize());
}

unsigned int CCompany::GetROEHistory(double* values)
{
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
		values[i] = m_arfROE[i];
	return ROCEPS_HYTORY_SIZE;
}

unsigned int CCompany::GetROEHistory(CArray<double, double>& arValues)
{
	double values[ROCEPS_HYTORY_SIZE];
	unsigned int nSize = GetROEHistory(values);

	arValues.SetSize(nSize);
	for(unsigned int i=0; i<nSize; i++)
		arValues[i] = values[i];

	return nSize;
}

///////////////////////////////////////////////////////////////////////////////
void CCompany::SetEPSHistory(double* values, unsigned int nSize)
{
	if(nSize > ROCEPS_HYTORY_SIZE)
	{
		theApp.m_Exception.SetException(CIP_ERR_HISTORY_OVERFLOW);
		return;
	}

	unsigned int i;
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		m_arfEPS[i] = m_arfEPSOriginal[i] = NULL_VAL;

	for(i=0; i<nSize; i++)
	{
		m_arfEPS[ROCEPS_HYTORY_SIZE-nSize+i] = values[i];
		m_arfEPSOriginal[ROCEPS_HYTORY_SIZE-nSize+i] = values[i]/m_fEPSAdjustMultiply - m_fEPSAdjustAdd;
	}
}

void CCompany::SetEPSHistory(CArray<double, double>& arValues)
{
	SetEPSHistory(arValues.GetData(), arValues.GetSize());
}

unsigned int CCompany::GetEPSHistory(double* values)
{
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
		values[i] = m_arfEPS[i];
	return ROCEPS_HYTORY_SIZE;
}

unsigned int CCompany::GetEPSHistory(CArray<double, double>& arValues)
{
	double values[ROCEPS_HYTORY_SIZE];
	unsigned int nSize = GetEPSHistory(values);

	arValues.SetSize(nSize);
	for(unsigned int i=0; i<nSize; i++)
		arValues[i] = values[i];

	return nSize;
}

unsigned int CCompany::GetSharesHistory(double* values)
{
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
		values[i] = m_arfShares[i];
	return ROCEPS_HYTORY_SIZE;
}

unsigned int CCompany::GetSharesHistory(CArray<double, double>& arValues)
{
	double values[ROCEPS_HYTORY_SIZE];
	unsigned int nSize = GetSharesHistory(values);

	arValues.SetSize(nSize);
	for(unsigned int i=0; i<nSize; i++)
		arValues[i] = values[i];

	return nSize;
}


///////////////////////////////////////////////////////////////////////
void CCompany::SetYearsForMeansTable(double* pRiskLevels, unsigned int* pYearsROC, unsigned int* pYearsEPS, unsigned int nSize)
{
	// clear old levels
	m_TableYearsForMeans.RemoveAll();

	unsigned int i;
	for(i=0; i<nSize; i++)
	{
		CTableYearsForMeansItem item;
		item.m_fRiskAdjustmentLevel = pRiskLevels[i];
		item.m_nYearsForROC = pYearsROC[i];
		item.m_nYearsForEPS = pYearsEPS[i];

		m_TableYearsForMeans.Add(item);
	}
}

unsigned int CCompany::GetYearsForMeansTable(double* pRiskLevels, unsigned int* pYearsROC, unsigned int* pYearsEPS)
{
	unsigned int nSize = m_TableYearsForMeans.GetSize();

	unsigned int i;
	for(i=0; i<nSize; i++)
	{
		pRiskLevels[i] = m_TableYearsForMeans[i].m_fRiskAdjustmentLevel;
		pYearsROC[i] = m_TableYearsForMeans[i].m_nYearsForROC;
		pYearsEPS[i] = m_TableYearsForMeans[i].m_nYearsForEPS;
	}

	return nSize;
}

void CCompany::GetYearsForMeans(double fRiskAdjustment, unsigned int &nYearsROC, unsigned int &nYearsEPS)
{
	nYearsROC = 7;
	nYearsEPS = 7;

	int i;
	for(i=0; i<m_TableYearsForMeans.GetSize(); i++)
	{
		CTableYearsForMeansItem item = m_TableYearsForMeans[i];
		if(fRiskAdjustment < item.m_fRiskAdjustmentLevel)
		{
			nYearsROC = item.m_nYearsForROC;
			nYearsEPS = item.m_nYearsForEPS;
			return;
		}
	}
}

void CCompany::SetWeightInIndex(const char* szIndex, double val)
{
	for(int i=0; i<NUM_WEIGHTS; i++)
		if(strcmp(szIndex, WeightsIndices[i])==0)
		{
			m_arWeightsInIndices[i] = val;
			return;
		}
}

double CCompany::GetWeightInIndex(const char* szIndex)
{
	for(int i=0; i<NUM_WEIGHTS; i++)
		if(strcmp(szIndex, WeightsIndices[i])==0)
			return m_arWeightsInIndices[i];
	return 0;
}

BOOL CCompany::CreateFromRawData(SCompanyRawData& data)
{
	// set up input member variables
	m_sTicker = data.Fundamentals.sTicker;
	m_sName = data.Fundamentals.sName;
	m_sIndustry = data.Fundamentals.sSector;	  // switch Sector and Industry
	m_sSector = data.Fundamentals.sIndustry;

	m_sCIQTicker = data.Fundamentals.sCIQTicker;
	m_bInSP500 = data.Fundamentals.bInSP;
	m_bRecentRollOver = data.Fundamentals.bRollOver;

	m_bLoadBS = data.Fundamentals.bLoadBS;
	m_bLoadEPS = data.Fundamentals.bLoadEPS;
	m_bLoadROCROE = data.Fundamentals.bLoadROCROE;
	m_bUseDebtAdjust = data.Fundamentals.bUseDebtAdjust;

	m_nType = data.Fundamentals.type;
	m_nYear = data.Fundamentals.nLastReport+1; 

	// Weights in Indices
	SetWeightsInIndices(data.WeightsInIndices.data);

	// EPS Adjustments
	m_fEPSAdjustMultiply = data.EpsRocRoe.fAdjustMult;
	m_fEPSAdjustAdd = data.EpsRocRoe.fAdjustAdd;
	
	// EPS
	for(int i=0; i<NUM_YEARS_FOR_EPS; i++)
	{
		// Save Original
		double fEps = data.EpsRocRoe.arEps[i];
		m_arfEPSOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_EPS - 2 + i] = fEps;

		// Adjust
		fEps = fEps * m_fEPSAdjustMultiply + m_fEPSAdjustAdd;
		if(fEps < 0)
			fEps = 0.0;
		m_arfEPS[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_EPS - 2 + i] = fEps;
	}
	// Previous Year
	double fEps = data.Fundamentals.fEpsLastYear;		
	m_arfEPSOriginal[ROCEPS_HYTORY_SIZE - 3] = fEps;
	fEps = fEps * m_fEPSAdjustMultiply + m_fEPSAdjustAdd;
	if(fEps < 0)
		fEps = 0.0;
	m_arfEPS[ROCEPS_HYTORY_SIZE - 3] = fEps;

	// Last 12 Months
	fEps = data.Fundamentals.fEpsLast12Months;		
	m_arfEPSOriginal[ROCEPS_HYTORY_SIZE - 2] = fEps;
	fEps = fEps * m_fEPSAdjustMultiply + m_fEPSAdjustAdd;
	if(fEps < 0)
		fEps = 0.01;
	m_arfEPS[ROCEPS_HYTORY_SIZE - 2] = fEps;

	// Next year forecast
	m_arfEPS[ROCEPS_HYTORY_SIZE - 1] = data.Fundamentals.fEpsNextYForecast;		

	// ROC and ROE 
	for(int i=0; i<NUM_YEARS_FOR_ROC; i++)
	{
		double f = data.EpsRocRoe.arRoc[i];
		m_arfROCOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_ROC + i] = f;
		if(f < 0)
		{
			if(i<NUM_YEARS_FOR_ROC-2)
				f = 0.0;
			else
				f = 0.1;
		}
		m_arfROC[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_ROC + i] = f;

		// ROE
		f = data.EpsRocRoe.arRoe[i];
		m_arfROEOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_ROC + i] = f;
		if(f < 0)
		{
			if(i<NUM_YEARS_FOR_ROC-2)
				f = 0.0;
			else
				f = 0.1;
		}
		m_arfROE[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_ROC + i] = f;
	}

	// Shares Outstanding
	for(int i=0; i<NUM_YEARS_FOR_EPS; i++)
	{
		// Save Original
		double fShares = data.Shares.arShares[i];
		m_arfSharesOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_EPS - 2 + i] = fShares;
		m_arfShares[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_EPS - 2 + i] = fShares;
	}

	// Price and Return
	m_dtPrice = data.PricesAndReturns.date;
	m_fPrice = data.PricesAndReturns.fPrice; 
	m_fReturn = data.PricesAndReturns.fReturn; 

	// Fundamentals (Balance Sheet)
	m_dtFundamentals = data.Fundamentals.dtReport;

	if(m_nType == 1 || m_nType == 2 || m_nType == 4 || m_nType == 5)
		m_fAssets = data.Fundamentals.fCommonEquity; // Banks, Insurance, Homebuilders, Divers.. assets = equity
	else
		m_fAssets = data.Fundamentals.fAssets;

	m_fLiabilities = data.Fundamentals.fLiabilities;
	m_fCommonEquity = data.Fundamentals.fCommonEquity;
	m_fNetPlant = data.Fundamentals.fFixed;
	m_fIntangibles = data.Fundamentals.fIntang;
	m_fCash = data.Fundamentals.fCash;
	m_fShortTermDebt = data.Fundamentals.fShortDebt;
	m_fLongTermDebt = data.Fundamentals.fLongDebt;
	m_fPreTaxIncome = data.Fundamentals.fPreTaxIncome;
	m_fInterestExpense = data.Fundamentals.fInterest;
	m_fDeprAndAmort = data.Fundamentals.fDeprAmort;
	m_fSharesOut = data.Fundamentals.fShares;
	m_fDividend = data.Fundamentals.fDiv;
	m_fEPSForecastPrevY = data.Fundamentals.fEpsLastYear;
	m_fEPSForecastThisY = data.Fundamentals.fEpsThisYForecast;
	m_fEPSForecastNextY = data.Fundamentals.fEpsNextYForecast;
	m_fBookValue = data.Fundamentals.fBookVal;

	m_sSplitRatio = data.Fundamentals.sSplit;
	m_dtSplit = data.Fundamentals.dtSplit;

	return true;
}

BOOL CCompany::GetRawData(SCompanyRawData& data)
{
	strcpy(data.Fundamentals.sTicker, m_sTicker);
	strcpy(data.Fundamentals.sName, m_sName);
	strcpy(data.Fundamentals.sIndustry, m_sSector);		// switch Sector and Industry
	strcpy(data.Fundamentals.sSector, m_sIndustry);
	strcpy(data.Fundamentals.sCIQTicker, m_sCIQTicker);

	data.Fundamentals.bInSP = m_bInSP500;
	data.Fundamentals.bLoadBS = m_bLoadBS;
	data.Fundamentals.bLoadEPS = m_bLoadEPS;
	data.Fundamentals.bLoadROCROE = m_bLoadROCROE;
	data.Fundamentals.bUseDebtAdjust = m_bUseDebtAdjust;

	data.Fundamentals.type = m_nType;
	data.Fundamentals.nLastReport = m_nYear-1;

	// EPS Adjustments
	data.EpsRocRoe.fAdjustMult = m_fEPSAdjustMultiply;
	data.EpsRocRoe.fAdjustAdd = m_fEPSAdjustAdd;

	// Ticker and Year 0
	strcpy(data.EpsRocRoe.sTicker, m_sTicker);
	data.EpsRocRoe.nYear0 = m_nYear - NUM_YEARS_FOR_EPS;

	// EPS
	for(int i=0; i<NUM_YEARS_FOR_EPS; i++)
		data.EpsRocRoe.arEps[i] = m_arfEPSOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_EPS - 2 + i];

	// Last 12 Months
	data.Fundamentals.fEpsLast12Months = m_arfEPSOriginal[ROCEPS_HYTORY_SIZE - 2];

	// ROC and ROE 
	for(int i=0; i<NUM_YEARS_FOR_ROC; i++)
	{
		data.EpsRocRoe.arRoc[i] = m_arfROCOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_ROC + i];
		data.EpsRocRoe.arRoe[i] = m_arfROEOriginal[ROCEPS_HYTORY_SIZE - NUM_YEARS_FOR_ROC + i];
	}

	// Price and Return
	strcpy(data.PricesAndReturns.sTicker, m_sTicker);
	data.PricesAndReturns.date = m_dtPrice;
	data.PricesAndReturns.fPrice = m_fPrice; 
	data.PricesAndReturns.fReturn = m_fReturn; 

	// Fundamentals (Balance Sheet)
	data.Fundamentals.dtReport = m_dtFundamentals;
	data.Fundamentals.fAssets = m_fAssets;
	data.Fundamentals.fLiabilities = m_fLiabilities;
	data.Fundamentals.fCommonEquity = m_fCommonEquity;
	data.Fundamentals.fFixed = m_fNetPlant;
	data.Fundamentals.fIntang = m_fIntangibles;
	data.Fundamentals.fCash = m_fCash;
	data.Fundamentals.fShortDebt = m_fShortTermDebt;
	data.Fundamentals.fLongDebt = m_fLongTermDebt;
	data.Fundamentals.fPreTaxIncome = m_fPreTaxIncome;
	data.Fundamentals.fInterest = m_fInterestExpense;
	data.Fundamentals.fDeprAmort = m_fDeprAndAmort;
	data.Fundamentals.fShares = m_fSharesOut;
	data.Fundamentals.fDiv = m_fDividend;
	data.Fundamentals.fEpsLastYear = m_fEPSForecastPrevY;
	data.Fundamentals.fEpsThisYForecast = m_fEPSForecastThisY;
	data.Fundamentals.fEpsNextYForecast = m_fEPSForecastNextY;
	data.Fundamentals.fBookVal = m_fBookValue;

	strcpy(data.Fundamentals.sSplit, m_sSplitRatio);
	data.Fundamentals.dtSplit = m_dtSplit;

	return true;
}

BOOL CCompany::LoadLocalCopyFromAppData()
{
	TCHAR szAppDataPath[_MAX_PATH];
	SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_APPDATA, false);

	CString sFile;
	sFile.Format("%s\\Cornerstone IP\\Fair Value Model\\Stock Data\\%s.sti", szAppDataPath, m_sTicker);

	CCompany temp;
	if(!temp.Load(sFile, false))
		return false;

	if(!m_bLoadBS)
		CopyBalanceSheetFrom(temp);

	if(!m_bLoadEPS)
		CopyEPSFrom(temp);

	return true;
}

BOOL CCompany::SaveLocalCopyInAppData()
{
	TCHAR szAppDataPath[_MAX_PATH];
	SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_APPDATA, false);

	CString sFile;
	sFile.Format("%s\\Cornerstone IP\\Fair Value Model\\Stock Data\\%s.sti", szAppDataPath, m_sTicker);

	return Save(sFile);
}

void CCompany::CopyBalanceSheetFrom(CCompany& src)
{
	// EPS Adjustments
	m_fEPSAdjustMultiply = src.m_fEPSAdjustMultiply;
	m_fEPSAdjustAdd = src.m_fEPSAdjustAdd;
	
	// EPS, ROC, ROE
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
	{
		m_arfROC[i] = src.m_arfROC[i];
		m_arfROE[i] = src.m_arfROE[i];
		m_arfEPS[i] = src.m_arfEPS[i];
	}

	// Price and Return
	m_dtPrice = src.m_dtPrice;
	m_fPrice = src.m_fPrice;
	m_fReturn = src.m_fReturn;

	// Fundamentals (Balance Sheet)
	m_dtFundamentals = src.m_dtFundamentals;
	m_fAssets = src.m_fAssets;
	m_fLiabilities = src.m_fLiabilities;
	m_fCommonEquity = src.m_fCommonEquity;
	m_fNetPlant = src.m_fNetPlant;
	m_fIntangibles = src.m_fIntangibles;
	m_fCash = src.m_fCash;
	m_fShortTermDebt = src.m_fShortTermDebt;
	m_fLongTermDebt = src.m_fLongTermDebt;
	m_fPreTaxIncome = src.m_fPreTaxIncome;
	m_fInterestExpense = src.m_fInterestExpense;
	m_fDeprAndAmort = src.m_fDeprAndAmort;
	m_fSharesOut = src.m_fSharesOut;
	m_fDividend = src.m_fDividend;
	m_fEPSForecastPrevY = src.m_fEPSForecastPrevY;
	m_fEPSForecastThisY = src.m_fEPSForecastThisY;
	m_fEPSForecastNextY = src.m_fEPSForecastNextY;
	m_fBookValue = src.m_fBookValue;

	m_sSplitRatio = src.m_sSplitRatio;
	m_dtSplit = src.m_dtSplit;
}

void CCompany::CopyEPSFrom(CCompany& src)
{
	// EPS, ROC, ROE
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
	{
		m_arfROC[i] = src.m_arfROC[i];
		m_arfROE[i] = src.m_arfROE[i];
		m_arfEPS[i] = src.m_arfEPS[i];
	}
}

void CCompany::operator = (CCompany& src)
{
	// set up input member variables
	m_nYear = src.m_nYear;
	m_sTicker = src.m_sTicker;
	m_sName = src.m_sName;
	m_sIndustry = src.m_sIndustry;
	m_sSector = src.m_sSector;

	// download Balance Sheet and EPS history from database
	m_bLoadBS = src.m_bLoadBS;
	m_bLoadEPS = src.m_bLoadEPS;

	m_bUseDebtAdjust = src.m_bUseDebtAdjust;
	m_fDilution = src.m_fDilution;

	// EPS Adjustments
	m_fEPSAdjustMultiply = src.m_fEPSAdjustMultiply;
	m_fEPSAdjustAdd = src.m_fEPSAdjustAdd;
	
	// EPS, ROC, ROE
	for(int i=0; i<ROCEPS_HYTORY_SIZE; i++)
	{
		m_arfROC[i] = src.m_arfROC[i];
		m_arfROE[i] = src.m_arfROE[i];
		m_arfEPS[i] = src.m_arfEPS[i];
	}

	// Price and Return
	m_dtPrice = src.m_dtPrice;
	m_fPrice = src.m_fPrice;
	m_fReturn = src.m_fReturn;

	// Fundamentals (Balance Sheet)
	m_dtFundamentals = src.m_dtFundamentals;
	m_fAssets = src.m_fAssets;
	m_fLiabilities = src.m_fLiabilities;
	m_fCommonEquity = src.m_fCommonEquity;
	m_fNetPlant = src.m_fNetPlant;
	m_fIntangibles = src.m_fIntangibles;
	m_fCash = src.m_fCash;
	m_fShortTermDebt = src.m_fShortTermDebt;
	m_fLongTermDebt = src.m_fLongTermDebt;
	m_fPreTaxIncome = src.m_fPreTaxIncome;
	m_fInterestExpense = src.m_fInterestExpense;
	m_fDeprAndAmort = src.m_fDeprAndAmort;
	m_fSharesOut = src.m_fSharesOut;
	m_fDividend = src.m_fDividend;
	m_fEPSForecastPrevY = src.m_fEPSForecastPrevY;
	m_fEPSForecastThisY = src.m_fEPSForecastThisY;
	m_fEPSForecastNextY = src.m_fEPSForecastNextY;
	m_fBookValue = src.m_fBookValue;

	m_sSplitRatio = src.m_sSplitRatio;
	m_dtSplit = src.m_dtSplit;
}

void CCompany::GetHistoryItem(SHistoryItem& item)
{
	item.data[1] = m_fPrice;
	item.data[2] = m_nRankEPV;
	item.data[3] = m_nRankEPVRazor;
	item.data[4] = m_nRankRAV;
	item.data[5] = m_nRankBEV;
	item.data[6] = m_nRankPMV;
	item.data[7] = m_fEPV;
	item.data[8] = m_fEPVFore;
	item.data[9] = m_fMomentumActual;
	item.data[10] = m_nRankMomentumActual;
	item.data[11] = m_fEPVRazor;
	item.data[12] = m_fEPVRazorFore;
	item.data[13] = m_fMomentumRazor;
	item.data[14] = m_nRankMomentumRazor;
	item.data[15] = m_fGrowthEstimate1*100.0;
	item.data[16] = m_nRankGrowth1;
	item.data[17] = m_fGrowthEstimate2*100.0;
	item.data[18] = m_nRankGrowth2;
	item.data[19] = m_fMultiple*100.0;
	item.data[20] = m_fMeanROI*100.0;
	item.data[21] = m_fAdjustedGrowthRate*100.0;
	item.data[22] = m_fRiskAdjustment;
	item.data[23] = m_fYield*100;
	item.data[24] = m_fPE;
	item.data[25] = m_fEV_EBITDA;
	item.data[26] = m_fAdjustForPayout*100;
	item.data[27] = m_fNormEPS;

	item.data[28] = m_fBookEPS;
	item.data[29] = m_fBookValue;
	item.data[30] = m_fDebtRatio;
	item.data[31] = m_fMeanROC;
	item.data[32] = m_fMeanROE;
	item.data[33] = m_fMeanEPS;
	item.data[34] = m_fAdjustedAssets_Share;
	item.data[35] = m_fNetCashFlow_Share;
	item.data[36] = m_fDeprAmort_Share;
	item.data[37] = m_fOperFixChargeCover;
	item.data[38] = m_fInterestCover;

	item.data[39] = m_fDividend;
	item.data[40] = m_fEPSForecastNextY;
	item.data[41] = m_fEPSForecastThisY;
	item.data[42] = m_fEPSForecastPrevY;
	item.data[43] = m_fAssetLife;
	item.data[44] = m_fHistoricalGrowth;
	item.data[45] = m_fIndicatedGrowth;

	for(int i=0; i<NUM_WEIGHTS; i++)
		item.data[46+i] = m_arWeightsInIndices[i]*100.0;
}

double CCompany::GetSplitRatioDouble()
{
	int nFor = m_sSplitRatio.Find("for");
	if(nFor == -1)
		return 0;

	CString sAfter = m_sSplitRatio.Left(nFor);
	sAfter.TrimLeft(); sAfter.TrimRight();
	double fAfter = atof(sAfter);
	if(fAfter <= 0)
		return 0;

	CString sBefore = m_sSplitRatio.Mid(nFor+3);
	sBefore.TrimLeft(); sBefore.TrimRight();
	double fBefore = atof(sBefore);
	if(fBefore <= 0)
		return 0;

	return fAfter / fBefore;
}

BOOL CCompany::CheckForChanges(SChangeCriteria& ch)
{
	if(ch.nPeriods == 0)
		return false;

	if(ch.nPeriods > m_pHistory->GetSize())
		ch.nPeriods = m_pHistory->GetSize();

	BOOL bYes = false;
	int nItem = -1;

	// check for Normalized Rank
	if(ch.nDeltaRankN != 0)
	{
		nItem = CDataManager::GetColIndexFromName("NEPVRnk");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "NEPVRnk", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if((double)m_nRankEPV > fMax)
			fMax = (double)m_nRankEPV;

		if((double)m_nRankEPV < fMin)
			fMin = (double)m_nRankEPV;

		int nDelta = (int)(fMax - fMin);
		if(nDelta >= ch.nDeltaRankN)
		{
			ch.nDeltaRankN = nDelta;
			bYes = true;
		}
		else
			ch.nDeltaRankN = 0;
	}

	// check for Razor Rank
	if(ch.nDeltaRankR != 0)
	{
		nItem = CDataManager::GetColIndexFromName("REPVRnk");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "REPVRnk", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if((double)m_nRankEPVRazor > fMax)
			fMax = (double)m_nRankEPVRazor;

		if((double)m_nRankEPVRazor < fMin)
			fMin = (double)m_nRankEPVRazor;

		int nDelta = (int)(fMax - fMin);
		if(nDelta >= ch.nDeltaRankR)
		{
			ch.nDeltaRankR = nDelta;
			bYes = true;
		}
		else
			ch.nDeltaRankR = 0;
	}

	// check for BEV Rank
	if(ch.nDeltaBEV != 0)
	{
		nItem = CDataManager::GetColIndexFromName("BEVRnk");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "BEVRnk", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if((double)m_nRankBEV > fMax)
			fMax = (double)m_nRankBEV;

		if((double)m_nRankBEV < fMin)
			fMin = (double)m_nRankBEV;

		int nDelta = (int)(fMax - fMin);
		if(nDelta >= ch.nDeltaBEV)
		{
			ch.nDeltaBEV = nDelta;
			bYes = true;
		}
		else
			ch.nDeltaBEV = 0;
	}

	// check for Price
	if(ch.fDeltaPrice != 0)
	{
		nItem = CDataManager::GetColIndexFromName("Price");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "Price", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if(m_fPrice > fMax)
			fMax = m_fPrice;

		if(m_fPrice < fMin)
			fMin = m_fPrice;

		double fDelta = (fMin != 0) ? fMax / fMin - 1 : (fMax != 0) ? 1 : 0;
		if(fDelta >= ch.fDeltaPrice)
		{
			ch.fDeltaPrice = fDelta;
			bYes = true;
		}
		else
			ch.fDeltaPrice = 0;
	}

	// check for next year EPS
	if(ch.fDeltaEPSNext != 0)
	{
		nItem = CDataManager::GetColIndexFromName("EPSNxt");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "EPSNxt", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if(m_fEPSForecastNextY > fMax)
			fMax = m_fEPSForecastNextY;

		if(m_fEPSForecastNextY < fMin)
			fMin = m_fEPSForecastNextY;

		double fDelta = (fMin != 0) ? fMax / fMin - 1 : (fMax != 0) ? 1 : 0;
		if(fDelta >= ch.fDeltaEPSNext)
		{
			ch.fDeltaEPSNext = fDelta;
			bYes = true;
		}
		else
			ch.fDeltaEPSNext = 0;
	}

	// check for this year EPS
	if(ch.fDeltaEPSThis != 0)
	{
		nItem = CDataManager::GetColIndexFromName("EPSThis");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "EPSThis", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if(m_fEPSForecastThisY > fMax)
			fMax = m_fEPSForecastThisY;

		if(m_fEPSForecastThisY < fMin)
			fMin = m_fEPSForecastThisY;

		double fDelta = (fMin != 0) ? fMax / fMin - 1 : (fMax != 0) ? 1 : 0;
		if(fDelta >= ch.fDeltaEPSThis)
		{
			ch.fDeltaEPSThis = fDelta;
			bYes = true;
		}
		else
			ch.fDeltaEPSThis = 0;
	}

	// check for Debt Ratio
	if(ch.fDeltaDebt != 0)
	{
		nItem = CDataManager::GetColIndexFromName("DebtRto");
		if(m_pHistory->ItemLoadedSize(nItem) < ch.nPeriods)
			m_pHistory->LoadItem(GetTicker(), "DebtRto", ch.nPeriods);

		double fMax=-10000000, fMin=100000000;
		for(int i=0; i<ch.nPeriods; i++)
		{
			double val = m_pHistory->GetItemPtr(i)->data[nItem];
			if(val < _INF_ && val > fMax)
				fMax = val;

			if(val < fMin)
				fMin = val;
		}
		if(m_fDebtRatio > fMax)
			fMax = m_fDebtRatio;

		if(m_fDebtRatio < fMin)
			fMin = m_fDebtRatio;

		double fDelta = (fMin != 0) ? fMax / fMin - 1 : (fMax != 0) ? 1 : 0;
		if(fDelta >= ch.fDeltaDebt)
		{
			ch.fDeltaDebt = fDelta;
			bYes = true;
		}
		else
			ch.fDeltaDebt = 0;
	}

	return bYes;
}

///////////////////////////////////////////////////////////////////////
void CCompany::CalcAll()
{
	SetEPS(ROCEPS_HYTORY_SIZE-1, GetEPSForecastNextYear());

	CalcRiskAdjustment();
	CalcYearsForMeanROCandEPS();
	CalcMeanROC();
	CalcMeanROE();
	CalcMeanEPS();
	CalcMeanROCFor10Years();
	CalcPhasesAndMagnitudes();
	CalcDebtAndEquity(); 
	CalcDebtRatio();
	CalcMeanROI();
	CalcRatios();
	CalcHistoricalGrowth();
}	

void CCompany::DoSensitivityAnalysis(double fRoc, double fRoe, double fEps, double fDebtRatio, 
					double fPrice, double fDiv, double fEpsNext, double fEpsThis, double fEpsLast)
{
	m_fPrice = fPrice;
	m_fDividend = fDiv;
	m_fEPSForecastNextY = fEpsNext;
	m_fEPSForecastThisY = fEpsThis;
	m_fEPSForecastPrevY = fEpsLast;

	CalcRiskAdjustment();
	CalcYearsForMeanROCandEPS();
	CalcMeanROC();
	CalcMeanROE();
	CalcMeanEPS();
	CalcMeanROCFor10Years();
	
	m_fMeanROC = fRoc;
	m_fMeanROE = fRoe;
	m_fMeanEPS = fEps;

	CalcPhasesAndMagnitudes();
	CalcDebtAndEquity(); 
	
	m_fDebtRatio = fDebtRatio;

	CalcMeanROI();
	CalcRatios();

}

void CCompany::CalcRiskAdjustment()
{
	// check for ROC history data to be valid
	/*if(m_arfROC[ROCEPS_HYTORY_SIZE - YEARS_TO_CALC_RISK_ADJUST - 2] >= NULL_VAL)
	{
		theApp.m_Exception.SetException(CIP_ERR_NO_ROC_HISTORY);
		return;
	}*/

	// Get Coeff of Var for ROC. Additional 2 means 1) this year and 2) next year forecasts
	double fCoeffVar = CMath::GetCoeffVar(	&m_arfROC[ROCEPS_HYTORY_SIZE - YEARS_TO_CALC_RISK_ADJUST - 2], 
											YEARS_TO_CALC_RISK_ADJUST + 2);  

	m_fRiskAdjustment = 1 - fCoeffVar;
	if(m_fRiskAdjustment < 0.01)
		m_fRiskAdjustment = 0.01;
}

void CCompany::CalcYearsForMeanROCandEPS()
{
	int i;
	for(i=0; i<m_TableYearsForMeans.GetSize(); i++)
	{
		CTableYearsForMeansItem item = m_TableYearsForMeans[i];
		if(m_fRiskAdjustment < item.m_fRiskAdjustmentLevel)
		{
			m_nYearsForROC = item.m_nYearsForROC;
			m_nYearsForEPS = item.m_nYearsForEPS;
			return;
		}
	}
}

void CCompany::CalcMeanROC() 
{
	int nYears = min(m_nYearsForROC, GetUniverse()->GetMacroParametersPointer()->GetYearsToUseEpsRoi());

	// this mean is the average for m_nYearsForROC, starting from THIS year (the next forecast year is ignored)
	m_fMeanROC = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE - nYears - 1], nYears);

	// shifted to the right by one 
	m_fMeanROCFore = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE - nYears], nYears);

	// Razor is just the forecast for this year
	m_fMeanROCRazor = m_arfROC[ROCEPS_HYTORY_SIZE - 2];

	// Razor forecast is just the forecast for Next year
	m_fMeanROCRazorFore = m_arfROC[ROCEPS_HYTORY_SIZE - 1];
}

void CCompany::CalcMeanROE()
{
	int nYears = min(m_nYearsForROC, GetUniverse()->GetMacroParametersPointer()->GetYearsToUseEpsRoi());

	// this mean is the average for m_nYearsForROC, starting from THIS year (the next forecast year is ignored)
	m_fMeanROE = CMath::GetMean(&m_arfROE[ROCEPS_HYTORY_SIZE - nYears - 1], nYears);

	// shifted to the right by one
	m_fMeanROEFore = CMath::GetMean(&m_arfROE[ROCEPS_HYTORY_SIZE - nYears], nYears);

	// Razor one is just the forecast for this year
	m_fMeanROERazor = m_arfROE[ROCEPS_HYTORY_SIZE - 2];

	// Razor Forecast is just the forecast for this year
	m_fMeanROERazorFore = m_arfROE[ROCEPS_HYTORY_SIZE - 1];
}

void CCompany::CalcMeanEPS()
{
	int nYears = min(m_nYearsForEPS, GetUniverse()->GetMacroParametersPointer()->GetYearsToUseEpsRoi());

	// this mean is the average for m_nYearsForEPS, starting from THIS year (the next forecast year is ignored)
	m_fMeanEPS = CMath::GetMean(&m_arfEPS[ROCEPS_HYTORY_SIZE - nYears - 1], nYears);

	// shifted to the right by one
	double tmp = m_arfEPS[ROCEPS_HYTORY_SIZE-1];
	m_arfEPS[ROCEPS_HYTORY_SIZE-1] = m_arfEPS[ROCEPS_HYTORY_SIZE-1]*m_fEPSAdjustMultiply + m_fEPSAdjustAdd;
	if(m_arfEPS[ROCEPS_HYTORY_SIZE-1] < 0)
		m_arfEPS[ROCEPS_HYTORY_SIZE-1] = 0.1;

	m_fMeanEPSFore = CMath::GetMean(&m_arfEPS[ROCEPS_HYTORY_SIZE - nYears], nYears);
	m_arfEPS[ROCEPS_HYTORY_SIZE-1] = tmp;

	// Razor one is ju st the forecast for this year
	m_fMeanEPSRazor = m_arfEPS[ROCEPS_HYTORY_SIZE - 2];
	if(m_fMeanEPSRazor < 0)
		m_fMeanEPSRazor = 0.1;

	// Razor Fore is just the forecast for this year
	m_fMeanEPSRazorFore = m_arfEPS[ROCEPS_HYTORY_SIZE - 1]*m_fEPSAdjustMultiply + m_fEPSAdjustAdd;
	if(m_fMeanEPSRazorFore < 0)
		m_fMeanEPSRazorFore = 0.1;
}

void CCompany::CalcMeanROCFor10Years()
{
	// this mean is the average for m_nYearsForEPS, starting from THIS year (the next forecast year is ignored)
	m_f10YROC = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE - 10], 10);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCompany::CalcPhasesAndMagnitudes()
{
	// Phase calculation for A (from current year)
	double fRoc1 = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE-m_nPhasePoint1-1], m_nPhasePoint1);
	double fRoc2 = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE-m_nPhasePoint2-1], m_nPhasePoint2);
	double fRoc3 = m_arfROC[ROCEPS_HYTORY_SIZE-2];

	int nPhaseA = 0;
	if(fRoc1 >= fRoc3 && fRoc3 >= fRoc2)  // Phase 1 - H, L, M
		nPhaseA = 1;
	else if(fRoc3 >= fRoc1 && fRoc1 >= fRoc2) // Phase 2 - M, L, H
		nPhaseA = 2;
	else if(fRoc3 >= fRoc2 && fRoc2 >= fRoc1)  // Phase 3 - L, M, H
		nPhaseA = 3;
	else if(fRoc2 >= fRoc3 && fRoc3 >= fRoc1)  // Phase 4 - L, H, M
		nPhaseA = 4;
	else if(fRoc2 >= fRoc1 && fRoc1 >= fRoc3)  // Phase 5 - M, H, L
		nPhaseA = 5;
	else if(fRoc1 >= fRoc2 && fRoc2 >= fRoc3)  // Phase 6 - H, M, L
		nPhaseA = 6;

	//ASSERT(nPhaseA != 0);
	if(nPhaseA == 0)
		nPhaseA = 1;

	m_fMagnitudeA = 100.0 * (fRoc3 - fRoc2) / fRoc3;

	// Phase calculation for E (from forecast year)
	fRoc1 = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE-m_nPhasePoint1], m_nPhasePoint1);
	fRoc2 = CMath::GetMean(&m_arfROC[ROCEPS_HYTORY_SIZE-m_nPhasePoint2], m_nPhasePoint2);
	fRoc3 = m_arfROC[ROCEPS_HYTORY_SIZE-1];

	int nPhaseE = 0;
	if(fRoc1 >= fRoc3 && fRoc3 >= fRoc2)  // Phase 1 - H, L, M
		nPhaseE = 1;
	else if(fRoc3 >= fRoc1 && fRoc1 >= fRoc2) // Phase 2 - M, L, H
		nPhaseE = 2;
	else if(fRoc3 >= fRoc2 && fRoc2 >= fRoc1)  // Phase 3 - L, M, H
		nPhaseE = 3;
	else if(fRoc2 >= fRoc3 && fRoc3 >= fRoc1)  // Phase 4 - L, H, M
		nPhaseE = 4;
	else if(fRoc2 >= fRoc1 && fRoc1 >= fRoc3)  // Phase 5 - M, H, L
		nPhaseE = 5;
	else if(fRoc1 >= fRoc2 && fRoc2 >= fRoc3)  // Phase 6 - H, M, L
		nPhaseE = 6;

	//ASSERT(nPhaseE != 0);
	if(nPhaseE == 0)
		nPhaseE   = 1;

	m_fMagnitudeE = 100.0 * (fRoc3 - fRoc2) / fRoc3;

	m_nPhaseA = nPhaseA;
	m_nPhaseE = nPhaseE;
}

void CCompany::CalcDebtAndEquity()
{
	// Total Debt
	m_fTotalDebt = m_fShortTermDebt + m_fLongTermDebt;

	if(m_nType == 2 || m_nType == 4 || m_nType == 5)
	{
		m_fNetPlant = 0;
		m_fDeprAndAmort = 0;
		m_fLiabilities = 0;
	}

	// Net Worth
	if(m_nType != 1 && m_nType != 2 && m_nType != 4 && m_nType != 5)
		m_fNetWorth = m_fAssets - m_fLiabilities;
	else
		m_fNetWorth = m_fAssets; // For Banks Assets is Equity and Liabilities is the Non-Performing Assets

	// Preferred Stock
	m_fPreferred = m_fNetWorth - m_fCommonEquity;
	if(m_fPreferred < 0)
		m_fPreferred = 0;
}

void CCompany::CalcDebtRatio()
{
	// Debt Ratio (this one is calculated here to be used in ROI formula)
	m_fDebtRatio = (m_fTotalDebt - m_fCash) / m_fNetWorth;
	if(m_fNetWorth > 0)
	{
		if(m_fDebtRatio < 0)
			m_fDebtRatio = 0;
	}
	else
	{
		if(m_fDebtRatio < 0 || (m_fNetWorth + m_fCash) < 0)
			m_fDebtRatio = 1;
		else
			m_fDebtRatio = 0;
	}
}

void CCompany::CalcMeanROI()
{
	double fOriginalDebtRatio = m_fDebtRatio;
	if(!m_bUseDebtAdjust)  // Just use ROC
	{
		m_fMeanROI = m_fMeanROC;
		m_fDebtRatio = 1;
	}
	else // blend ROE and ROC
	{
		if(m_fDebtRatio <= 0.5)
			m_fMeanROI = m_fMeanROE;
		else if(m_fDebtRatio > 1)
			m_fMeanROI = m_fMeanROC;
		else
		{
			double fROC = (m_fDebtRatio - 0.5) * 2.0; 
			m_fMeanROI = fROC*m_fMeanROC + (1.0-fROC)*m_fMeanROE;
		}
	}
	// make percentage real number
	m_fMeanROI /= 100.0;

	////////////////////////////////////////////////////////////////
	if(!m_bUseDebtAdjust)  // Just use ROC
	{
		m_fMeanROIFore = m_fMeanROCFore;
	}
	else // blend ROE and ROC
	{
		if(m_fDebtRatio <= 0.5)
			m_fMeanROIFore = m_fMeanROEFore;
		else if(m_fDebtRatio > 1)
			m_fMeanROIFore = m_fMeanROCFore;
		else
		{
			double fROC = (m_fDebtRatio - 0.5) * 2.0; 
			m_fMeanROIFore = fROC*m_fMeanROCFore + (1.0-fROC)*m_fMeanROEFore;
		}
	}
	// make percentage real number
	m_fMeanROIFore /= 100.0;

	////////////////////////////////////////////////////////////////
	if(!m_bUseDebtAdjust)  // Just use ROC
	{
		m_fMeanROIRazor = m_fMeanROCRazor;
	}
	else // blend ROE and ROC
	{
		if(m_fDebtRatio <= 0.5)
			m_fMeanROIRazor = m_fMeanROERazor;
		else if(m_fDebtRatio > 1)
			m_fMeanROIRazor = m_fMeanROCRazor;
		else
		{
			double fROC = (m_fDebtRatio - 0.5) * 2.0; 
			m_fMeanROIRazor = fROC*m_fMeanROCRazor + (1.0-fROC)*m_fMeanROERazor;
		}
	}
	// make percentage real number
	m_fMeanROIRazor /= 100.0;

	////////////////////////////////////////////////////////////////
	if(!m_bUseDebtAdjust)  // Just use ROC
	{
		m_fMeanROIRazorFore = m_fMeanROCRazorFore;
	}
	else // blend ROE and ROC
	{
		if(m_fDebtRatio <= 0.5)
			m_fMeanROIRazorFore = m_fMeanROERazorFore;
		else if(m_fDebtRatio > 1)
			m_fMeanROIRazorFore = m_fMeanROCRazorFore;
		else
		{
			double fROC = (m_fDebtRatio - 0.5) * 2.0; 
			m_fMeanROIRazorFore = fROC*m_fMeanROCRazorFore + (1.0-fROC)*m_fMeanROERazorFore;
		}
	}
	// make percentage real number
	m_fMeanROIRazorFore /= 100.0;

	// restore original debt ratio
	m_fDebtRatio = fOriginalDebtRatio;
}

void CCompany::CalcHistoricalGrowth()
{
	double x[] = {1,2,3,4,5,6};
	double slope, intercept, coeff;

	CMath::SimpleRegression(x, &m_arfEPS[ROCEPS_HYTORY_SIZE-8], 6, slope, intercept, coeff);
	
	double y0 = slope * 1.0 + intercept;
	double y5 = slope * 6.0 + intercept;

	m_fHistoricalGrowth = pow((y5/y0), 0.2) - 1;
}

void CCompany::CalcRatios()
{
	// Operating Fixed Charge Coverage
	if(m_fInterestExpense != 0)
		m_fOperFixChargeCover = 1+( (m_fPreTaxIncome+m_fDeprAndAmort) / m_fInterestExpense );
	else
		m_fOperFixChargeCover = 1.0;

	// Interest Coverage
	if(m_fInterestExpense != 0)
		m_fInterestCover = (m_fPreTaxIncome + m_fInterestExpense) / m_fInterestExpense;
	else 
		m_fInterestCover = 1.0;

	// Yield
	m_fYield = m_fDividend / m_fPrice;

	// Reinvestment Rate
	m_fReinvestRate = max(0, 1 - m_fDividend/m_fMeanEPS);
	m_fReinvestRateFore = max(0, 1 - m_fDividend/m_fMeanEPSFore);
	m_fReinvestRateRazor = max(0, 1 - m_fDividend/m_fMeanEPSRazor);
	m_fReinvestRateRazorFore = max(0, 1 - m_fDividend/m_fMeanEPSRazorFore);

	// Max Reinvestment Rate
	if(m_fReinvestRate < 0.5)
		m_fMaxReinvestRate = m_fReinvestRate;
	else
		m_fMaxReinvestRate = (m_fReinvestRate - 0.5)*theApp.m_Macro.GetMaxReinvestFactor() + 0.5;

	// Max Reinvestment Rate Forecast
	if(m_fReinvestRateFore < 0.5)
		m_fMaxReinvestRateFore = m_fReinvestRateFore;
	else
		m_fMaxReinvestRateFore = (m_fReinvestRateFore - 0.5)*theApp.m_Macro.GetMaxReinvestFactor() + 0.5;

	// Max Reinvestment Rate Razor
	if(m_fReinvestRateRazor < 0.5)
		m_fMaxReinvestRateRazor = m_fReinvestRateRazor;
	else
		m_fMaxReinvestRateRazor = (m_fReinvestRateRazor - 0.5)*theApp.m_Macro.GetMaxReinvestFactor() + 0.5;

	// Max Reinvestment Rate Razor Forecast
	if(m_fReinvestRateRazorFore < 0.5)
		m_fMaxReinvestRateRazorFore = m_fReinvestRateRazorFore;
	else
		m_fMaxReinvestRateRazorFore = (m_fReinvestRateRazorFore - 0.5)*theApp.m_Macro.GetMaxReinvestFactor() + 0.5;

	// P/E ratio
	m_fPE = m_fPrice / m_arfEPS[ROCEPS_HYTORY_SIZE-2];

	// EV/EBIDTA ratio
	double fEV = m_fSharesOut*m_fPrice + m_fLongTermDebt + m_fShortTermDebt - m_fCash;
	double fEbitda = m_fPreTaxIncome + m_fInterestExpense + m_fDeprAndAmort;
	m_fEV_EBITDA = (fEbitda != 0) ? fEV / fEbitda : 0;

	// Adjustment For Payout
	if(m_fReinvestRate < 0.5)
	{
		double fPayout = max(m_fDividend / m_fMeanEPS, 1-m_fMaxReinvestRate);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1 = fPayout - 0.5;
		m_fAdjustForPayout = factor1 * m_fMeanROI;
	}
	else
	{
		double fPayout = max(m_fDividend / m_fMeanEPS, 1-m_fMaxReinvestRate);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1;
		if(m_fMeanROI > 0.2)
			factor1 = 0;
		else if(m_fMeanROI < 0.1)
			factor1 = m_fMeanROI / 10.0;
		else
			factor1 = (0.2-m_fMeanROI)/10.0;

		double factor2 = 0.5-fPayout;
		double factor3 = 10.0;
		m_fAdjustForPayout = -factor1*factor2*factor3;
	}
	if(m_fAdjustForPayout < -0.05) m_fAdjustForPayout = -0.05; else
	if(m_fAdjustForPayout > 0.05) m_fAdjustForPayout = 0.05;

	// Adjustment For Payout Forecasted
	if(m_fReinvestRateFore < 0.5)
	{
		double fPayout = max(m_fDividend / m_fMeanEPSFore, 1-m_fMaxReinvestRateFore);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1 = fPayout - 0.5;
		m_fAdjustForPayoutFore = factor1 * m_fMeanROIFore;
	}
	else
	{
		double fPayout = max(m_fDividend / m_fMeanEPSFore, 1-m_fMaxReinvestRateFore);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1;
		if(m_fMeanROIFore > 0.2)
			factor1 = 0;
		else if(m_fMeanROIFore < 0.1)
			factor1 = m_fMeanROIFore / 10.0;
		else
			factor1 = (0.2-m_fMeanROIFore)/10.0;

		double factor2 = 0.5-fPayout;
		double factor3 = 10.0;
		m_fAdjustForPayoutFore = -factor1*factor2*factor3;
	}
	if(m_fAdjustForPayoutFore < -0.05) m_fAdjustForPayoutFore = -0.05; else
	if(m_fAdjustForPayoutFore > 0.05) m_fAdjustForPayoutFore = 0.05;

	// Adjustment For Payout Razor
	if(m_fReinvestRateRazor < 0.5)
	{
		double fPayout = max(m_fDividend / m_fMeanEPSRazor, 1-m_fMaxReinvestRateRazor);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1 = fPayout - 0.5;
		m_fAdjustForPayoutRazor = factor1 * m_fMeanROIRazor;
	}
	else
	{
		double fPayout = max(m_fDividend / m_fMeanEPSRazor, 1-m_fMaxReinvestRateRazor);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1;
		if(m_fMeanROIRazor > 0.2)
			factor1 = 0;
		else if(m_fMeanROIRazor < 0.1)
			factor1 = m_fMeanROIRazor / 10.0;
		else
			factor1 = (0.2-m_fMeanROIRazor)/10.0;

		double factor2 = 0.5-fPayout;
		double factor3 = 10.0;
		m_fAdjustForPayoutRazor = -factor1*factor2*factor3;
	}
	if(m_fAdjustForPayoutRazor < -0.05) m_fAdjustForPayoutRazor = -0.05; else
	if(m_fAdjustForPayoutRazor > 0.05) m_fAdjustForPayoutRazor = 0.05;

	// Adjustment For Payout Razor Forecast
	if(m_fReinvestRateRazorFore < 0.5)
	{
		double fPayout = max(m_fDividend / m_fMeanEPSRazorFore, 1-m_fMaxReinvestRateRazorFore);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1 = fPayout - 0.5;
		m_fAdjustForPayoutRazorFore = factor1 * m_fMeanROIRazorFore;
	}
	else
	{
		double fPayout = max(m_fDividend / m_fMeanEPSRazorFore, 1-m_fMaxReinvestRateRazorFore);
		if(fPayout > 1)
			fPayout = 1.;
		double factor1;
		if(m_fMeanROIRazorFore > 0.2)
			factor1 = 0;
		else if(m_fMeanROIRazorFore < 0.1)
			factor1 = m_fMeanROIRazorFore / 10.0;
		else
			factor1 = (0.2-m_fMeanROIRazorFore)/10.0;

		double factor2 = 0.5-fPayout;
		double factor3 = 10.0;
		m_fAdjustForPayoutRazorFore = -factor1*factor2*factor3;
	}
	if(m_fAdjustForPayoutRazorFore < -0.05) m_fAdjustForPayoutRazorFore = -0.05; else
	if(m_fAdjustForPayoutRazorFore > 0.05) m_fAdjustForPayoutRazorFore = 0.05;
	
	// Growth Rate
	m_fGrowthRate = m_fMeanROI * m_fReinvestRate;
	m_fGrowthRateFore = m_fMeanROIFore * m_fReinvestRateFore;
	m_fGrowthRateRazor = m_fMeanROIRazor * m_fReinvestRateRazor;
	m_fGrowthRateRazorFore = m_fMeanROIRazorFore * m_fReinvestRateRazorFore;

	// Estimated Growth for 1 year (MO1)
	if(m_fEPSForecastThisY > 0 && m_fEPSForecastNextY > 0)
		m_fGrowthEstimate1 = m_fEPSForecastNextY / m_fEPSForecastThisY - 1;
	else
		m_fGrowthEstimate1 = (m_fEPSForecastNextY - m_fEPSForecastThisY) / m_fPrice;

	// Estimated Growth for 2 years (MO2) = NextY / LastY
	if(m_fEPSForecastPrevY > 0 && m_fEPSForecastNextY > 0)
		m_fGrowthEstimate2 = sqrt( m_fEPSForecastNextY / m_fEPSForecastPrevY) - 1;
	else
		m_fGrowthEstimate2 = (m_fEPSForecastNextY - m_fEPSForecastPrevY) / m_fPrice;

	// Adjusted Growth Rate
	if((m_fGrowthRate >= theApp.m_Macro.GetMinGrowth() && m_fGrowthEstimate2 < theApp.m_Macro.GetMinGrowth()))  // Don't let go less than 5%
		m_fAdjustedGrowthRate = theApp.m_Macro.GetMinGrowth();
	else
	{
		m_fAdjustedGrowthRate = min(m_fGrowthRate, m_fMaxReinvestRate*m_fMeanROI);

		if(m_fGrowthEstimate2 > theApp.m_Macro.GetMinGrowth() && m_fGrowthEstimate2 < m_fAdjustedGrowthRate)
			m_fAdjustedGrowthRate = m_fGrowthEstimate2;

		if(theApp.m_Macro.GetMaxReinvestFactor() < 1)
			m_fAdjustedGrowthRate = min( m_fAdjustedGrowthRate, (m_fGrowthEstimate2 - (1-theApp.m_Macro.GetMaxReinvestFactor())/100.0 ) );
	}

	// Adjusted Growth Rate Forecasted
	if((m_fGrowthRateFore >= 0.05 && m_fGrowthEstimate2 < 0.05))  // Don't let go less than 5%
		m_fAdjustedGrowthRateFore = 0.05;
	else
	{
		m_fAdjustedGrowthRateFore = min(m_fGrowthRateFore, m_fMaxReinvestRateFore*m_fMeanROIFore);

		if(m_fGrowthEstimate2 > 0.05 && m_fGrowthEstimate2 < m_fAdjustedGrowthRateFore)
			m_fAdjustedGrowthRateFore = m_fGrowthEstimate2;

		if(theApp.m_Macro.GetMaxReinvestFactor() < 1)
			m_fAdjustedGrowthRateFore = min( m_fAdjustedGrowthRateFore, (m_fGrowthEstimate2 - (1-theApp.m_Macro.GetMaxReinvestFactor())/100.0 ) );
	}

	// Adjusted Growth Rate Razor
	if(m_fGrowthRateRazor >= 0.05 && (m_fGrowthEstimate1 < 0.05 || m_fGrowthEstimate2 < 0.05 ))  // Don't let go less than 5%
		m_fAdjustedGrowthRateRazor = 0.05;
	else if(m_fGrowthRateRazor < 0.05 && (m_fGrowthEstimate1 < 0.05 || m_fGrowthEstimate2 < 0.05 ))
		m_fAdjustedGrowthRateRazor = m_fGrowthRateRazor;
	else
	{
		m_fAdjustedGrowthRateRazor = min(m_fGrowthRateRazor, m_fMaxReinvestRateRazor*m_fMeanROIRazor);

		if(m_fGrowthEstimate1 > 0 && m_fGrowthEstimate1 < m_fAdjustedGrowthRateRazor)
			m_fAdjustedGrowthRateRazor = m_fGrowthEstimate1;

		if(m_fGrowthEstimate2 > 0 && m_fGrowthEstimate2 < m_fAdjustedGrowthRateRazor)
			m_fAdjustedGrowthRateRazor = m_fGrowthEstimate2;

		if(theApp.m_Macro.GetMaxReinvestFactor() < 1)
			m_fAdjustedGrowthRateRazor = min( m_fAdjustedGrowthRateRazor, (m_fGrowthEstimate2 - (1-theApp.m_Macro.GetMaxReinvestFactor())/100.0 ) );
	}

	// Adjusted Growth Rate Razor Forecast
	if(m_fGrowthRateRazorFore >= 0.05 && (m_fGrowthEstimate1 < 0.05 || m_fGrowthEstimate2 < 0.05 ))  // Don't let go less than 5%
		m_fAdjustedGrowthRateRazorFore = 0.05;
	else if(m_fGrowthRateRazorFore < 0.05 && (m_fGrowthEstimate1 < 0.05 || m_fGrowthEstimate2 < 0.05 ))
		m_fAdjustedGrowthRateRazorFore = m_fGrowthRateRazorFore;
	else
	{
		m_fAdjustedGrowthRateRazorFore = min(m_fGrowthRateRazorFore, m_fMaxReinvestRateRazorFore*m_fMeanROIRazorFore);

		if(m_fGrowthEstimate1 > 0 && m_fGrowthEstimate1 < m_fAdjustedGrowthRateRazorFore)
			m_fAdjustedGrowthRateRazorFore = m_fGrowthEstimate1;

		if(m_fGrowthEstimate2 > 0 && m_fGrowthEstimate2 < m_fAdjustedGrowthRateRazorFore)
			m_fAdjustedGrowthRateRazorFore = m_fGrowthEstimate2;

		if(theApp.m_Macro.GetMaxReinvestFactor() < 1)
			m_fAdjustedGrowthRateRazorFore = min( m_fAdjustedGrowthRateRazorFore, (m_fGrowthEstimate2 - (1-theApp.m_Macro.GetMaxReinvestFactor())/100.0 ) );
	}

	// Multiple
	if(m_fDebtRatio  > 0.5)
		m_fMultiple = m_fAdjustedGrowthRate*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayout /*- fDebtAdjust*/;
	else
	{
		double fRiskDebtAdjust = 1 + ((( max(0, m_fRiskAdjustment-0.7))/0.3 ) * ((0.5-m_fDebtRatio)/0.5)*0.15);
		m_fMultiple = (m_fAdjustedGrowthRate*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayout ) * fRiskDebtAdjust;
	}

	// Multiple for Forecast
	if(m_fDebtRatio  > 0.5)
		m_fMultipleFore = m_fAdjustedGrowthRateFore*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayoutFore /*- fDebtAdjust*/;
	else
	{
		double fRiskDebtAdjust = 1 + ((( max(0, m_fRiskAdjustment-0.7))/0.3 ) * ((0.5-m_fDebtRatio)/0.5)*0.15);
		m_fMultipleFore = (m_fAdjustedGrowthRateFore*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayoutFore) * fRiskDebtAdjust;
	}

	// Multiple for Razor
	if(m_fDebtRatio  > 0.5)
		m_fMultipleRazor = m_fAdjustedGrowthRateRazor*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayoutRazor /*- fDebtAdjust*/;
	else
	{
		double fRiskDebtAdjust = 1 + ((( max(0, m_fRiskAdjustment-0.7))/0.3 ) * ((0.5-m_fDebtRatio)/0.5)*0.15);
		m_fMultipleRazor = (m_fAdjustedGrowthRateRazor*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayoutRazor) * fRiskDebtAdjust;
	}

	// Multiple for Razor Forecast
	if(m_fDebtRatio  > 0.5)
		m_fMultipleRazorFore = m_fAdjustedGrowthRateRazorFore*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor()  +  m_fAdjustForPayoutRazorFore /*- fDebtAdjust*/;
	else
	{
		double fRiskDebtAdjust = 1 + ((( max(0, m_fRiskAdjustment-0.7))/0.3 ) * ((0.5-m_fDebtRatio)/0.5)*0.15);
		m_fMultipleRazorFore = (m_fAdjustedGrowthRateRazorFore*theApp.m_Macro.GetFactor()  +  theApp.m_Macro.GetInterestFactor() +  m_fAdjustForPayoutRazorFore) * fRiskDebtAdjust;
	}

	// Asset Life
	if(m_fDeprAndAmort == 0.0 || m_nType == 6)  // some companies (banks) don't report D&A, so this should be entered manually
		m_fAssetLife = 10.0;   // this is default for banks (from where?)
	else 
		m_fAssetLife = m_fNetPlant / m_fDeprAndAmort;

	// Adjusted Assets / Share
	//if(m_nType != 1)
		m_fAdjustedAssets_Share = ( (m_fAssets-m_fNetPlant-m_fIntangibles-m_fLiabilities)/m_fAssetLife ) / m_fSharesOut;
	//else
	//	m_fAdjustedAssets_Share = ( (m_fCommonEquity-m_fNetPlant-m_fIntangibles-m_fLiabilities)/m_fAssetLife ) / m_fSharesOut;

	// Depr & Amort / Share
	m_fDeprAmort_Share = m_fDeprAndAmort / m_fSharesOut;

	// Net Chas Flow / Share
	m_fNetCashFlow_Share = m_fAdjustedAssets_Share + m_fDeprAmort_Share;

	// Break Even Value
	m_fBEV = CMath::GetPresentValue(m_fNetCashFlow_Share, theApp.m_Macro.GetDiscountRate(), m_fAssetLife);
	if(m_fBEV < 0)
		m_fBEV = 0.1;

	// Price / BEV
	m_fPrice_BEV = m_fPrice / m_fBEV;

	// Book EPS
	m_fBookEPS = m_fBookValue * m_fMeanROI;

	// Norm EPS
	m_fNormEPS = ( m_fBookEPS + m_fMeanEPS ) / 2.0;

	// Actual EPV
	m_fEPV = m_fMultiple * m_fMeanEPS * 100.0;

	// Actual RAV
	m_fRAV = m_fRiskAdjustment * m_fEPV;

	// Actual PMV
	m_fPMV = (m_fMeanEPS*m_fSharesOut) / ( theApp.m_Macro.GetInterestRate() * (1-theApp.m_Macro.GetTaxRate()) );
	m_fPMV += m_fDeprAndAmort / theApp.m_Macro.GetInterestRate() - m_fTotalDebt;
	m_fPMV /= m_fSharesOut;

	// Price Over Actual EPV
	m_fPrice_EPV = m_fPrice / m_fEPV;
	if(m_fPrice_EPV <= 0)
		m_fPrice_EPV = 10000;

	// Price Over Actual RAV
	m_fPrice_RAV = m_fPrice / m_fRAV;
	if(m_fPrice_RAV <= 0)
		m_fPrice_RAV = 10000;

	// Price Over Actual PMV
	m_fPrice_PMV = m_fPrice / m_fPMV;
	if(m_fPrice_PMV <= 0)
		m_fPrice_PMV = 10000;

	//////////////////////////////////////////////////////
	// Forecast EPV
	m_fEPVFore = m_fMultipleFore * m_fMeanEPSFore * 100.0;

	// Forecast RAV
	m_fRAVFore = m_fRiskAdjustment * m_fEPVFore;

	// Forecast PMV
	m_fPMVFore = (m_fMeanEPSFore*m_fSharesOut) / ( theApp.m_Macro.GetInterestRate() * (1-theApp.m_Macro.GetTaxRate()) );
	m_fPMVFore += m_fDeprAndAmort / theApp.m_Macro.GetInterestRate() - m_fTotalDebt;
	m_fPMVFore /= m_fSharesOut;

	// Price Over Forecast EPV
	m_fPrice_EPVFore = m_fPrice / m_fEPVFore;
	if(m_fPrice_EPVFore <= 0)
		m_fPrice_EPVFore = 10000;

	// Price Over Forecast RAV
	m_fPrice_RAVFore = m_fPrice / m_fRAVFore;
	if(m_fPrice_RAVFore <= 0)
		m_fPrice_RAVFore = 10000;

	// Price Over Forecast PMV
	m_fPrice_PMVFore = m_fPrice / m_fPMVFore;
	if(m_fPrice_PMVFore <= 0)
		m_fPrice_PMVFore = 10000;

	//////////////////////////////////////////////////////
	// Razor EPV
	m_fEPVRazor = m_fMultipleRazor * m_fMeanEPSRazor * 100.0;

	// Razor RAV
	m_fRAVRazor = m_fRiskAdjustment * m_fEPVRazor;

	// Razor PMV
	m_fPMVRazor = (m_fMeanEPSRazor*m_fSharesOut) / ( theApp.m_Macro.GetInterestRate() * (1-theApp.m_Macro.GetTaxRate()) );
	m_fPMVRazor += m_fDeprAndAmort / theApp.m_Macro.GetInterestRate() - m_fTotalDebt;
	m_fPMVRazor /= m_fSharesOut;

	// Price Over Razor EPV
	m_fPrice_EPVRazor = m_fPrice / m_fEPVRazor;
	if(m_fPrice_EPVRazor <= 0)
		m_fPrice_EPVRazor = 10000;

	// Price Over Razor RAV
	m_fPrice_RAVRazor = m_fPrice / m_fRAVRazor;
	if(m_fPrice_RAVRazor <= 0)
		m_fPrice_RAVRazor = 10000;

	// Price Over Razor PMV
	m_fPrice_PMVRazor = m_fPrice / m_fPMVRazor;
	if(m_fPrice_PMVRazor <= 0)
		m_fPrice_PMVRazor = 10000;

	//////////////////////////////////////////////////////
	// Razor Forecast EPV
 	m_fEPVRazorFore = m_fMultipleRazorFore * m_fMeanEPSRazorFore * 100.0;

	// Razor Forecast RAV
	m_fRAVRazorFore = m_fRiskAdjustment * m_fEPVRazorFore;

	// Razor Forecast PMV
	m_fPMVRazorFore = (m_fMeanEPSRazorFore*m_fSharesOut) / ( theApp.m_Macro.GetInterestRate() * (1-theApp.m_Macro.GetTaxRate()) );
	m_fPMVRazorFore += m_fDeprAndAmort / theApp.m_Macro.GetInterestRate() - m_fTotalDebt;
	m_fPMVRazorFore /= m_fSharesOut;

	// Price Over Razor Forecast EPV
	m_fPrice_EPVRazorFore = m_fPrice / m_fEPVRazorFore;
	if(m_fPrice_EPVRazorFore <= 0)
		m_fPrice_EPVRazorFore = 10000;

	// Price Over Razor Forecast RAV
	m_fPrice_RAVRazorFore = m_fPrice / m_fRAVRazorFore;
	if(m_fPrice_RAVRazorFore <= 0)
		m_fPrice_RAVRazorFore = 10000;

	// Price Over Razor Forecast PMV
	m_fPrice_PMVRazorFore = m_fPrice / m_fPMVRazorFore;
	if(m_fPrice_PMVRazorFore <= 0)
		m_fPrice_PMVRazorFore = 10000;

	// Momentum Actual
	if(m_fEPV > 0)
		m_fMomentumActual = 100.0 * m_fEPVFore / m_fEPV;
	else
		m_fMomentumActual = 0;

	// Momentum Razor
	if(m_fEPVRazor > 0)
		m_fMomentumRazor = 100.0 * m_fEPVRazorFore / m_fEPVRazor;
	else
		m_fMomentumRazor = 0;

	// Historical and Indicated Growth
	m_fHistoricalGrowth = m_fIndicatedGrowth = _INF_;
}	

