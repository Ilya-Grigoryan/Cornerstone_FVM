// SQLUser.h: interface for the CSQLUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLUSER_H__AA967C96_B5F6_4461_A86D_C523286D2CBC__INCLUDED_)
#define AFX_SQLUSER_H__AA967C96_B5F6_4461_A86D_C523286D2CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define CIP_USER_LEVEL_CIO			1
#define CIP_USER_LEVEL_ADMIN		2
#define CIP_USER_LEVEL_EMPLOYEE		3


/////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CSQLUser  
{
public:
	CSQLUser();
	~CSQLUser();

	static int AddUser(const char* szLogin, const char* szPassw, int nLevel, const char* szName, const char* szDesc);
	static int GetUserIdAndNames(CArray<int,int>& arIds, CStringArray& arNames);

	static int Login(const char* szName, const char* szPswd);
	static int GetLevel(int nUserId);

	static int ChangePassword(const char* szName, const char* szPwdOld, const char* szPwd);
};


#endif // !defined(AFX_SQLUSER_H__AA967C96_B5F6_4461_A86D_C523286D2CBC__INCLUDED_)
