// MacroParameters.h: interface for the CMacroParameters class.
//
// Global parameters to be used in model calculations 
//
// Copyright (C) 2010 Cornerstone Investment Partners
//
// ///////////////////////////////////////////////////////////////////
//
// Created:			05/13/2010, Davit Davtyan
// Last update:		
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACROPARAMETERS_H__DF3FC507_7D85_4226_ADFA_55328A189219__INCLUDED_)
#define AFX_MACROPARAMETERS_H__DF3FC507_7D85_4226_ADFA_55328A189219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMacroParameters  
{
public:
	CMacroParameters();
	~CMacroParameters();

public:
	void			SetDiscountRate(double fValue);  // adjusts IntFactor, IntRate and MaxReinvestFactor accordingly 
	double			GetDiscountRate()										{ return m_fDiscountRate;				}

	void			SetFactor(double fValue)								{ m_fFactor = fValue;					}
	double			GetFactor()												{ return m_fFactor;						}

	void			SetInterestFactor(double fValue)						{ m_fInterestFactor = fValue;			}
	double			GetInterestFactor()										{ return m_fInterestFactor/100.0;		}

	void			SetInterestRate(double fValue)							{ m_fInterestRate = fValue;				}
	double			GetInterestRate()										{ return m_fInterestRate;				}

	void			SetMaxReinvestFactor(double fValue)						{ m_fMaxReinvestFactor = fValue;		}
	double			GetMaxReinvestFactor()									{ return m_fMaxReinvestFactor;			}


	void			SetTaxRate(double fValue)								{ m_fTaxRate = fValue;					}
	double			GetTaxRate()											{ return m_fTaxRate;					}

	void			SetYearsToDiscountEarnings(unsigned int nValue)			{ m_nYearsToDiscountEarnings = nValue;	}
	unsigned int	GetYearsToDiscountEarnings()							{ return m_nYearsToDiscountEarnings;	}

	void			SetYearsToUseEpsRoi(unsigned int nValue)				{ m_nYearsToUseEpsRoi = nValue;			}
	unsigned int	GetYearsToUseEpsRoi()									{ return m_nYearsToUseEpsRoi;			}

	void			SetPeriodsMA(unsigned int nValue)						{ m_nPeriodsMA = nValue;				}
	unsigned int	GetPeriodsMA()											{ return m_nPeriodsMA;					}

	void			SetMinGrowth(double fValue)								{ m_fMinGrowth = fValue;				}
	double			GetMinGrowth()											{ return m_fMinGrowth;					}

// class members
protected:
	double			m_fDiscountRate;				// discount rate
	double			m_fFactor;						// Discount factor (hidden value used in calculations)
	double			m_fInterestFactor;				// interest factor
	double			m_fInterestRate;				// interest rate
	double			m_fMaxReinvestFactor;			// factor for maximum reinvestment rate

	double			m_fTaxRate;						// tax rate

	unsigned int	m_nYearsToDiscountEarnings;		// # of years to discount earnings
	unsigned int	m_nYearsToUseEpsRoi;			// # of years to use for EPS and ROI means
	unsigned int	m_nPeriodsMA;					// # of periods to use for moving average

	double			m_fMinGrowth;					// Minimum Growth Rate for the companies 

};

#endif // !defined(AFX_MACROPARAMETERS_H__DF3FC507_7D85_4226_ADFA_55328A189219__INCLUDED_)
