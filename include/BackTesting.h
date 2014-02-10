// BackTesting.h: interface for the CBackTesting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKTESTING_H__D61B308F_86C1_4B56_B08C_EFF59D4591C5__INCLUDED_)
#define AFX_BACKTESTING_H__D61B308F_86C1_4B56_B08C_EFF59D4591C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basemath.h"
#include "listex.h"

class CDialogProgressEx;
class CBackTesting;
class CBackTestingItem;

typedef CBackTestingItem* (*FnPtItemCreator)(CBackTesting*	pBackTesting);

//////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CBackTestingItem
{
public:
	CBackTestingItem();
	CBackTestingItem(const char* szTicker, const COleDateTime& date);

	~CBackTestingItem();

public:
	double GetPrice() const					{ return m_fPrice;		} 
	double GetScore() const					{ return m_fScore;		}
	const char* GetTicker() const			{ return m_szTicker;	}
	const COleDateTime GetDate() const		{ return m_Date;		} 

public:
	virtual BOOL	Load()=0;
	virtual void	Calc()=0;

	virtual double	Get(const char* szWhat)=0;
	virtual double	Get(int nWhat)=0;

	static CBackTestingItem* Creator(CBackTesting*	pBackTesting);

public:
	char			m_szTicker[8];		// Ticker
	COleDateTime	m_Date;				// date
	
	double			m_fPrice;			// Price is a special item (for effective computations)
	double			m_fScore;			// Internal score of the stock (will be sorted and filtered based on this score)

	CBackTesting*	m_pBackTesting;		// back pointer
};

typedef CArray<CBackTestingItem*,CBackTestingItem*> CBackTestingItemArray;

//////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CBackTesting  
{
public:
	CBackTesting();
	~CBackTesting();

public:
	BOOL	CreateEnvironment(	const COleDateTime& dtStart, const COleDateTime& dtEnd, const COleDateTimeSpan& AdditionalHist,
								const CStringArray& arTickers, 
								const COleDateTimeSpan& PeriodStatement,
								const COleDateTimeSpan& PeriodRebalance,
								FnPtItemCreator FnCreator);
	void	CalcItems();

protected:
	void AddItemToListDates(const COleDateTime& date, const CBackTestingItem* pItem);
	void AddItemToListTickers(const char* szTicker, const CBackTestingItem* pItem);

	CBackTestingItem* GetItem(int nIndex);
	CBackTestingItem* GetItem(const char* szTicker, const COleDateTime& date);

	void GetUniverseSnapshot(const COleDateTime& date);
	void GetItemSeriesForTicker(const char* szTicker);

	void RemoveAllItems();
	void ClearMappingLists();

protected:
	COleDateTime		m_StartDate;			// start date of simulation / backtesting
	COleDateTime		m_EndDate;				// end date of simulation / backtesting
	
	COleDateTimeSpan	m_AdditionalHistory;	// additional time to go back in history (for MA's, etc.)
	COleDateTimeSpan	m_PeriodStatement;		// statements period (day, week, month, etc.)
	COleDateTimeSpan	m_PeriodRebalance;		// rebalancing period

	// basic data (universe snapshots for the specified period)
	CArray<CBackTestingItem*,CBackTestingItem*>		m_arItems;
	CListEx<COleDateTime>							m_ListByDates;
	CListEx<CString>								m_ListByTickers;

	CDialogProgressEx*	m_pDlgProgress;
};

//////////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CIRRItem : public CBackTestingItem
{
public:
	CIRRItem();
	CIRRItem(const char* szTicker, const COleDateTime& date);

	virtual BOOL	Load();
	virtual void	Calc();

	virtual double	Get(const char* szWhat);
	virtual double	Get(int nWhat);

	static CBackTestingItem* Creator(CBackTesting*	pBackTesting);

public:
	double m_fFCF;
	double m_fDiv;

	double m_fRisk;
	double m_fIRR;
};

#endif // !defined(AFX_BACKTESTING_H__D61B308F_86C1_4B56_B08C_EFF59D4591C5__INCLUDED_)
