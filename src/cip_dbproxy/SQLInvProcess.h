// SQLInvProcess.h: interface for the CSQLInvProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLINVPROCESS_H__7130D3CB_5655_4B3E_B739_E616B20BD1DA__INCLUDED_)
#define AFX_SQLINVPROCESS_H__7130D3CB_5655_4B3E_B739_E616B20BD1DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class _declspec(dllexport) CSQLInvProcess  
{
public:
	CSQLInvProcess();
	~CSQLInvProcess();

	static int Add(int nCompanyId, int nStageId);
	static int Delete(int nCompanyId, int nStageId);
	static int Get(int nStageId, CArray<int,int>& arCompIds, CStringArray& arTickers);

};

#endif // !defined(AFX_SQLINVPROCESS_H__7130D3CB_5655_4B3E_B739_E616B20BD1DA__INCLUDED_)
