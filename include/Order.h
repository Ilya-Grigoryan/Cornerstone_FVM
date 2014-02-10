// Order.h: interface for the COrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDER_H__21A5AA58_7F5D_47BD_B8FA_84D2727B13D5__INCLUDED_)
#define AFX_ORDER_H__21A5AA58_7F5D_47BD_B8FA_84D2727B13D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CAccount;
class CSimulation;
class CSimulationItem;

/////////////////////////////////////////////////////////////////////
class _declspec(dllexport) COrder  
{
public:
	COrder(CAccount* pAccount);
	~COrder();

	void Recalc(const CTime& time);

public:
	int		m_nMagic;
	int		m_nTicket;
	char	m_szSymbol[32];
	
	double	m_fLot;
	int		m_nType;		// 1 - buy, -1 - close
	
	int		m_nStatus;		// 0 - pending, 1 - active, 2 - history
	CTime	m_TimeExpire;	// expire time for pending orders

	CTime	m_TimeOpen;
	CTime	m_TimeClose;

	double	m_fPriceOpen;
	double	m_fPriceClose;

	double	m_fSL;
	double	m_fTP;

	double	m_fSwap;
	double	m_fCommision;

	double	m_fProfit;

	CSimulationItem*	m_pItem;	// back pointer to simulation item
	CAccount* m_pAccount;	// back pointer to account
};

/////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CAccount
{
public:
	CAccount();
	~CAccount();

public:
	int		NewOrder(CSimulationItem* pItem, const char* szSymbol, int nType, double fLot, double fPrice, int nSlippage, 
					 double fSL, double fTP, int nMagic, time_t timeExpire=0);
	int		CloseOrder(int nTicket,double fPrice, int nSlippage);
	void	CloseActiveOrders();

	void	Recalc(const CTime& time);
	int		GetActiveOrderByTicket(int nTicket);

	double	GetInitialBalance()		{ return m_fInitialBalance;	}
	double	GetBalance()			{ return m_fBalance;		}
	double	GetEquity()				{ return m_fEquity;			}

	int		GetLeverage()			{ return m_nLeverage;		}
	double	GetFreeMargin()			{ return m_fFreeMargin;		}

public:
	CArray<COrder*,COrder*>		m_arOrdersActive;
	CArray<COrder*,COrder*>		m_arOrdersHistory;

	double	m_fInitialBalance;
	double	m_fBalance;
	double	m_fEquity;
	
	int		m_nLeverage;
	double	m_fFreeMargin;

	CSimulation* m_pSimulation;
	double	m_fCurrentPL;
};

#endif // !defined(AFX_ORDER_H__21A5AA58_7F5D_47BD_B8FA_84D2727B13D5__INCLUDED_)
