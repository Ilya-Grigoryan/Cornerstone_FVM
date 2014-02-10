// Math.cpp: implementation of the CMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "BaseMath.h"

#include "rngs.h"
#include "rvgs.h"

#include "listex.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long CMath::m_nRandomSeed = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMath::CMath()
{

}

CMath::~CMath()
{

}

//////////////////////////////////////////////////////////////////////
double CMath::GetMean(double* values, unsigned int size)
{
	if(size <= 0)
		return (double)_INF_;

	int nRealSize = 0;
	double s = 0;
	for(unsigned int i=0; i<size; i++)
	{
		if(values[i] != (double)_INF_)
		{
			s += values[i];
			++ nRealSize;
		}
	}

	if(nRealSize > 0)
		s /= (double)nRealSize;
	else
		s = 0.;

	return s;
}

double CMath::GetMean(CArray<double, double>& values)
{
	return GetMean(values.GetData(), values.GetSize());
}

double CMath::GetVariance(double* values, unsigned int size)
{
	if(size < 1)
		return 0;

	double mean = GetMean(values, size);
	double fV = 0;
	int nRealSize = 0;
	for(unsigned int i=0; i<size; i++)
	{
		if(values[i] != (double)_INF_)
		{
			double diff = (values[i]-mean)*(values[i]-mean);
			fV += diff;
			++ nRealSize;
		}
	}

	if(nRealSize > 1)
		fV /= (double)(nRealSize-1);
	else
		fV = 0;

	return fV;	
}

double CMath::GetVariance(CArray<double,double>& values)
{
	return GetVariance(values.GetData(), values.GetSize());
}

double CMath::GetStDev(double* values, unsigned int size)
{
	if(size < 1)
		return 0;

	double fV = GetVariance(values, size);
	fV = sqrt(fV);
	
	return fV;
}

double CMath::GetStDev(CArray<double,double>& values)
{
	return GetStDev(values.GetData(), values.GetSize());
}

double CMath::GetCoeffVar(double* values, unsigned int size)
{
	if(size < 1)
		return 0;

	double fMean = GetMean(values, size);
	double fStdev = GetStDev(values, size);

	double fCoeffVar = fStdev / fMean;
	return fCoeffVar;
}

double CMath::GetCoeffVar(CArray<double, double>& values)
{
	return GetCoeffVar(values.GetData(), values.GetSize());
}

double CMath::GetPresentValue(double fAmountEachPeriod, double fDiscountRate, double fPeriods)
{
	double fVal = fAmountEachPeriod * ((1-(1/pow((1+fDiscountRate),fPeriods)))/ fDiscountRate );

	return fVal;
}

double CMath::GetPresentValue(double *pPayments, int nCount, double fDiscountRate)
{
	double fPV = 0;
	for(int i=0; i<nCount; i++)
	{
		double f = pPayments[i] / pow((1+fDiscountRate), i+1);
		fPV += f;
	}
	return fPV;
}

double CMath::GetIRR(double fInvestment, double* pPayments, int nCount, double fGuess)
{
	double fRate = fGuess;
	int nDir = 1;
	double fStep = 0.1;

	int nIteration = 0;
	while(fStep > 0.0000001)
	{
		double fPV = GetPresentValue(pPayments, nCount, fRate);

		if(fPV == fInvestment)
			return fRate;
		else if(fPV > fInvestment)
		{
			if(nDir == -1) 
			{
				fStep /= 2.0;
				nDir = 1;
			}
			fRate += fStep;
		}
		else
		{
			if(nDir == 1)
			{
				fStep /= 2.0;
				nDir = -1;
			}
			fRate -= fStep;
		}

		if(++nIteration > 10000)
			return _INF_;
	}

	return fRate;
}

double CMath::GetCovariance(double* px, double *py, unsigned int size)
{
	double x0 = GetMean(px, size);
	double y0 = GetMean(py, size);

	int N=0;
	double SSxy=0;
	for(unsigned int i=0; i<size; i++)
	{
		if( !(px[i]<_INF_ && py[i]<_INF_) )
			continue;
	
		SSxy += (px[i]-x0)*(py[i]-y0);
		++N;
	} 

	if(N==0)
		return 0;

	return SSxy/N;
}

double CMath::GetCovariance(CArray<double, double> &x, CArray<double, double> &y)
{
	return GetCovariance(x.GetData(), y.GetData(), x.GetSize());
}

double CMath::GetCorrelation(double* px, double *py, unsigned int size)
{
	double x0 = GetMean(px, size);
	double y0 = GetMean(py, size);

	double SSxx=0, SSyy=0, SSxy=0;
	for(unsigned int i=0; i<size; i++)
	{
		if( !(px[i]<_INF_ && py[i]<_INF_) )
			continue;
		
		SSxx += (px[i]-x0)*(px[i]-x0);
		SSyy += (py[i]-y0)*(py[i]-y0);
		SSxy += (px[i]-x0)*(py[i]-y0);
	} 

	if(SSxx == 0 || SSyy == 0)
		return 0;
	
	return SSxy/sqrt(SSxx*SSyy);
}

double CMath::GetCorrelation(CArray<double, double> &x, CArray<double, double> &y)
{
	return GetCorrelation(x.GetData(), y.GetData(), x.GetSize());
}

void CMath::SimpleRegression(double* px, double* py, unsigned int size, double& slope, double& intercept, double& coeff)
{
	double x0 = GetMean(px, size);
	double y0 = GetMean(py, size);

	double Sx=0, Sy=0, Sxy=0, SSx=0, SSy=0, SSxx=0, SSyy=0, SSxy=0;
	for(unsigned int i=0; i<size; i++)
	{
		if( !(px[i]<_INF_ && py[i]<_INF_) )
			continue;
		
		SSxx += (px[i]-x0)*(px[i]-x0);
		SSyy += (py[i]-y0)*(py[i]-y0);
		SSxy += (px[i]-x0)*(py[i]-y0);
	} 

	if(SSxx != 0)
		slope = SSxy/SSxx;
	else
		slope = 0;

	intercept = y0 - slope*x0;

	coeff = SSxy/sqrt(SSxx*SSyy);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void CMath::InitRandomSeeds(long nValue)
{
	if(nValue == m_nRandomSeed) // nothing to change
		return;

	m_nRandomSeed = nValue;
	PlantSeeds(m_nRandomSeed);
}

void CMath::SetRandomSeed(long nValue)
{
	PutSeed(nValue);
}

long CMath::GetRandomSeed()
{
	long x;
	GetSeed(&x);
	return x;
}

double CMath::GetDistribNormal(double fMean, double fStdev)
{
	return Normal(fMean, fStdev);
}

void CMath::GetDistribNormal(double fMean, double fStdev, double *values, int size)
{
	for(int i=0; i<size; i++)
		values[i] = GetDistribNormal(fMean, fStdev);
}

void CMath::GetHistogram(double* pData, int nDataSize, int nBarCount, double* pLimits, int* pProbs)
{
	CListEx<double> sort;
	int i;

	for(i=0; i<nDataSize; i++)
		sort.Add(pData[i], NULL);

	memset(pProbs, 0, nBarCount*sizeof(int));

	double min = sort.GetItem(0)->m_Key;
	double max = sort.GetItem(sort.GetSize()-1)->m_Key;
	double range = max-min;
	double step = range/(double)nBarCount;

	int ind = 0;
	pLimits[0] = min;
	for(i=0; i<nBarCount; i++)
	{
		double limit = min+step*(i+1);
		pLimits[i+1] = limit;

		while(ind < nDataSize && sort.GetItem(ind)->m_Key <= limit)
		{
			++ pProbs[i];
			++ ind;
		}
	}

}