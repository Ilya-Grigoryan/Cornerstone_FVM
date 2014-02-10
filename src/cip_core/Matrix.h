// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__E9134E62_DC78_4F8B_AE12_734D30556CCC__INCLUDED_)
#define AFX_MATRIX_H__E9134E62_DC78_4F8B_AE12_734D30556CCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _INF_	((double)(0xffffffff-1))

class _declspec(dllexport) CMatrix  
{
public:
	CMatrix();
	CMatrix(int nRows, int nCols);
	CMatrix(int nRows, int nCols, double val);
	CMatrix(int nRows, int nCols, double* values);
	CMatrix(int nRows, int nCols, double** values);
	CMatrix(CMatrix& src);

	~CMatrix();

public:
	inline double get(int y, int x)					{ return m_pData[y*m_nCols+x];		}
	inline void set(int y, int x, double val)		{ m_pData[y*m_nCols+x] = val;		}

public:
	void Fill(double val);

	
	double operator() (int y, int x);

protected:
	double	*m_pData;
	int		m_nRows;
	int		m_nCols;
};

#endif // !defined(AFX_MATRIX_H__E9134E62_DC78_4F8B_AE12_734D30556CCC__INCLUDED_)
