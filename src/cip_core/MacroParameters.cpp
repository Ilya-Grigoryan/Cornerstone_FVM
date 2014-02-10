// MacroParameters.cpp: implementation of the CMacroParameters class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "MacroParameters.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _DISCOUNT_TABLE_ROWS	53
#define _DISCOUNT_TABLE_COLS	4

extern CCip_coreApp		theApp;

//////////////////////////////////////////////////////////////////////
double arDiscountTable[_DISCOUNT_TABLE_ROWS][_DISCOUNT_TABLE_COLS] = 
{
	{0.15,		1,		5,		0.5},
	{0.1475,	1,		5,		0.5},
	{0.145,		1,		5,		0.5},
	{0.1425,	1,		5,		0.5},
	{0.14,		1,		5,		0.5},
	{0.1375,	1,		5,		0.5},
	{0.135,		1,		5,		0.5},
	{0.1325,	1,		5,		0.5},
	{0.13,		1,		5,		0.5},
	{0.1275,	1,		5,		0.5},
	{0.125,		1,		5,		0.5},
	{0.1225,	1,		5,		0.5},
	{0.12,		1,		5,		0.5},
	{0.1175,	1,		5,		0.5},
	{0.115,		1,		5,		0.5},
	{0.1125,	1,		5,		0.5},
	{0.11,		1,		5,		0.5},
	{0.1075,	1,		5,		0.5},
	{0.105,		1,		5,		0.5},
	{0.1025,	1,		5,		0.5},
	{0.1,		1,		5,		0.5},
	{0.0975,	1.0125,	5.25,	0.525},
	{0.095,		1.025,	5.5,	0.55},
	{0.0925,	1.0375,	5.75,	0.575},
	{0.09,		1.05,	6,		0.6},
	{0.0875,	1.0625,	6.25,	0.625},
	{0.085,		1.075,	6.5,	0.65},
	{0.0825,	1.0875,	6.75,	0.675},
	{0.08,		1.1,	7,		0.7},
	{0.0775,	1.1125,	7.25,	0.725},
	{0.075,		1.125,	7.5,	0.75},
	{0.0725,	1.1375,	7.75,	0.775},
	{0.07,		1.15,	8,		0.8},
	{0.0675,	1.1625,	8.25,	0.825},
	{0.065,		1.175,	8.5,	0.85},
	{0.0625,	1.1875,	8.75,	0.875},
	{0.06,		1.2,	9,		0.9},
	{0.0575,	1.2125,	9.25,	0.925},
	{0.055,		1.225,	9.5,	0.95},
	{0.0525,	1.2375,	9.75,	0.975},
	{0.05,		1.25,	10,		1},
	{0.0475,	1.2625,	10.25,	1},
	{0.045,		1.275,	10.5,	1},
	{0.0425,	1.2875,	10.75,	1},
	{0.04,		1.3,	11,		1},
	{0.0375,	1.3125,	11.25,	1},
	{0.035,		1.325,	11.5,	1},
	{0.0325,	1.3375,	11.75,	1},
	{0.03,		1.35,	12,		1},
	{0.0275,	1.3625,	12.25,	1},
	{0.025,		1.375,	12.5,	1},
	{0.0225,	1.3875,	12.75,	1},
	{0.02,		1.4,	13,		1}
};

//////////////////////////////////////////////////////////////////////
CMacroParameters::CMacroParameters()
{
	m_nYearsToDiscountEarnings = 10;
	m_nYearsToUseEpsRoi = 7;
	m_nPeriodsMA = 26;

	m_fTaxRate = 0.35;
	m_fInterestRate = 0.0525;

	SetDiscountRate(0.035);

	m_fMinGrowth = 0.05;
}

CMacroParameters::~CMacroParameters()
{

}

void CMacroParameters::SetDiscountRate(double value)
{
	m_fDiscountRate = -1;

	// check out of range
	if(value > arDiscountTable[0][0])
	{
		m_fDiscountRate = arDiscountTable[0][0];
		m_fFactor = arDiscountTable[0][1];
		m_fInterestFactor = arDiscountTable[0][2];
		m_fMaxReinvestFactor = arDiscountTable[0][3];

		return;
	}
	
	if(value <= arDiscountTable[_DISCOUNT_TABLE_ROWS-1][0])
	{
		m_fDiscountRate = arDiscountTable[_DISCOUNT_TABLE_ROWS-1][0];
		m_fFactor = arDiscountTable[_DISCOUNT_TABLE_ROWS-1][1];
		m_fInterestFactor = arDiscountTable[_DISCOUNT_TABLE_ROWS-1][2];
		m_fMaxReinvestFactor = arDiscountTable[_DISCOUNT_TABLE_ROWS-1][3];

		return;
	}

	// lookup in the table
	int i;
	for(i=0; i<_DISCOUNT_TABLE_ROWS-1; i++)
	{
		if(value <= arDiscountTable[i][0] && value > arDiscountTable[i+1][0])
		{
			m_fDiscountRate = arDiscountTable[i][0];
			m_fFactor = arDiscountTable[i][1];
			m_fInterestFactor = arDiscountTable[i][2];
			m_fMaxReinvestFactor = arDiscountTable[i][3];
			break;
		}
	}

	if(m_fDiscountRate == -1)
	{
		theApp.m_Exception.SetException(CIP_ERR_INVALID_DISCOUNT_RATE);
	}
}
