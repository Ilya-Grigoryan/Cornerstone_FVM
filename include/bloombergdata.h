#pragma once

#define PX_CLOSE_QTD					"PX_CLOSE_QTD"
#define PX_CLOSE_1D						"PX_CLOSE_1D"
#define T12M_DIL_EPS_CONT_OPS			"T12M_DIL_EPS_CONT_OPS"
#define IS_AVG_NUM_SH_FOR_EPS			"IS_AVG_NUM_SH_FOR_EPS"
#define ARD_DEPREC_DEPLETION_AMORT		"ARD_DEPREC_DEPLETION_AMORT"
#define BS_SH_OUT						"BS_SH_OUT"
#define BS_TOT_ASSET					"BS_TOT_ASSET"
#define BS_GOODWILL						"BS_GOODWILL"
#define BS_TOT_LIAB2					"BS_TOT_LIAB2"
#define BS_NET_FIX_ASSET				"BS_NET_FIX_ASSET"
#define ARD_EXPLORATION_EXPENSE			"ARD_EXPLORATION_EXPENSE"
#define OIL_PRODUCTION_WORLD			"OIL_PRODUCTION_WORLD"
#define GAS_PRODUCTION_WORLD			"GAS_PRODUCTION_WORLD"
#define OIL_END_YEAR_WORLD				"OIL_END_YEAR_WORLD"
#define GAS_END_YEAR_WORLD				"GAS_END_YEAR_WORLD"


//////////////////////////////////////////////////////////////////
struct CEnergyData
{
public:
	char		m_szTicker[64];
	
	double		m_fPriceQtr;
	double		m_fPrice1D;
	double		m_fEps12M;
	double		m_fSharesAvgEps;
	double		m_fDeprAmort;
	double		m_fShares;
	double		m_fAssets;
	double		m_fGoodwill;
	double		m_fLiabs;
	double		m_fFixedAssets;
	double		m_fExporationExpense;
	double		m_fOilProduction;
	double		m_fGasProduction;
	double		m_fOilReserve;
	double		m_fGasReserve;
};
typedef CArray<CEnergyData, CEnergyData> CEnergyDataArray;

//////////////////////////////////////////////////////////////////
class _declspec(dllexport) CBloombergData
{
public:
	CBloombergData(void);
	~CBloombergData(void);

public:
	static bool GetEnergyData(CStringArray &arTickers, CEnergyDataArray& arData);


protected:
	static bool ProcessResponseEnergyData(void* pEvent, CEnergyDataArray& arData);
};

