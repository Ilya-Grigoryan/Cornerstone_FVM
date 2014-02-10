// CIPException.h: interface for the CCIPException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIPEXCEPTION_H__B0750CAF_9D9E_4539_8110_F9ED9C485D19__INCLUDED_)
#define AFX_CIPEXCEPTION_H__B0750CAF_9D9E_4539_8110_F9ED9C485D19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define CIP_ERR_HISTORY_OVERFLOW				0x00001
#define CIP_ERR_NO_ROC_HISTORY					0x00002
#define CIP_ERR_INVALID_DISCOUNT_RATE			0x00003


/////////////////////////////////////////////////////////////
class CCIPException  
{
public:
	CCIPException();
	~CCIPException();

	void ClearAll()										{ m_arException.RemoveAll();	}
	void SetException(unsigned int nExp)				{ m_arException.Add(nExp);		}

public:
	CArray<unsigned int, unsigned int>	m_arException;
};

#endif // !defined(AFX_CIPEXCEPTION_H__B0750CAF_9D9E_4539_8110_F9ED9C485D19__INCLUDED_)
