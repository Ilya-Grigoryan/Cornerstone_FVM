// MonteCarlo.cpp: implementation of the CMonteCarlo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "MonteCarlo.h"

#include "ListEx.h"
#include "rvgs.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
CHistogram::CHistogram()
{
	m_nItems = 0;
	m_pItems = NULL;

	m_nData = 0;
	m_pDataSorted = NULL;
}

CHistogram::~CHistogram()
{
	RemoveAll();
}

void CHistogram::RemoveAll()
{
	if(m_pItems)
		delete [] m_pItems;
	m_pItems = NULL;
	m_nItems = 0;

	if(m_pDataSorted)
		delete m_pDataSorted;
	m_pDataSorted = NULL;
	m_nData = 0;
}

void CHistogram::Create(double* pData, int nDataSize, int nHistoSize)
{
	RemoveAll();

	CListEx<double> sort;
	int i;

	for(i=0; i<nDataSize; i++)
	{
		if(pData[i] < (double)_INF_)
			sort.Add(pData[i], NULL);
	}

	if(sort.GetSize() < 1)
		return; // no actual data 

	m_nData = sort.GetSize();
	m_pDataSorted = new double [m_nData];

	m_nItems = nHistoSize;
	m_pItems = new CHistogram::Item [nHistoSize];

	double min = sort.m_ItemData[0]->m_Key;
	double max = sort.m_ItemData[m_nData-1]->m_Key;
	double range = max-min;
	double step = range/(double)m_nItems;

	int ind = 0;
	double low = min;
	for(i=0; i<m_nItems; i++)
	{
		double high = min+step*(i+1);

		int prob = 0;
		while(ind < m_nData && sort.m_ItemData[ind]->m_Key <= high)
		{
			m_pDataSorted[ind] = sort.m_ItemData[ind]->m_Key;

			++ prob;
			++ ind;
		}

		m_pItems[i].low = low;
		m_pItems[i].high = high;
		m_pItems[i].prob = (double)prob / (double)m_nData;

		low = high;
	}
}

double CHistogram::GetCumulativeProb(double val)
{
	if(m_nItems == 0 || !m_pItems)
		return _INF_;

	if(val < m_pDataSorted[0])
		return 0.0;
	if(val >= m_pDataSorted[m_nData-1])
		return 1.0;

	double prob = 0;
	int i=0;
	while(i < m_nData && val <= m_pDataSorted[i])
		++i;

	double fCumProb = (double)i / (double)m_nData;
	return fCumProb;
}

double	CHistogram::GetInverseCumulativeProb(double prob)
{
	if(prob < 0 || prob > 1)
		return _INF_;

	int nIndex = (int) ( prob * (double)(m_nData-1) );

	if(nIndex >= 0 && nIndex < m_nData)
		return m_pDataSorted[nIndex];

	return 0;
}

double CHistogram::GetMean()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;

	return CMath::GetMean(m_pDataSorted, m_nData);
}

double CHistogram::GetMedian()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;
	
	if(m_nItems % 2 != 0)
	{
		int nIndex = (m_nData-1)/2;
		return m_pDataSorted[nIndex];
	}
	else
	{
		int nIndex = m_nData/2;
		return (m_pDataSorted[nIndex] + m_pDataSorted[nIndex-1])/2.0;
	}
	
	return _INF_;
}

CHistogram::Item CHistogram::GetMode()
{
	if(!m_nItems || !m_pItems)
		return CHistogram::Item();

	double fMax = m_pItems[0].prob;
	int nIndex = 0;
	for(int i=1; i<m_nItems; i++)
	{
		if(m_pItems[i].prob > fMax)
		{
			fMax = m_pItems[i].prob;
			nIndex = i;
		}
	}

	return m_pItems[nIndex];
}

double CHistogram::GetStDev()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;

	return CMath::GetStDev(m_pDataSorted, m_nData);
}

double CHistogram::GetVariance()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;

	return CMath::GetVariance(m_pDataSorted, m_nData);
}

double CHistogram::GetCoeffOfVar()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;

	return CMath::GetCoeffVar(m_pDataSorted, m_nData);
}

double CHistogram::GetSkewness()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;

	double fMean = GetMean();
	double fVar = GetVariance();

	double fSkew = 0;
	for(int i=0; i<m_nData; i++)
		fSkew += (m_pDataSorted[i]-fMean)*(m_pDataSorted[i]-fMean)*(m_pDataSorted[i]-fMean);
	
	fSkew *= sqrt((double)m_nData)/pow( fVar*(double)m_nData, 1.5 );
	return fSkew;
}

double CHistogram::GetKurtosis()
{
	if(m_nData == 0 || !m_pDataSorted)
		return _INF_;

	double fMean = GetMean();
	double fVar = GetVariance();

	double fKurt = 0;
	for(int i=0; i<m_nData; i++)
		fKurt += (m_pDataSorted[i]-fMean)*(m_pDataSorted[i]-fMean)*(m_pDataSorted[i]-fMean)*(m_pDataSorted[i]-fMean);

	fKurt /= double(m_nData)*pow(fVar, 2.0);
    fKurt -= 3.0;
	return fKurt;
}

//////////////////////////////////////////////////////////////////////
CMonteCarloInput::CMonteCarloInput()
{
	m_nType = MC_PARAM_INVALID;
	m_nSampleSize = 0;
	m_pValues = NULL;
}

CMonteCarloInput::CMonteCarloInput(int nType, double f1, double f2, int nSampleSize)
{
	m_nType = nType;
	m_f1 = f1;
	m_f2 = f2;
	m_nSampleSize = nSampleSize;
}

CMonteCarloInput::~CMonteCarloInput()
{
	Clear();
}

double CMonteCarloInput::GetValue(int index)
{ 
	// special case for constants
	if(m_nType == MC_PARAM_CONST)
		return m_f1;

	if(index >= 0 && index < m_nSampleSize) 
		return m_pValues[index]; 
	
	return _INF_;	
}

void CMonteCarloInput::Generate()
{
	if(m_nType == MC_PARAM_INVALID)
		return;

	if(m_pValues)
		delete [] m_pValues;

	m_pValues = new double[m_nSampleSize];

	int i;
	switch(m_nType)
	{
	case MC_PARAM_UNIFORM:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Uniform(m_f1, m_f2);
		break;

	case MC_PARAM_NORMAL:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Normal(m_f1, m_f2);
		break;

	case MC_PARAM_LOGNORMAL:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Lognormal(m_f1, m_f2);
		break;

	case MC_PARAM_EXPONENTIAL:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Exponential(m_f1);
		break;

	case MC_PARAM_ERLANG:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Erlang((int)m_f1, m_f2);
		break;

	case MC_PARAM_CHISQUARE:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Chisquare((int)m_f1);
		break;

	case MC_PARAM_STUDENT:
		for(i=0; i<m_nSampleSize; i++)
			m_pValues[i] = Student((int)m_f1);
		break;
	}
}

void CMonteCarloInput::Clear()
{
	if(m_pValues)
		delete [] m_pValues;
	m_nSampleSize = 0;
}

//////////////////////////////////////////////////////////////////////
CMonteCarloOut::CMonteCarloOut()
{
	m_nSize = 0;
	m_pValues = NULL;
}

CMonteCarloOut::CMonteCarloOut(int nSize)
{
	m_nSize = nSize;
	m_pValues = new double[nSize];
}

CMonteCarloOut::~CMonteCarloOut()
{
	Clear();
}

void CMonteCarloOut::SetSize(int size)
{
	if(m_pValues)
		delete [] m_pValues;

	m_pValues = new double[size];
	m_nSize = size;

	for(int i=0; i<m_nSize; i++)
		m_pValues[i] = (double)_INF_;
}

void CMonteCarloOut::CreateHistogram(int nHistoSize)
{
	if(!m_pValues)
		return;

	m_Histo.Create(m_pValues, m_nSize, nHistoSize);
}

void CMonteCarloOut::Clear()
{
	if(m_pValues)
		delete [] m_pValues;
	m_pValues = NULL;

	m_Histo.RemoveAll();
	m_nSize = 0;
}

//////////////////////////////////////////////////////////////////////
CMonteCarloSimulation::CMonteCarloSimulation()
{
	m_nSampleSize = 0;
	m_nInputs = 0;
	m_nOuts = 0;

	m_pInputs = NULL;
	m_pOuts = NULL;

	m_pFuncCalc = NULL;
}

CMonteCarloSimulation::CMonteCarloSimulation(int nSampleSize, int nInputs, int nOuts, FnPointerCalc pFunc)
{
	m_nInputs = nInputs;
	m_nOuts = nOuts;

	m_pInputs = new CMonteCarloInput[nInputs];
	m_pOuts = new CMonteCarloOut[nOuts];

	m_pFuncCalc = pFunc;

	SetSampleSize(nSampleSize);
}

CMonteCarloSimulation::~CMonteCarloSimulation()
{
	Clear();
}

void CMonteCarloSimulation::SetSampleSize(int size)
{
	m_nSampleSize = size;

	int i;
	for(i=0; i<m_nInputs; i++)
		m_pInputs[i].SetSampleSize(size);
	
	for(i=0; i<m_nOuts; i++)
		m_pOuts[i].SetSize(size);
}

void CMonteCarloSimulation::SetInputCount(int count)
{
	if(m_pInputs)
		delete [] m_pInputs;
	
	m_nInputs = count;
	m_pInputs = new CMonteCarloInput[count];

	for(int i=0; i<m_nInputs; i++)
		m_pInputs[i].SetSampleSize(m_nSampleSize);
}

void CMonteCarloSimulation::SetOutCount(int count)
{
	if(m_pOuts)
		delete [] m_pOuts;

	m_nOuts = count;
	m_pOuts = new CMonteCarloOut[count];

	for(int i=0; i<m_nOuts; i++)
		m_pOuts[i].SetSize(m_nSampleSize);
}

void CMonteCarloSimulation::Clear()
{
	if(m_pInputs)
		delete [] m_pInputs;
	m_pInputs = NULL;

	if(m_pOuts)
		delete [] m_pOuts;
	m_pOuts = NULL;

	m_nInputs = m_nOuts = m_nSampleSize = 0; 
}

void CMonteCarloSimulation::Run()
{
	if(!m_pFuncCalc)
		return;

	// allocate buffers
	double* pInputs = new double [m_nInputs];
	double* pOuts = new double [m_nOuts];

	// generate inputs
	for(int i=0; i<m_nInputs; i++)
		m_pInputs[i].Generate();

	// run for all samples
	for(int i=0; i<m_nSampleSize; i++)
	{
		for(int j=0; j<m_nInputs; j++)
			pInputs[j] = m_pInputs[j].GetValue(i);

		// call the function
		(*m_pFuncCalc)(m_nInputs, pInputs, m_nOuts, pOuts);

		// set output values
		for(int j=0; j<m_nOuts; j++)
			m_pOuts[j].SetValue(i, pOuts[j]);
	}

	delete [] pInputs;
	delete [] pOuts;
}
