// Universe.cpp: implementation of the CUniverse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "Universe.h"
#include "BaseMath.h"

#include "SQLCompany.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_coreApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUniverse::CUniverse()
{
	
}

CUniverse::~CUniverse()
{
	ClearAll();
}

//////////////////////////////////////////////////////////////////////
BOOL CUniverse::CreateFromRawData(SUniverseRawData& UniverseData)
{
	ClearAll();

	for(int i=0; i<UniverseData.GetSize(); i++)
	{
		CCompany* pCompany = new CCompany();
		if(!pCompany->CreateFromRawData(UniverseData[i]))
		{
			// handle error
			continue;
		}
		pCompany->SetUniverse(this);
		m_arCompanies.Add(pCompany);
	}

	return true;
}

void CUniverse::SetupRankings()
{
	int i;
	for(i=0; i<m_ListByRankEPV.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankEPV.GetItem(i)->m_pData;
		pCompany->SetRankEPV(i+1);
	}
	for(i=0; i<m_ListByRankRAV.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankRAV.GetItem(i)->m_pData;
		pCompany->SetRankRAV(i+1);
	}
	for(i=0; i<m_ListByRankPMV.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankPMV.GetItem(i)->m_pData;
		pCompany->SetRankPMV(i+1);
	}
	////////////////////////////////////////////////////////////////////////////
	for(i=0; i<m_ListByRankEPVFore.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankEPVFore.GetItem(i)->m_pData;
		pCompany->SetRankEPVFore(i+1);
	}
	for(i=0; i<m_ListByRankRAVFore.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankRAVFore.GetItem(i)->m_pData;
		pCompany->SetRankRAVFore(i+1);
	}
	for(i=0; i<m_ListByRankPMVFore.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankPMVFore.GetItem(i)->m_pData;
		pCompany->SetRankPMVFore(i+1);
	}
	////////////////////////////////////////////////////////////////////////////
	for(i=0; i<m_ListByRankEPVRazor.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankEPVRazor.GetItem(i)->m_pData;
		pCompany->SetRankEPVRazor(i+1);
	}
	for(i=0; i<m_ListByRankRAVRazor.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankRAVRazor.GetItem(i)->m_pData;
		pCompany->SetRankRAVRazor(i+1);
	}
	for(i=0; i<m_ListByRankPMVRazor.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankPMVRazor.GetItem(i)->m_pData;
		pCompany->SetRankPMVRazor(i+1);
	}
	////////////////////////////////////////////////////////////////////////////
	for(i=0; i<m_ListByRankEPVRazorFore.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankEPVRazorFore.GetItem(i)->m_pData;
		pCompany->SetRankEPVRazorFore(i+1);
	}
	for(i=0; i<m_ListByRankRAVRazorFore.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankRAVRazorFore.GetItem(i)->m_pData;
		pCompany->SetRankRAVRazorFore(i+1);
	}
	for(i=0; i<m_ListByRankPMVRazorFore.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankPMVRazorFore.GetItem(i)->m_pData;
		pCompany->SetRankPMVRazorFore(i+1);
	}
	///////////////////////////////////////////////////////////////////////////////
	for(i=0; i<m_ListByRankBEV.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankBEV.GetItem(i)->m_pData;
		pCompany->SetRankBEV(i+1);
	}
	for(i=0; i<m_ListByRankGrowth1.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankGrowth1.GetItem(i)->m_pData;
		pCompany->SetRankGrowth1(m_arCompanies.GetSize()-i);
	}
	for(i=0; i<m_ListByRankGrowth2.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankGrowth2.GetItem(i)->m_pData;
		pCompany->SetRankGrowth2(m_arCompanies.GetSize()-i);
	}
	////////////////////////////////////////////////////////////////////////////////
	for(i=0; i<m_ListByRankMomentumActual.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankMomentumActual.GetItem(i)->m_pData;
		pCompany->SetRankMomentumActual(m_arCompanies.GetSize()-i);
	}
	for(i=0; i<m_ListByRankMomentumRazor.m_ItemData.GetSize(); i++)
	{
		CCompany* pCompany = (CCompany*)m_ListByRankMomentumRazor.GetItem(i)->m_pData;
		pCompany->SetRankMomentumRazor(m_arCompanies.GetSize()-i);
	}
}

void CUniverse::AddToIndustryGroup(CCompany* pCompany)
{
	int nIndex = m_ListByIndustry.Lookup(pCompany->GetIndustry());
	if(nIndex != -1)
	{
		CListExItem<CString>* pItem = m_ListByIndustry.GetItem(nIndex);
		CListEx<CString>* pComps = (CListEx<CString>*)pItem->m_pData;

		pComps->Add(pCompany->GetTicker(), pCompany);
	}
	else
	{
		CListEx<CString>* pComps = new CListEx<CString>;
		m_ListByIndustry.Add(pCompany->GetIndustry(), pComps);
		
		pComps->Add(pCompany->GetTicker(), pCompany);
	}
}

void CUniverse::AddToSectorGroup(CCompany* pCompany)
{
	int nIndex = m_ListBySector.Lookup(pCompany->GetSector());
	if(nIndex != -1)
	{
		CListExItem<CString>* pItem = m_ListBySector.GetItem(nIndex);
		CListEx<CString>* pComps = (CListEx<CString>*)pItem->m_pData;

		pComps->Add(pCompany->GetTicker(), pCompany);
	}
	else
	{
		CListEx<CString>* pComps = new CListEx<CString>;
		m_ListBySector.Add(pCompany->GetSector(), pComps);
		
		pComps->Add(pCompany->GetTicker(), pCompany);
	}
}

void CUniverse::AddToTypeGroup(CCompany* pCompany)
{
	int nIndex = m_ListByType.Lookup(pCompany->GetType());
	if(nIndex != -1)
	{
		CListExItem<int>* pItem = m_ListByType.GetItem(nIndex);
		CListEx<CString>* pComps = (CListEx<CString>*)pItem->m_pData;

		pComps->Add(pCompany->GetTicker(), pCompany);
	}
	else
	{
		CListEx<CString>* pComps = new CListEx<CString>;
		m_ListByType.Add(pCompany->GetType(), pComps);
		
		pComps->Add(pCompany->GetTicker(), pCompany);
	}
}

void CUniverse::AddCompanyToIndexLists(CCompany* pCompany)
{
	m_ListByTicker.Add(pCompany->GetTicker(), (void*)pCompany);	

	AddToIndustryGroup(pCompany);
	AddToSectorGroup(pCompany);
	AddToTypeGroup(pCompany);

	// rankings
	m_ListByRankEPV.Add(pCompany->GetPriceOverEPV(), (void*)pCompany);
	m_ListByRankRAV.Add(pCompany->GetPriceOverRAV(), (void*)pCompany);
	m_ListByRankPMV.Add(pCompany->GetPriceOverPMV(), (void*)pCompany);
	m_ListByRankEPVFore.Add(pCompany->GetPriceOverEPVFore(), (void*)pCompany);
	m_ListByRankRAVFore.Add(pCompany->GetPriceOverRAVFore(), (void*)pCompany);
	m_ListByRankPMVFore.Add(pCompany->GetPriceOverPMVFore(), (void*)pCompany);
	m_ListByRankEPVRazor.Add(pCompany->GetPriceOverEPVRazor(), (void*)pCompany);
	m_ListByRankRAVRazor.Add(pCompany->GetPriceOverRAVRazor(), (void*)pCompany);
	m_ListByRankPMVRazor.Add(pCompany->GetPriceOverPMVRazor(), (void*)pCompany);
	m_ListByRankEPVRazorFore.Add(pCompany->GetPriceOverEPVRazorFore(), (void*)pCompany);
	m_ListByRankRAVRazorFore.Add(pCompany->GetPriceOverRAVRazorFore(), (void*)pCompany);
	m_ListByRankPMVRazorFore.Add(pCompany->GetPriceOverPMVRazorFore(), (void*)pCompany);

	m_ListByRankBEV.Add(pCompany->GetPriceOverBEV(), (void*)pCompany);
	m_ListByRankGrowth1.Add(pCompany->GetGrowthEstimate1(), (void*)pCompany);
	m_ListByRankGrowth2.Add(pCompany->GetGrowthEstimate2(), (void*)pCompany);

	m_ListByRankMomentumActual.Add(pCompany->GetMomentumActual(), (void*)pCompany);
	m_ListByRankMomentumRazor.Add(pCompany->GetMomentumRazor(), (void*)pCompany);
}

void CUniverse::DestroyIndexLists()
{
	m_ListByTicker.RemoveAll();
	m_ListByRankEPV.RemoveAll();
	m_ListByRankRAV.RemoveAll();
	m_ListByRankPMV.RemoveAll();
	m_ListByRankEPVFore.RemoveAll();
	m_ListByRankRAVFore.RemoveAll();
	m_ListByRankPMVFore.RemoveAll();
	m_ListByRankEPVRazor.RemoveAll();
	m_ListByRankRAVRazor.RemoveAll();
	m_ListByRankPMVRazor.RemoveAll();
	m_ListByRankEPVRazorFore.RemoveAll();
	m_ListByRankRAVRazorFore.RemoveAll();
	m_ListByRankPMVRazorFore.RemoveAll();

	for(int i=0; i<m_ListByIndustry.GetSize(); i++)
	{
		( (CListEx<CString>*)(m_ListByIndustry.GetItem(i)->m_pData) )->RemoveAll();
		delete ( (CListEx<CString>*)(m_ListByIndustry.GetItem(i)->m_pData) );
	}

	for(int i=0; i<m_ListBySector.GetSize(); i++)
	{
		( (CListEx<CString>*)(m_ListBySector.GetItem(i)->m_pData) )->RemoveAll();
		delete ( (CListEx<CString>*)(m_ListBySector.GetItem(i)->m_pData) );
	}

	for(int i=0; i<m_ListByType.GetSize(); i++)
	{
		( (CListEx<CString>*)(m_ListByType.GetItem(i)->m_pData) )->RemoveAll();
		delete ( (CListEx<CString>*)(m_ListByType.GetItem(i)->m_pData) );
	}

	m_ListByIndustry.RemoveAll();
	m_ListBySector.RemoveAll();
	m_ListByType.RemoveAll();

	m_ListByRankBEV.RemoveAll();
	m_ListByRankGrowth1.RemoveAll();
	m_ListByRankGrowth2.RemoveAll();

	m_ListByRankMomentumActual.RemoveAll();
	m_ListByRankMomentumRazor.RemoveAll();
}

void CUniverse::CalcAll()
{
	DestroyIndexLists();

	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];

		m_arCompanies[i]->CalcAll();
		AddCompanyToIndexLists(pCompany); 
	}

	SetupRankings();
} 

void CUniverse::RecalcRankings()
{
	DestroyIndexLists();

	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];
		AddCompanyToIndexLists(pCompany); 
	}

	SetupRankings();
}

#include "Markup.h"
void CUniverse::LoadAll(int nUniverse, int nYear, COleDateTime dtPrices)
{
	ClearAll();

	// load discount rate
	double fDiscount = CSQLCompany::GetInterestRate(dtPrices);
	theApp.m_Macro.SetDiscountRate(fDiscount/100.0);

	// load data
	SUniverseRawData UniverseData;
	CSQLCompany::GetUniverseRawData(nUniverse, nYear, dtPrices, UniverseData);
	SetDate(dtPrices);
	CreateFromRawData(UniverseData);
	CalcAll();

	// Pre-Load Histories, i.e. Load dates
	for(int i=0; i<GetSize(); i++)
		GetCompany(i)->PreLoadHistory();
}

void CUniverse::ClearAll()
{
	for(int i=m_arCompanies.GetUpperBound(); i>=0; i--)
		delete m_arCompanies[i];
	m_arCompanies.RemoveAll();

	DestroyIndexLists();
}

int	CUniverse::GetCompaniesWithRollOverYear(CArray<CCompany*,CCompany*>& arCompanies)
{
	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];
		if(	pCompany->GetRecentRollOver() ||
			pCompany->GetROC(ROCEPS_HYTORY_SIZE-1) >= _INF_	||
			pCompany->GetROE(ROCEPS_HYTORY_SIZE-1) >= _INF_	||
			pCompany->GetEPS(ROCEPS_HYTORY_SIZE-3) >= _INF_)

			arCompanies.Add(pCompany);
	}

	return arCompanies.GetSize();
}

int	CUniverse::GetCompaniesWithNoSharesOut(CArray<CCompany*,CCompany*>& arCompanies)
{
	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];

		if(pCompany->GetSharesOut() >= _INF_ || pCompany->GetSharesOut() <= 0)
			arCompanies.Add(pCompany);
	}

	return arCompanies.GetSize();
}

int CUniverse::GetCompaniesWithSP500Controversy(CArray<CCompany*,CCompany*>& arCompanies)
{
	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];

		if(!pCompany->GetInSP500() && pCompany->GetWeightInIndex(3) > 0)
			arCompanies.Add(pCompany);
	}

	return arCompanies.GetSize();
}

int CUniverse::GetCompaniesWithRecentSplit(const COleDateTime &date, CArray<CCompany*,CCompany*>& arCompanies)
{
	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];

		if(pCompany->CheckForRecentSplit(date))
			arCompanies.Add(pCompany);
	}

	return arCompanies.GetSize();
}

int CUniverse::GetIndexByTicker(const char* szTicker, BOOL bExact)
{
	int nIndex;
	if(bExact)
		nIndex = m_ListByTicker.Lookup(szTicker);
	else
		nIndex = m_ListByTicker.LookupEx(szTicker);

	return nIndex;
}

void CUniverse::GetCompaniesInIndustry(const char* szIndustry, CArray<CCompany*, CCompany*> &arResult)
{
	int nIndex = m_ListByIndustry.Lookup(szIndustry);
	if(nIndex == -1)
		return;

	CListExItem<CString>* pItem = m_ListByIndustry.GetItem(nIndex);

	for(int i=0; i < ((CListEx<CString>*)pItem->m_pData)->GetSize(); i++)
	{
		CListExItem<CString>* pI = (CListExItem<CString>*) ((CListEx<CString>*)pItem->m_pData)->GetItem(i);
		arResult.Add( (CCompany*) pI->m_pData );
	}
}

void CUniverse::GetCompaniesInSector(const char* szSector, CArray<CCompany*, CCompany*> &arResult)
{
	int nIndex = m_ListBySector.Lookup(szSector);
	if(nIndex == -1)
		return;

	CListExItem<CString>* pItem = m_ListBySector.GetItem(nIndex);

	for(int i=0; i < ((CListEx<CString>*)pItem->m_pData)->GetSize(); i++)
	{
		CListExItem<CString>* pI = (CListExItem<CString>*) ((CListEx<CString>*)pItem->m_pData)->GetItem(i);
		arResult.Add( (CCompany*) pI->m_pData );
	}
}

int CUniverse::GetCompaniesInType(int nType, CArray<CCompany*,CCompany*> &arResult)
{
	int nIndex = m_ListByType.Lookup(nType);
	if(nIndex == -1)
		return 0;

	CListExItem<int>* pItem = m_ListByType.GetItem(nIndex);

	for(int i=0; i < ((CListEx<CString>*)pItem->m_pData)->GetSize(); i++)
	{
		CListExItem<CString>* pI = (CListExItem<CString>*) ((CListEx<CString>*)pItem->m_pData)->GetItem(i);
		arResult.Add( (CCompany*) pI->m_pData );
	}

	return arResult.GetSize();
}

int	CUniverse::GetCompaniesInIndex(const char* szIndex, CArray<CCompany*,CCompany*> &arResult)
{
	// special case for the whole universe szIndex = "@ALL"
	if(stricmp(szIndex, "@ALL")==0)
	{
		for(int i=0; i<m_arCompanies.GetSize(); i++)
		{
			CCompany* pCompany = m_arCompanies[i];
			arResult.Add(pCompany);
		}
		return -100;
	}

	int i, nIndex = -1;
	for(i=0; i<NUM_WEIGHTS; i++)
		if(stricmp(szIndex, WeightsIndices[i])==0)
		{
			nIndex = i;
			break;
		}
	if(nIndex == -1)
		return -1;

	for(i=0; i<m_arCompanies.GetSize(); i++)
	{
		CCompany* pCompany = m_arCompanies[i];
		double fWeight = pCompany->GetWeightInIndex(nIndex);
		if(fWeight > 0)
			arResult.Add(pCompany);
	}

	return nIndex;
}

double CUniverse::GetSectorNEPVRank(const char* szSector, int nAveType)
{
	double fRet = 0;
	CArray<CCompany*,CCompany*>	arCompanies;
	GetCompaniesInSector(szSector, arCompanies);
	int nCount = arCompanies.GetSize();

	for(int i=0; i<nCount; i++)
	{
		CCompany* pCompany = arCompanies[i];
		fRet += pCompany->GetRankEPV();
	}

	fRet = (double)( fRet / (double)nCount );
	return fRet;
}

double CUniverse::GetSectorREPVRank(const char* szSector, int nAveType)
{
	double fRet = 0;
	CArray<CCompany*,CCompany*>	arCompanies;
	GetCompaniesInSector(szSector, arCompanies);
	int nCount = arCompanies.GetSize();

	for(int i=0; i<nCount; i++)
	{
		CCompany* pCompany = arCompanies[i];
		fRet += pCompany->GetRankEPVRazor();
	}

	fRet = (double)( fRet / (double)nCount );
	return fRet;
}

int CUniverse::RemoveCompany(CCompany* pCompany)
{
	int nIndex = -1;
	for(int i=0; i<m_arCompanies.GetSize(); i++)
	{
		if(pCompany == m_arCompanies[i])
		{
			nIndex = i;
			break;
		}
	}
	if(nIndex == -1)
		return -1;

	m_arCompanies.RemoveAt(nIndex);
	return nIndex;
}

BOOL CUniverse::SaveLocalCopyInAppData()
{
	/////////////////////////////////
	// create target folder
	/////////////////////////////////
	TCHAR szAppDataPath[_MAX_PATH];
	SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_APPDATA, false);

	BOOL bRet;
	CString sDir; 
	
	sDir.Format("%s\\Cornerstone IP", szAppDataPath);
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return false;

	sDir += "\\Fair Value Model";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return false;

	sDir += "\\Stock Data";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return false;

	// Save the files
	for(int i=0; i<m_arCompanies.GetSize(); i++)
		m_arCompanies[i]->SaveLocalCopyInAppData();

	return true;
}

BOOL CUniverse::Save(const char* szFilePath)
{
	CFile file;
	if(!file.Open(szFilePath, CFile::modeCreate|CFile::modeWrite))
	{
		CString sErr; sErr.Format("Unable to create file: '%s'", szFilePath);
		AfxMessageBox(sErr, MB_ICONSTOP);
		return false;
	}

	CArchive ar(&file, CArchive::store);

	// write version
	ar << (DWORD)_UNIVERSE_VERSION_;

	int nSize = m_arCompanies.GetSize();
	ar << nSize;
	for(int i=0; i<nSize; i++)
		if(!m_arCompanies[i]->Save(ar))
		{
			ar.Close();
			file.Close();
			return false;
		}

	return true;
}

BOOL CUniverse::Load(const char* szFilePath)
{
	CFile file;
	if(!file.Open(szFilePath, CFile::modeRead))
	{
		CString sErr; sErr.Format("Unable to open file: '%s'.", szFilePath);
		AfxMessageBox(sErr, MB_ICONSTOP);
		return false;
	}

	CArchive ar(&file, CArchive::load);

	// read version
	DWORD dwVersion;
	ar >> (DWORD)dwVersion;
	if(dwVersion != _UNIVERSE_VERSION_)
	{
		AfxMessageBox("Wrong file version.", MB_ICONSTOP);
		return false;
	}

	ClearAll();

	int nSize;
	ar >> nSize;
	for(int i=0; i<nSize; i++)
	{
		CCompany* pCompany = new CCompany;
		if(!pCompany->Load(ar))
		{
			delete pCompany;
			ClearAll();

			ar.Close();
			file.Close();
			return false;
		}
		pCompany->SetUniverse(this);
	}

	CalcAll();
	return true;
}

/////////////////////////////////////////////////////////////////////
CMacroParameters* CUniverse::GetMacroParametersPointer()
{
	return &theApp.m_Macro;
}

CMacroParameters CUniverse::GetMacroParameters()
{
	return theApp.m_Macro;
}

void CUniverse::SetMacroParameters(CMacroParameters &Macro)
{
	theApp.m_Macro = Macro;
}