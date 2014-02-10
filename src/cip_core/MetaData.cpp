// MetaData.cpp: implementation of the CMetaData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "MetaData.h"

#include "markup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMetaDataItem::CMetaDataItem(CMetaData* pData)
{
	m_nSize = 0;
	m_pData = NULL;
	m_pMetaData = pData;
}

CMetaDataItem::CMetaDataItem(const char* szTicker, CMetaData* pData)
{
	m_sTicker = szTicker;
	m_nSize = 0;
	m_pData = NULL;
	m_pMetaData = pData;
}

CMetaDataItem::CMetaDataItem(const char* szTicker, const CStringArray& arNames, CMetaData* pData)
{
	m_sTicker = szTicker;

	m_nSize = arNames.GetSize();
	for(int i=0; i<m_nSize; i++)
		m_arColNames.Add(arNames[i]);

	m_pData = new double[m_nSize];

	m_pMetaData = pData;
}

CMetaDataItem::CMetaDataItem(const char* szTicker, int nSize, CMetaData* pData)
{
	m_sTicker = szTicker;

	m_nSize = nSize;
	m_pData = new double[m_nSize];

	for(int i=0; i<m_nSize; i++)
	{
		CString sItem;
		sItem.Format("Item %u", i);
		m_arColNames.Add(sItem);
	}

	m_pMetaData = pData;
}

CMetaDataItem::~CMetaDataItem()
{
	if(m_pData)
	{
		delete m_pData;
		m_pData = NULL;
	}
	m_nSize = 0;
	m_arColNames.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
CMetaData::CMetaData()
{
	m_pServer = NULL;
}

CMetaData::~CMetaData()
{
	RemoveAllItems();
	ClearMappingLists();

	DisconnectFromFactSet();
}

// 

BOOL CMetaData::ConnectToFactSet()
{
	m_Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000);
	m_Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3);
	
	char lpstrServer[] = "datadirect.factset.com";
	char pstrUserName[] = "CORNER_STONE_290495_SERVICES";
	char pstrPassword[] = "xhWJ5kFq8B2Hv7Rs";
	int usPort = 443; //INTERNET_INVALID_PORT_NUMBER;  
	
	if(!m_pServer)
		m_pServer = m_Session.GetHttpConnection(lpstrServer, usPort, pstrUserName, pstrPassword);

	return (m_pServer != NULL);
}

void CMetaData::DisconnectFromFactSet()
{
	if(m_pServer)
	{
		delete m_pServer;
		m_pServer = NULL;
	}

	m_Session.Close();
}

BOOL CMetaData::GetXMLDataFromFactSet(	const CStringArray& arTickers,
										const COleDateTime& dtStart, const COleDateTime &dtEnd,
										char chPeriod,
										const CStringArray& arItems,
										char* &szDataXML)
{
	BOOL bRet = false;
	const int FILEBUFLEN = 1024;
    char httpBuff[FILEBUFLEN];
    TCHAR szCause[255];

	CString sIds;
	for(int i=0; i<arTickers.GetSize(); i++)
	{
		if(i>0)
			sIds += ",";
		sIds += arTickers[i];
	}

	CString sDates1; sDates1.Format("%s:%s:%c", dtStart.Format("%Y%m%d"),dtEnd.Format("%Y%m%d"), chPeriod);
	CString sDates2; sDates2.Format("%s,%s,%c", dtStart.Format("%Y%m%d"),dtEnd.Format("%Y%m%d"), chPeriod);
	CString sItems;
	for(int i=0; i<arItems.GetSize(); i++)
	{
		if(i>0)
			sItems += ",";
		CString sItem; 

		int nFind = arItems[i].Find(",");
		if(nFind == -1)
			sItem.Format("%s(%s)", arItems[i], sDates2);
		else
			sItem.Format("%s(%s,%s)", arItems[i].Left(nFind), arItems[i].Mid(nFind+1), sDates2);
		sItems += sItem;
	}

	CString sFactSetQuery;
	sFactSetQuery.Format(
		"/services/fastfetch?factlet=ExtractFormulaHistory&ids=%s&dates=%s&items=%s",
		sIds, sDates1, sItems);

    TRY {
 /*       CInternetSession session;
        session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000);
        session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3);

		char lpstrServer[] = "datadirect.factset.com";
		char pstrUserName[] = "CORNER_STONE_290495_SERVICES";
		char pstrPassword[] = "xhWJ5kFq8B2Hv7Rs";
		int usPort = 443; //INTERNET_INVALID_PORT_NUMBER;
		unsigned long dwRet;
		
		CHttpConnection* pServer = session.GetHttpConnection(lpstrServer, usPort, pstrUserName, pstrPassword);
*/
		
		unsigned long dwRet;
		CHttpFile* pFile = m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, 
							sFactSetQuery,
							NULL, 1, NULL, NULL, INTERNET_FLAG_SECURE |
							INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
							INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);

		pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);
		
		if(dwRet == HTTP_STATUS_OK)
		{
			CMemFile localFile;
			int numBytes;
			while (numBytes = pFile->Read(httpBuff, FILEBUFLEN)) {
				localFile.Write(httpBuff, numBytes);
			}
			
			szDataXML = (char*)localFile.Detach();
			bRet = true;
		}
		
		pFile->Close();
		delete pFile;

//		delete pServer;
//		session.Close();
        
    }
    CATCH_ALL(error) {
        error->GetErrorMessage(szCause,254,NULL);
        return FALSE;
    }
    END_CATCH_ALL;
    return bRet;
}

BOOL CMetaData::LoadFromXML(const char* szXML)
{
	CMarkup xml;
	xml.SetDoc(szXML);
	
	BOOL b = xml.FindElem("FASTFETCH");
	xml.IntoElem(); 
	b = xml.FindElem("RESPONSE");
	xml.IntoElem(); 
	b = xml.FindElem("FACTLET");
	xml.IntoElem(); 
	b = xml.FindElem("OUTPUT");
	xml.IntoElem(); 

	// get column names
	CStringArray arColumns;
	if(!xml.FindElem("COLUMNS")) return false;
	xml.IntoElem();
	
	// we don't need first two  - id and date
	xml.FindElem("COLUMN"); xml.FindElem("COLUMN");
	while ( xml.FindElem("COLUMN") )
		arColumns.Add(xml.GetData());
	xml.OutOfElem();
	int nColumns = arColumns.GetSize();

	// body - actual data
	b = xml.FindElem("BODY");
	int nRows = atoi(xml.GetAttrib("rows"));
	xml.IntoElem(); 
	while ( xml.FindElem("ROW") )
	{
		xml.IntoElem();
		
		xml.FindElem( "CELL" );
		CString sTicker = xml.GetData();

		xml.FindElem( "CELL" );
		CString sDate = xml.GetData();
		int nYear = atoi(sDate.Left(4));
		int nMonth = atoi(sDate.Mid(4, 2));
		int nDay = atoi(sDate.Mid(6));
		COleDateTime date(nYear, nMonth, nDay, 0,0,0);

		int nData = 0;
		CMetaDataItem *pItem = new CMetaDataItem(sTicker, arColumns, this);
		while ( xml.FindElem("CELL") )
		{
			CString s = xml.GetData();
			pItem->m_pData[nData++] = atof(s);
		}

		xml.OutOfElem();

		m_arItems.Add(pItem);
		AddItemToListDates(date, pItem);
		AddItemToListTickers(sTicker, pItem);
	}

	return true;
}

void CMetaData::AddItemToListDates(const COleDateTime& date, const CMetaDataItem* pItem)
{
	CListEx<CString>* pList = NULL;
	int nIndex = m_ListByDates.Lookup(date);
	
	if(nIndex == -1)
	{
		pList = new CListEx<CString>;
		m_ListByDates.Add(date, (void*)pList);
	}
	else
		pList = (CListEx<CString>*)m_ListByDates.m_ItemData[nIndex]->m_pData;
	
	pList->Add(pItem->GetTicker(), (void*)pItem);
}

void CMetaData::AddItemToListTickers(const char* szTicker, const CMetaDataItem* pItem)
{
	CListEx<COleDateTime>* pList = NULL;
	int nIndex = m_ListByTickers.Lookup(szTicker);

	if(nIndex == -1)
	{
		pList = new CListEx<COleDateTime>;
		m_ListByTickers.Add(szTicker, (void*)pList);
	}
	else
		pList = (CListEx<COleDateTime>*)m_ListByTickers.m_ItemData[nIndex]->m_pData;

	pList->Add(pItem->GetDate(), (void*)pItem);
}

void CMetaData::ClearMappingLists()
{
	// delete the list of lists (RemoveAll delete the list item objects but doesn't touch the objects inside)
	for(int i=0; i<m_ListByDates.m_ItemData.GetSize(); i++)
	{
		CListEx<CString>* pList = (CListEx<CString>*)m_ListByDates.m_ItemData[i]->m_pData;
		pList->RemoveAll();
		delete pList;
	}
	m_ListByDates.RemoveAll();

	for(int i=0; i<m_ListByTickers.m_ItemData.GetSize(); i++) 
	{
		CListEx<COleDateTime>* pList = (CListEx<COleDateTime>*)m_ListByTickers.m_ItemData[i]->m_pData;
		pList->RemoveAll();
		delete pList;
	}
	m_ListByTickers.RemoveAll();
}

void CMetaData::RemoveAllItems()
{
	for(int i=0; i<m_arItems.GetSize(); i++)
		delete m_arItems[i];
	m_arItems.RemoveAll();
}

CListEx<CString>* CMetaData::GetUniverseSnapshot(const COleDateTime& date, BOOL bExactDate) 
{
	int nIndex = -1;
	if(bExactDate)
		nIndex = m_ListByDates.Lookup(date);
	else
		nIndex = m_ListByDates.LookupEx(date);

	if(nIndex == -1)
		return NULL;

	CListEx<CString>* pList = (CListEx<CString>*)m_ListByDates.m_ItemData[nIndex]->m_pData;
	return pList;
}

CListEx<COleDateTime>*  CMetaData::GetSeriesForTicker(const char* szTicker)
{
	int nIndex = m_ListByTickers.Lookup(szTicker);
	if(nIndex == -1)
		return NULL;

	CListEx<COleDateTime>* pList = (CListEx<COleDateTime>*)m_ListByTickers.m_ItemData[nIndex]->m_pData;
	return pList;

}

BOOL CMetaData::GetFundamentalsFromFS(const char* szTicker, SCompanyDataFundamentals& data)
{
	if(!m_pServer)
		return false;

	const int FILEBUFLEN = 1024;
    char httpBuff[FILEBUFLEN];

	CString sQuery = "/services/fastfetch?factlet=ExtractDataSnapshot&ids=";
	sQuery += szTicker;
	sQuery += "&items=FF_FREQ_CODE(ANN,0),FG_COMPANY_NAME,FF_FISCAL_DATE(qtr,0,,,,),FF_FISCAL_DATE(semi,0,,,,),FF_FISCAL_DATE(ann,0,,,,),p_com_shs_out,ff_ppe_net(qtr,0,,,rf),ff_dep_amort_exp(ltm,0,,,rf),ff_dep_amort_exp(ltm_semi,0,,,rf),FF_ASSETS(QTR,0),,FF_ASSETS(semi,0),,FF_ASSETS(ann,0),ff_intang(qtr,0,,,rf),fg_int_exp_l";

	unsigned long dwRet;
	CHttpFile* pFile = m_pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, 
						sQuery,
						NULL, 1, NULL, NULL, INTERNET_FLAG_SECURE |
						INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
						INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);

	pFile->SendRequest();
	pFile->QueryInfoStatusCode(dwRet);
	
	if(dwRet != HTTP_STATUS_OK)
		return false;

	CMemFile localFile;
	int numBytes;
	while (numBytes = pFile->Read(httpBuff, FILEBUFLEN)) {
		localFile.Write(httpBuff, numBytes);
	}
		
	char* szXML = (char*)localFile.Detach();
	pFile->Close();
	delete pFile;

	//////////////////////////////////////////////////////////////////////////
	CMarkup xml;
	xml.SetDoc(szXML);
	delete szXML;
	
	if(!xml.FindElem("FASTFETCH"))	return false;
	xml.IntoElem(); 
	if(!xml.FindElem("RESPONSE")) return false;
	xml.IntoElem(); 
	if(!xml.FindElem("FACTLET")) return false;
	xml.IntoElem(); 
	if(!xml.FindElem("OUTPUT")) return false;
	xml.IntoElem(); 

	// body - actual data
	if(!xml.FindElem("BODY")) return false;
	int nRows = atoi(xml.GetAttrib("rows"));
	xml.IntoElem(); 

	while(xml.FindElem("ROW"))
	{
		xml.IntoElem();

		CString s;
		
		// Ticker 
		xml.FindElem( "CELL" );
		CString sTicker = xml.GetData();

		// second CELL - Today's date - skip
		xml.FindElem( "CELL" );

		// Frequency of Reporting
		xml.FindElem( "CELL" );
		s = xml.GetData();
		int nFreq = atoi(s);

		xml.FindElem( "CELL" );
		s = xml.GetData();
		strcpy(data.sName, s);




		xml.OutOfElem();
	}
	
	return true;
}