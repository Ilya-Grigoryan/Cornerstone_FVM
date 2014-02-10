// SQLDailySum.h: interface for the CSQLDailySum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLDAILYSUM_H__0BC98754_ED3F_4EF0_9AA4_3143D8272825__INCLUDED_)
#define AFX_SQLDAILYSUM_H__0BC98754_ED3F_4EF0_9AA4_3143D8272825__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct SDailySum
{
	int				nID;
	int				nCompanyID;
	int				nAnalystID;
	COleDateTime	date;
	double			fWeight;
	CString			sBias;
	int				nCoordValue;
	int				nCoordFund;
	int				nConfidenceValue;
	int				nConfidenceFund;
	CString			sNotesOnValue;
	CString			sNotesOnFund;
	int				nScoreMgmtVision;
	int				nScoreMgmtExec;
	CString			sNotesOnMgmt; 
	CString			sAnalystInitials;
	BOOL			bConfirmed;
	double			fWeight2;
	CString			sBias2;
	CString			sOneLiner;
};
typedef CArray<SDailySum*,SDailySum*> SDailySumArray;

/////////////////////////////////////////////////////////////////////
struct S2MinuteDrill
{
	int				nID;
	int				nCompanyID;
	COleDateTime	date;
	int				nNormalized;
	CString			sNormalizedNotes;
	int				nRazor;
	CString			sRazorNotes;
	int				nUndervalued;
	CString			sUndervaluedNotes;
	int				nPredictor;
	CString			sPredictorNotes;
	int				nMeanEPS;
	CString			sMeanEPSNotes;
	int				n3Year;			
	CString			s3YearNotes;
	int				nGrowth;		
	CString			sGrowthNotes;
	int				nEstimates;
	CString			sEstimatesNotes;
	int				nReturns;
	CString			sReturnsNotes;
	int				nAcct;
	CString			sAcctNotes;
	CString			sThesis;
};

///////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CSQLDailySum  
{
public:
	CSQLDailySum();
	~CSQLDailySum();

	static int UpdateDailySum(	int nCompanyID, int nAnalystID, COleDateTime date, 
								double fTargetWeight, const char* szBias,
								int nCoordValue, int nCoordFund, 
								int nConfidenceValue, int nConfidenceFund,
								const char* szNotesOnValue, const char* szNotesOnFund,
								int nScoreMgmtVision, int nScoreMgmtExec,
								const char* szNotesOnMgmt,
								BOOL bConfirmed, double fTargetWeight2, const char* szBias2,
								const char* szOneLiner);

	static BOOL GetDailySum( int nCompanyID, int nAnalystID, COleDateTime date, SDailySum& dailySum);
	static int GetDailySumsForTickerAndDate( int nCompanyID, COleDateTime date, SDailySumArray& arReturn);

	static int IsUnlocked(int nCompanyId, COleDateTime& date);
	static int Unlock(int nCompanyId, COleDateTime& date);
	static int Lock(int nCompanyId, COleDateTime& date);

	static int Update2MinuteDrill(	int nCompanyID, COleDateTime date,
									int nNormalized, const char* szNormalizedNotes,
									int nRazor, const char* szRazorNotes,
									int nUndervalued, const char* szUndervaluedNotes,
									int nPredictor, const char* szPredictorNotes,
									int nMeanEPS, const char* szMeanEPSNotes,
									int n3Year, const char* sz3YearNotes,
									int nGrowth, const char* szGrowthNotes,
									int nEstimates, const char* szEstimatesNotes,
									int nReturns, const char* szReturnsNotes,
									int nAcct, const char* szAcctNotes,
									const char* szThesis);
	static BOOL Get2MinuteDrill( int nCompanyID, S2MinuteDrill& ret);

};

#endif // !defined(AFX_SQLDAILYSUM_H__0BC98754_ED3F_4EF0_9AA4_3143D8272825__INCLUDED_)
