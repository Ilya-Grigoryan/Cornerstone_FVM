// History.h: interface for the CHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORY_H__D4C58C9A_F2CF_48F0_940F_518C5B7433A6__INCLUDED_)
#define AFX_HISTORY_H__D4C58C9A_F2CF_48F0_940F_518C5B7433A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "input.h"
#include "ListEx.h"

class CCompany;
class _declspec(dllexport) CHistory  : public SHistory
{
public:
	CHistory();
	CHistory(CCompany* pCompany);
	~CHistory();

	void			Clear();

	BOOL			PreLoad(const char* szTicker);
	BOOL			Load(const char* szTicker, COleDateTime from, COleDateTime to);
	BOOL			Load(const char* szTicker, int nNumRecords);

	BOOL			LoadItem(const char* szTicker, const char* szItem, int nNumRecords=-1);
	BOOL			ItemLoadedSize(int nItem)	{ return arLoaded[nItem];					}
	BOOL			IsItemAllLoaded(int nItem)	{ return arLoaded[nItem]==GetSize();		}

	BOOL			IsLoaded()					{ return m_bLoaded;				}
	BOOL			IsAllLoaded()				{ return m_bAllLoaded;			}

	void			SetTicker(const char* val)	{ strcpy(sTicker, val);			}
	CString			GetTicker()					{ return sTicker;				}
	int				GetSize()					{ return arItems.GetSize();		}

	SHistoryItem*	GetItemPtr(int index)		{ return &arItems[index];		}
	SHistoryItem*	GetItemPtr(COleDateTime date);
	int				GetItemIndex(COleDateTime date, BOOL bExact);

	void			GetStartEndIndexesForPeriod(COleDateTime from, COleDateTime to, int& nStart, int &nEnd);

	void			DeleteItem(int index)		{ arItems.RemoveAt(index);		}

	int				GetIndexByDate(COleDateTime date);
	void			CalcMAvgValues();

	double			GetRetFore()				{ return m_fRetFore;			}
	double			GetPriceFore()				{ return m_fPriceFore;			}
	double			GetMAFore()					{ return m_fMAFore;				}
	double			GetPMFore()					{ return m_fPMFore;				}
	double			GetMMFore()					{ return m_fMMFore;				}

protected:
	BOOL			m_bLoaded;
	BOOL			m_bAllLoaded;

	COleDateTime	m_From;
	COleDateTime	m_To;

	double			m_fRetFore;
	double			m_fPriceFore;
	double			m_fMAFore;
	double			m_fPMFore;
	double			m_fMMFore;

	CCompany*		m_pCompany;		// Back pointer
};

#endif // !defined(AFX_HISTORY_H__D4C58C9A_F2CF_48F0_940F_518C5B7433A6__INCLUDED_)
