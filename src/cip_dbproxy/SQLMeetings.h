// SQLMeetings.h: interface for the CSQLMeetings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLMEETINGS_H__E31BA326_889C_4AC5_90B4_43B18D5D2CFF__INCLUDED_)
#define AFX_SQLMEETINGS_H__E31BA326_889C_4AC5_90B4_43B18D5D2CFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "input.h"
#include "basemath.h"

////////////////////////////////////////////////////////////////////////////////
struct SMeeting
{
	int				id;
	int				nCompanyID;
	COleDateTime	date;
	char			szAnalyst[64];
	int				nRecommendation;
	int				nDecision;
	char			szDesc[512];
};
typedef CArray<SMeeting*,SMeeting*>	SMeetingArray;

////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CSQLMeetings  
{
public:
	CSQLMeetings();
	~CSQLMeetings();

public:
	static int		UpdateMeeting(int nCompanyID, COleDateTime date, const char* szAnalystName, int nRecommend, int nDecision, const char* szDesc);
	static int		GetMeetings(int nCompanyID, COleDateTime from, COleDateTime to, int nRecommend, int nDecision, int nSort, SMeetingArray& arMeetings);
	static int		DeleteMeeting(int nID, BOOL bAlsoDocuments);

	// Document handling
	static int		AddDocument(int nMeetingID, const char* szPath, const char* szDesc);
	static CString	GetDocument(int nID /* in */, const char* szFolder /* in */, char* szName /* out */, char* szDesc /* out */);
	static BOOL		DeleteDocument(int nID);
	static int		GetDocumentsForMeeting(int nMeetingID, CArray<int,int>& arIDs, CStringArray& arNames);

};

#endif // !defined(AFX_SQLMEETINGS_H__E31BA326_889C_4AC5_90B4_43B18D5D2CFF__INCLUDED_)
