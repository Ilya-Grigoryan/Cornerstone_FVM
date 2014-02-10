// MetaData.h: interface for the CMetaData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METADATA_H__D6D83AC9_5793_4DE5_B1E1_3B9AAAAE2A79__INCLUDED_)
#define AFX_METADATA_H__D6D83AC9_5793_4DE5_B1E1_3B9AAAAE2A79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basemath.h"
#include "listex.h"
#include "input.h"

#include <afxinet.h>

class CMetaData;

//////////////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMetaDataItem
{
public:
	CMetaDataItem(CMetaData* pData=NULL);
	CMetaDataItem(const char* szTicker, CMetaData* pData=NULL);
	CMetaDataItem(const char* szTicker, int nSize, CMetaData* pData=NULL);
	CMetaDataItem(const char* szTicker, const CStringArray& arColumns, CMetaData* pData=NULL);
	~CMetaDataItem();

public:
	const char* GetTicker()		const					{ return m_sTicker;				}
	COleDateTime	GetDate()	const					{ return m_Date;				}

public:
	void		SetColName(int n, const char* szName)	{ m_arColNames[n] = szName;		}
	const char* GetColName(int n)						{ return m_arColNames[n];		}

	double operator[](const char* szItem);
	double operator[](int n)							{ return m_pData[n];			}

public:
	CString			m_sTicker;
	COleDateTime	m_Date;

	CStringArray m_arColNames;
	int		m_nSize;
	double*	m_pData;

	CMetaData* m_pMetaData;
};
typedef CArray<CMetaDataItem*,CMetaDataItem*> CMetaDataItemArray;

//////////////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMetaData  
{
public:
	CMetaData();
	~CMetaData();

public:
	BOOL ConnectToFactSet();
	void DisconnectFromFactSet();

	BOOL GetXMLDataFromFactSet(	const CStringArray& arTickers,
								const COleDateTime& dtStart, const COleDateTime &dtEnd,
								char chPeriod,
								const CStringArray& arItems,
								char* &szDataXML);
	BOOL LoadFromXML(const char* szXML);

	BOOL GetFundamentalsFromFS(const char* szTicker, SCompanyDataFundamentals& data);

public:
	int	 GetTickersCount() const	{ return m_ListByTickers.m_ItemData.GetSize();	}
	int	 GetDatesCount() const		{ return m_ListByDates.m_ItemData.GetSize();	}

	CListEx<CString>* GetUniverseSnapshot(const COleDateTime& date, BOOL bExactDate=false);
	CListEx<COleDateTime>*  GetSeriesForTicker(const char* szTicker);

protected:   
	void AddItemToListDates(const COleDateTime& date, const CMetaDataItem* pItem);
	void AddItemToListTickers(const char* szTicker, const CMetaDataItem* pItem);

	void RemoveAllItems();
	void ClearMappingLists();


protected:
	CArray<CMetaDataItem*, CMetaDataItem*>	m_arItems;
	CListEx<COleDateTime>					m_ListByDates;
	CListEx<CString>						m_ListByTickers;

	CInternetSession						m_Session;		// The main session - connecting to primary server and setting up parameters.
	CHttpConnection*						m_pServer;		// There can be more than one connections for each session. 
};

#endif // !defined(AFX_METADATA_H__D6D83AC9_5793_4DE5_B1E1_3B9AAAAE2A79__INCLUDED_)
