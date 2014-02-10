// Bar.h: interface for the CBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAR_H__AE874415_5A27_482F_8830_E93525678BE1__INCLUDED_)
#define AFX_BAR_H__AE874415_5A27_482F_8830_E93525678BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////////////
struct CBar  
{
	CTime	time;
	double	open;
	double	low;
	double	high;
	double	close;
	double	volume;
};


//////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CBarData
{
public:
	CBarData();
	~CBarData();

public:
	void Clear();
	BOOL Load(CTime& start, CTime& end);

	BOOL IsEmpty()							{ return m_nDataSize == 0;	}

	void SetName(const char* szName)		{ strcpy(m_szName, szName); }
	const char* GetName()					{ return m_szName;			}

	void SetTimeframe(const int nTimeframe);
	const int GetTimeframe();

	// data access
	CBar*	GetCurrent()					{ return &m_pData[m_nPos];	}
	int		GetSize()						{ return m_nDataSize;		}
	
	CBar*	GetBar(int nIndex)				{ return &m_pData[nIndex];	}
	CBar*	operator[](int nIndex)			{ return &m_pData[nIndex];	}

	CBar*	GetBar(const CTime &time, BOOL bExact=false);
	CBar*	operator[](CTime& time);

	int		GetPos()						{ return m_nPos;			}
	void	SetPos(const int nPos)			{ m_nPos = nPos;			}

	int		SetPos(const CTime& time, BOOL bExact=false);
	void	Next()							{ if(m_nPos < m_nDataSize-1) m_nPos++;	}

	time_t	GetStartTime()					{ if(m_pData) return m_pData[0].time.GetTime(); else return 0;	}
	time_t	GetEndTime()					{ if(m_pData) return m_pData[m_nDataSize-1].time.GetTime(); else return 0;	}

protected:
	CBar*	m_pData;
	int		m_nDataSize;
	int		m_nPos;

	char	m_szName[32];
	int		m_nTimeframe;
};

#endif // !defined(AFX_BAR_H__AE874415_5A27_482F_8830_E93525678BE1__INCLUDED_)
