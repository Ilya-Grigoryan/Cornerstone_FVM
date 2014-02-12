// Json.h: interface for the CJson class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JSON_H__B17ABB5B_C5E3_4C8E_A5AC_27F018C15ED2__INCLUDED_)
#define AFX_JSON_H__B17ABB5B_C5E3_4C8E_A5AC_27F018C15ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BLOCK_LENGTH	2048

#include <afx.h>
#include <afxtempl.h>

#define _INF_	((double)(0xffffffff-1))

class CJson  
{
public:
	CJson();
	~CJson();

	void Begin();
	void End();
	const char* GetStr()	{return m_sJSON;}
	void Clear()			{m_sJSON=""; m_bEmpty=true;}

	void BeginArray();
	void EndArray();

	int Add(const char* szKey, const char* szVal);
	int Add(const char* szVal);

	int AddString(const char* szKey, const char* szVal);

	int AddInt(const char* szKey, const int nVal);
	int AddIntQ(const char* szKey, const int nVal);
	
	int AddDouble(const char* szKey, const double fVal);
	int AddStringArray(const char* szKey, CStringArray& arVals);
	int AddIntArray(const char* szKey, CArray<int,int>& arVals);
	int AddDoubleArray(const char* szKey, CArray<double,double>& arVals);

protected:
	CString m_sJSON;
	BOOL	m_bEmpty;
};

#endif // !defined(AFX_JSON_H__B17ABB5B_C5E3_4C8E_A5AC_27F018C15ED2__INCLUDED_)
