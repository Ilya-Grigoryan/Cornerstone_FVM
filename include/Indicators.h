// Indicators.h: interface for the CIndicators class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORS_H__2956A0F3_3B7C_4E09_BA54_87AEB41C2B72__INCLUDED_)
#define AFX_INDICATORS_H__2956A0F3_3B7C_4E09_BA54_87AEB41C2B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBarData;

double _declspec(dllexport) iMA(CBarData* pData, int nPeriodMA, int nShiftMA, int nAppliedPrice, int nShift); 

#endif // !defined(AFX_INDICATORS_H__2956A0F3_3B7C_4E09_BA54_87AEB41C2B72__INCLUDED_)
