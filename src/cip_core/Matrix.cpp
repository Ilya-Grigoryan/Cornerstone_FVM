// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_core.h"
#include "Matrix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	m_pData = NULL;
	m_nRows = m_nCols = 0;
}

CMatrix::CMatrix(int nRows, int nCols)
{
	m_nRows = nRows;
	m_nCols = nCols;
	m_pData = new double[m_nRows*m_nCols];
}

CMatrix::CMatrix(int nRows, int nCols, double val)
{
	m_nRows = nRows;
	m_nCols = nCols;
	m_pData = new double[m_nRows*m_nCols];
	Fill(val);
}

CMatrix::CMatrix(int nRows, int nCols, double* values)
{
	m_nRows = nRows;
	m_nCols = nCols;

	int nSize = m_nRows*m_nCols;
	m_pData = new double[nSize];

	memcpy(m_pData, values, nSize*sizeof(double));
}

CMatrix::CMatrix(int nRows, int nCols, double** values)
{
	m_nRows = nRows;
	m_nCols = nCols;

	m_pData = new double[m_nRows*m_nCols];
	//for(int y=0; y<m_nRows; y++)
	//	memcpy( &m_pData[y*m_nCols], values[y], m_nCols*sizeof(double));
}

CMatrix::CMatrix(CMatrix& src)
{
	m_nRows = src.m_nRows;
	m_nCols = src.m_nCols;
	int nSize = m_nRows*m_nCols;

	m_pData = new double[nSize];
	memcpy(m_pData, src.m_pData, nSize);
}

CMatrix::~CMatrix()
{
	if(m_pData)
		delete [] m_pData;
}

void CMatrix::Fill(double val)
{
	for(int i=0; i<m_nRows; i++)
		for(int j=0; j<m_nCols; j++)
			m_pData[i*m_nCols+j] = val;
}

inline double CMatrix::operator() (int y, int x)
{
	return m_pData[y*m_nCols+x];
}