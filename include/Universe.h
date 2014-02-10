// Universe.h: interface for the CUniverse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIVERSE_H__927806A0_5CCE_425C_94B3_537B56E25906__INCLUDED_)
#define AFX_UNIVERSE_H__927806A0_5CCE_425C_94B3_537B56E25906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Company.h"
#include "ListEx.h"

#define _UNIVERSE_VERSION_		0x001

///////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CUniverse  
{
public:
	CUniverse();
	~CUniverse();

public:
	void	SetDate(COleDateTime date)		{ m_Today = date;						}
	COleDateTime GetDate()					{ return m_Today;						}

	int		GetSize()						{ return m_arCompanies.GetSize();		}
	CCompany*	GetCompany(int nIndex)		{ return m_arCompanies[nIndex];			}

	int		GetIndexByTicker(const char* szTicker, BOOL bExact=true);
	
	int		GetIndustriesCount()			{ return m_ListByIndustry.GetSize();	}
	int		GetSectorsCount()				{ return m_ListBySector.GetSize();		}
	
	CString	GetIndustryName(int nIndex)		{ return m_ListByIndustry.GetItem(nIndex)->m_Key;	}
	CString GetSectorName(int nIndex)		{ return m_ListBySector.GetItem(nIndex)->m_Key;		}
	
	void	GetCompaniesInIndustry(const char* szIndustry, CArray<CCompany*, CCompany*> &arResult);
	void	GetCompaniesInSector(const char* szSector, CArray<CCompany*,CCompany*> &arResult);
	int		GetCompaniesInIndex(const char* szIndex, CArray<CCompany*,CCompany*> &arResult);
	int		GetCompaniesInType(int nType, CArray<CCompany*,CCompany*> &arResult);

	double	GetSectorNEPVRank(const char* szSector, int nAveType=1);		// AveType: 1-equal weight, 2-price weight, 3-cap weight
	double	GetSectorREPVRank(const char* szSector, int nAveType=1);
	double	GetIndustryNEPVRank(const char* szSector, int nAveType=1);		
	double	GetIndustryREPVRank(const char* szSector, int nAveType=1);

	CCompany* GetCompanyByRankEPV(int nRank)	{ return (CCompany*)m_ListByRankEPV.GetItem(nRank)->m_pData;	}
	CCompany* GetCompanyByTicker(int nIndex)	{ return (CCompany*)m_ListByTicker.GetItem(nIndex)->m_pData;	}
	void	AddCompany(CCompany* pCompany)		{ m_arCompanies.Add(pCompany); pCompany->SetUniverse(this);		}
	int		RemoveCompany(CCompany* pCompany);

	BOOL CreateFromRawData(SUniverseRawData& UniverseData);
	void CalcAll();

	void LoadAll(int nUniverse, int nYear, COleDateTime dtPrices);
	void ClearAll();

	void	RecalcRankings();
	
	int		GetCompaniesWithRollOverYear(CArray<CCompany*,CCompany*>& arCompanies);
	int		GetCompaniesWithNoSharesOut(CArray<CCompany*,CCompany*>& arCompanies);
	int		GetCompaniesWithSP500Controversy(CArray<CCompany*,CCompany*>& arCompanies);
	int		GetCompaniesWithRecentSplit(const COleDateTime &date, CArray<CCompany*,CCompany*>& arCompanies);

	BOOL Save(const char* szFilePath);
	BOOL Load(const char* szFilePath);

	BOOL SaveLocalCopyInAppData();

	CMacroParameters*	GetMacroParametersPointer();
	CMacroParameters	GetMacroParameters();
	void SetMacroParameters(CMacroParameters &Macro);

protected:
	void	AddCompanyToIndexLists(CCompany* pCompany);
	void	DestroyIndexLists();
	void	SetupRankings();

	void	AddToIndustryGroup(CCompany* pCompany);
	void	AddToSectorGroup(CCompany* pCompany);
	void	AddToTypeGroup(CCompany* pCompany);

protected:
	CArray<CCompany*,CCompany*>		m_arCompanies;
	COleDateTime					m_Today;

	CListEx<CString>				m_ListByTicker;
	CListEx<CString>				m_ListByIndustry;
	CListEx<CString>				m_ListBySector;

	CListEx<int>					m_ListByType;

	CListEx<double>					m_ListByRankEPV;
	CListEx<double>					m_ListByRankRAV;
	CListEx<double>					m_ListByRankPMV;
	CListEx<double>					m_ListByRankEPVFore;
	CListEx<double>					m_ListByRankRAVFore;
	CListEx<double>					m_ListByRankPMVFore;
	CListEx<double>					m_ListByRankEPVRazor;
	CListEx<double>					m_ListByRankRAVRazor;
	CListEx<double>					m_ListByRankPMVRazor;
	CListEx<double>					m_ListByRankEPVRazorFore;
	CListEx<double>					m_ListByRankRAVRazorFore;
	CListEx<double>					m_ListByRankPMVRazorFore;

	CListEx<double>					m_ListByRankBEV;
	CListEx<double>					m_ListByRankGrowth1;
	CListEx<double>					m_ListByRankGrowth2;

	CListEx<double>					m_ListByRankMomentumActual;
	CListEx<double>					m_ListByRankMomentumRazor;
};

#endif // !defined(AFX_UNIVERSE_H__927806A0_5CCE_425C_94B3_537B56E25906__INCLUDED_)
