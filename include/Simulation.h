// Simulation.h: interface for the CSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULATION_H__B0B11CF8_4CAE_4900_9879_491D3F4DE53A__INCLUDED_)
#define AFX_SIMULATION_H__B0B11CF8_4CAE_4900_9879_491D3F4DE53A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Order.h"
#include "Bar.h"
#include "ListEx.h"

class CSimulation;
class CSimulationItem;
class CAccount;

typedef int (__stdcall *_RunFunc)(CSimulationItem* pItem, const CTime& time);
typedef void (__stdcall *_SetParamFunc)(const char* szParam, double fValue);

/////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CSimulationContext
{
public:
	
};

/////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CSimulationItem
{
public:
	CSimulationItem(CSimulation* pSimulation);
	BOOL		Init(const char* szSecurity, int nTimeframe, const char* szExpert);

	int			Run(const CTime& time);

	void		MoveOrderToHistory(COrder* pOrder);

public:
	char		m_szSecurity[32];
	int			m_nTimeframe;
	char		m_szExpert[128];

	_RunFunc		m_pFuncRun;
	_SetParamFunc	m_pFuncSetParam;

	CBarData*	m_pData;	

	CArray<COrder*,COrder*>		m_arOrdersActive;
	CArray<COrder*,COrder*>		m_arOrdersHistory;

	CSimulation*	m_pSimulation;		// back pointer
};

/////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CSimulation  
{
public:
	CSimulation();
	~CSimulation();

public:
	CSimulationItem* AddExpert(const char* szSecurity, int nTimeframe, const char* szExpert);
	void SetSimulationPeriod(const CTime& start, const CTime& end);
	void SetDataPeriod(const CTime& start, const CTime& end);

	BOOL Run();

	CTime GetCurrentTime()			{ return m_CurrentTime;		}
	CAccount* GetAccount()			{ return m_pAccount;		}

protected:
	int		m_nStep;		// step by minutes
	
	CTime	m_StartTime;	// start time
	CTime	m_EndTime;		// end time

	CTime	m_DataStartTime;// data start time
	CTime	m_DataEndTime;	// data end time

	CTime	m_CurrentTime;	

	CArray<CSimulationItem*,CSimulationItem*>	m_arItems;
	CListEx<CString>	m_arDataList; 
	CAccount*			m_pAccount;

	static int GetNextOrderTicket();

	friend class CSimulationItem;
	friend class CAccount;
	friend class COrder;
};

#endif // !defined(AFX_SIMULATION_H__B0B11CF8_4CAE_4900_9879_491D3F4DE53A__INCLUDED_)
