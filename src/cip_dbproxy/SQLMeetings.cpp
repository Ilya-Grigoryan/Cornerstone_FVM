// SQLMeetings.cpp: implementation of the CSQLMeetings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_dbproxy.h"
#include "SQLMeetings.h"

#include "SQLCompany.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_dbproxyApp	theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLMeetings::CSQLMeetings()
{

}

CSQLMeetings::~CSQLMeetings() 
{

}

int CSQLMeetings::UpdateMeeting(int nCompanyID, COleDateTime date, const char* szAnalystName, int nRecommend, int nDecision, const char* szDesc)
{
	char szSql[4*4096];
	int nResult;

	CString sDate = date.Format("%Y-%m-%d");
	char szTemplate[] = "INSERT INTO meetings SET "
							"id=DEFAULT, "
							"CompanyID = '%u', "
							"ItemDate = '%s', "
							"AnalystName='%s', "
							"Recommendation='%u', "
							"Decision='%u', "
							"Description='%s' "
						"ON DUPLICATE KEY UPDATE "
							"AnalystName='%s', "
							"Recommendation='%u', "
							"Decision='%u', "
							"Description='%s' ";
	
	sprintf(szSql, szTemplate, nCompanyID, sDate,	szAnalystName, nRecommend, nDecision, szDesc,
													szAnalystName, nRecommend, nDecision, szDesc);

	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	int nID = (unsigned int)mysql_insert_id(theApp.m_sql);
	return nID;
}

int CSQLMeetings::GetMeetings(int nCompanyID, COleDateTime from, COleDateTime to, int nRecommend, int nDecision, int nSort, SMeetingArray& arMeetings)
{
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sSql, sWhere, sOrder;

	// Filter
	if(nCompanyID != -1)
	{
		CString s; s.Format(" CompanyID = '%u' ", nCompanyID);
		sWhere += s;
	}

	if(from.GetStatus() == COleDateTime::valid)
	{
		CString s; s.Format(" ItemDate >= '%s' ", from.Format("%Y-%m-%d"));
		if(!sWhere.IsEmpty())
			sWhere += " AND ";
		sWhere += s;
	}

	if(to.GetStatus() == COleDateTime::valid)
	{
		CString s; s.Format(" ItemDate <= '%s' ", to.Format("%Y-%m-%d"));
		if(!sWhere.IsEmpty())
			sWhere += " AND ";
		sWhere += s;
	}

	if(nRecommend != -1)
	{
		CString s; s.Format(" Recommendation = '%u' ", nRecommend);
		if(!sWhere.IsEmpty())
			sWhere += " AND ";
		sWhere += s;
	}

	if(nDecision != -1)
	{
		CString s; s.Format(" Decision = '%u' ", nDecision);
		if(!sWhere.IsEmpty())
			sWhere += " AND ";
		sWhere += s;
	}

	if(sWhere.IsEmpty())
		sWhere += " 1 ";

	// Sort
	if(nSort == 0)
		sOrder = " ItemDate DESC, CompanyID ASC ";
	else
		sOrder = " CompanyID ASC, ItemDate DESC ";

	// SQL
	sSql.Format("SELECT * FROM meetings WHERE %s ORDER BY %s", sWhere, sOrder);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	while(row = mysql_fetch_row(result))
	{
		SMeeting* sm = new SMeeting;
		sm->id = atoi(row[0]);
		sm->nCompanyID = atoi(row[1]);
		sm->date = CSQLCompany::ConvertSqlToOleDate(row[2]);
		strcpy(sm->szAnalyst, row[3]);
		sm->nRecommendation = atoi(row[4]);
		sm->nDecision = atoi(row[5]);
		strcpy(sm->szDesc, row[6]);

		arMeetings.Add(sm);
	}

	mysql_free_result(result);
	return arMeetings.GetSize();
}

int CSQLMeetings::DeleteMeeting(int nID, BOOL bAlsoDocuments)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "DELETE FROM meetings WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}

	if(bAlsoDocuments)
	{
		sprintf(szSql, "DELETE FROM meetingdocs WHERE MeetingID='%u'", nID);
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			theApp.m_sError.Format(_T("MySql Error: %s"), err);
			return false;
		}
	}

	return true;
}

int CSQLMeetings::AddDocument(int nMeetingID, const char* szPath, const char* szDesc)
{
	// parse path
	char name[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(szPath, NULL, NULL, name, ext);

	char fname[_MAX_PATH];
	_makepath(fname, NULL, NULL, name, ext);

	// try to open the file
	CFile file;
	if(!file.Open(szPath, CFile::modeRead))
	{
		theApp.m_sError.Format("Unable to open file: '%s'", szPath);
		return -1;
	}

	// read the data
	unsigned int nLength = file.GetLength();
	char* pData = new char[nLength];
	if(file.Read(pData, nLength) != nLength)
	{
		theApp.m_sError.Format("Unable to read file data: '%s'", szPath);
		delete [] pData;
		return -2;
	}
	
	// prepare as a string (escape)
	char* pChunkData = new char[2*nLength+1];
	mysql_real_escape_string(theApp.m_sql, pChunkData, pData, nLength);

	char *szSql = new char[2*nLength+1 + 2048];
	int nResult;

	sprintf(szSql, "INSERT INTO meetingdocs (id, MeetingID, FileName, Data, Description) VALUES (DEFAULT, '%u', '%s', '%s', '%s')", nMeetingID, fname, pChunkData, szDesc);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		delete [] pData;
		delete [] pChunkData;
		delete [] szSql;
		return 0;
	}

	int nID = (int)mysql_insert_id(theApp.m_sql);

	delete [] pData;
	delete [] pChunkData;
	delete [] szSql;
	return nID;
}

CString	CSQLMeetings::GetDocument(int nID /* in */, const char* szFolder /* in */, char* szName /* out */, char* szDesc /* out */)
{
	// get the document
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int nRows;
	unsigned long* lengths;

	sprintf(szSql, "SELECT * FROM meetingdocs WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return "";
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows==0)
	{
		mysql_free_result(result);
		return "";
	}
	row = mysql_fetch_row(result);
	lengths = mysql_fetch_lengths(result);

	strcpy(szName, row[2] ? row[2] : "");
	strcpy(szDesc, row[4] ? row[4] : "");

	CString sPath = szFolder;
	sPath += szName;

	// open the file
	CFile file;
	if(!file.Open(sPath, CFile::modeCreate|CFile::modeWrite))
	{
		mysql_free_result(result);
		return "";
	}
	file.Write(row[3], lengths[3]);
	file.Close();

	mysql_free_result(result);
	return sPath;
}

int CSQLMeetings::GetDocumentsForMeeting(int nMeetingID, CArray<int,int>& arIDs, CStringArray& arNames)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;
	
	sprintf(szSql, "SELECT id, FileName FROM meetingdocs WHERE MeetingID='%u'", nMeetingID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	while(row = mysql_fetch_row(result))
	{
		int nID = atoi(row[0]);
		CString sName = row[1];
		arIDs.Add(nID);
		arNames.Add(sName);
	}
	mysql_free_result(result);
	return arIDs.GetSize();
}

BOOL CSQLMeetings::DeleteDocument(int nID)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "DELETE FROM meetingdocs WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}

	return true;
}
