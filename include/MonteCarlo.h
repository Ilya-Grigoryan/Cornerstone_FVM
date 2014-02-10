// MonteCarlo.h: interface for the CMonteCarlo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONTECARLO_H__638B74CA_EEFB_4290_B33F_57E6A30A21E6__INCLUDED_)
#define AFX_MONTECARLO_H__638B74CA_EEFB_4290_B33F_57E6A30A21E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseMath.h"

// callback function to make one calc step
typedef void (*FnPointerCalc)(int nInputs, double* inputs, int nOuts, double* outs);

// input distribution types
#define MC_PARAM_INVALID			-1 
#define MC_PARAM_CONST				0  
#define MC_PARAM_UNIFORM			1
#define MC_PARAM_NORMAL				2
#define MC_PARAM_LOGNORMAL			3
#define MC_PARAM_EXPONENTIAL		4
#define MC_PARAM_ERLANG				5
#define MC_PARAM_CHISQUARE			6
#define MC_PARAM_STUDENT			7

#define MC_PARAM_BERNOULLI			8
#define MC_PARAM_BINOMIAL			9
#define MC_PARAM_EQUILIKELY			10
#define MC_PARAM_GEOMETRIC			11
#define MC_PARAM_PASCAL				12
#define MC_PARAM_POISSON			13

/////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CHistogram
{
public:
	CHistogram();
	~CHistogram();

public:
	struct Item
	{
		Item() {	low = high = prob = 0.0;	}
		double low;
		double high;
		double prob;
	};

	void	Create(double* pData, int nDataSize, int nHistoSize);
	void	RemoveAll();
	int		GetItemCount()			{ return m_nItems;				}
	Item	GetItem(int index)		{ return m_pItems[index];		}

	int		GetDataSize()			{ return m_nData;				}
	double	GetValue(int index)		{ return m_pDataSorted[index];	}
	double*	GetSortedData()			{ return m_pDataSorted;			}

	double	GetMin()				{ return m_nData > 0 ? m_pDataSorted[0] : _INF_;			}
	double	GetMax()				{ return m_nData > 0 ? m_pDataSorted[m_nData-1] : _INF_;	}
	double	GetMean();
	double	GetMedian();
	Item	GetMode();
	double	GetStDev();
	double  GetVariance();
	double	GetCoeffOfVar();
	double	GetSkewness();
	double	GetKurtosis();

	double	GetCumulativeProb(double val);
	double	GetInverseCumulativeProb(double prob);

protected:
	int		m_nItems;
	Item*	m_pItems;

	int		m_nData;
	double*	m_pDataSorted;
};

/////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMonteCarloInput
{
public:
	CMonteCarloInput();
	CMonteCarloInput(int nType, double f1, double f2, int nSampleSize);
	~CMonteCarloInput();

public:
	void SetType(int type)				{ m_nType = type;			}
	int GetType()						{ return m_nType;			}

	void SetF1(double f)				{ m_f1 = f;					}
	double GetF1()						{ return m_f1;				}

	void SetF2(double f)				{ m_f2 = f;					}
	double GetF2()						{ return m_f2;				}

	void SetSampleSize(int size)		{ m_nSampleSize = size;		}
	int GetSampleSize()					{ return m_nSampleSize;		}

	double* GetValues()					{ return m_pValues;			}
	inline double GetValue(int index);			

	void Generate();
	void Clear();

protected:
	int		m_nType;	// distribution type
	
	double	m_f1;		// first descriptor; for example if type=const it's the actual value, if type=normal, it's the mean
	double	m_f2;		// second descriptor; for example if type=normal, it's the stdev

	int		m_nSampleSize;
	double* m_pValues;
};

/////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMonteCarloOut
{
public:
	CMonteCarloOut();
	CMonteCarloOut(int nSize);
	~CMonteCarloOut();

public:
	void SetSize(int size);
	int	GetSize()								{ return m_nSize;			}

	void CreateHistogram(int nHistoSize);
	void Clear();

	void SetValue(int index, double val)		{ m_pValues[index] = val;	}
	double GetValue(int index)					{ return m_pValues[index];	}

	CHistogram* GetHistogram()					{ return &m_Histo;			}

protected:
	int			m_nSize;
	double*		m_pValues;
	CHistogram	m_Histo;
};

/////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMonteCarloSimulation
{
public:
	CMonteCarloSimulation();
	CMonteCarloSimulation(int nSampleSize, int nInputs, int nOuts, FnPointerCalc pFunc);
	~CMonteCarloSimulation();

public:
	void SetSampleSize(int size);
	int GetSampleSize()							{ return m_nSampleSize;			}

	void SetInputCount(int count);
	int GetInputCount()							{ return m_nInputs;				}

	void SetOutCount(int count);
	int GetOutCount()							{ return m_nOuts;				}

	void SetCalcFunction(FnPointerCalc pFunc)	{ m_pFuncCalc = pFunc;			}
	FnPointerCalc GetCalcFunction()				{ return m_pFuncCalc;			}

	CMonteCarloInput*	GetInput(int index)		{ return &m_pInputs[index];		}
	CMonteCarloOut*		GetOut(int index)		{ return &m_pOuts[index];		}

	void Run();
	void Clear();

protected:
	int	m_nSampleSize;

	int	m_nInputs;
	int	m_nOuts;
	
	CMonteCarloInput*		m_pInputs;
	CMonteCarloOut*			m_pOuts;

	FnPointerCalc	m_pFuncCalc;
};

#endif // !defined(AFX_MONTECARLO_H__638B74CA_EEFB_4290_B33F_57E6A30A21E6__INCLUDED_)
