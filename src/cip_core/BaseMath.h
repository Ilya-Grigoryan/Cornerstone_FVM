// Math.h: interface for the CMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATH_H__C9140AA3_9955_4D71_B812_20B339F80D08__INCLUDED_)
#define AFX_MATH_H__C9140AA3_9955_4D71_B812_20B339F80D08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "afxtempl.h"
#include "matrix.h"

//////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CRegression
{
public:
	double*	  V;            // Least squares and var/covar matrix
    double*  C;				// Coefficients
    double*  SEC;			// Std Error of coefficients
    double RYSQ;            // Multiple correlation coefficient
    double SDV;             // Standard deviation of errors
    double FReg;            // Fisher F statistic for regression
    double*  Ycalc;         // Calculated values of Y
    double*  DY;            // Residual values of Y
	double RSS;				// Residual Sum of Squares
	int NDF;	            // Degrees of freedom

public:
	bool Regress(double* Y, double* X, double* W, int M, int N)
    {
        // Y[j]   = j-th observed data point
        // X[i,j] = j-th value of the i-th independent varialble
        // W[j]   = j-th weight value
		
		// M = Number of data points	
        // N = Number of linear terms	

        NDF = M - N;              // Degrees of freedom
        Ycalc = new double[M];
        DY = new double[M];
        // If not enough data, don't attempt regression
        if (NDF < 1)
        {
            return false;
        }
        V = new double[N*N];
        C = new double[N];
        SEC = new double[N];
        double* B = new double[N];   // Vector for LSQ

        // Clear the matrices to start out
        for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            V[i*N + j] = 0;

        // Form Least Squares Matrix
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                V[i*N + j] = 0;
                for (int k = 0; k < M; k++)
                    V[i*N + j] = V[i*N + j] + W[k] * X[i*M + k] * X[j*M + k];
            }
            B[i] = 0;
            for (int k = 0; k < M; k++)
                B[i] = B[i] + W[k] * X[i*M + k] * Y[k];
        }
        
		// V now contains the raw least squares matrix
        if (!SymmetricMatrixInvert(V, N))
            return false;

        // V now contains the inverted least square matrix
        // Matrix multpily to get coefficients C = VB
        for (int i = 0; i < N; i++)
        {
            C[i] = 0;
            for (int j = 0; j < N; j++)
                C[i] = C[i] + V[i*N + j] * B[j];
        }

        // Calculate statistics
		RSS = 0;
        double TSS = 0;
        double YBAR = 0;
        double WSUM = 0;
        for (int k = 0; k < M; k++)
        {
            YBAR = YBAR + W[k] * Y[k];
            WSUM = WSUM + W[k];
        }
        YBAR = YBAR / WSUM;
        for (int k = 0; k < M; k++)
        {
            Ycalc[k] = 0;
            for (int i = 0; i < N; i++)
                Ycalc[k] = Ycalc[k] + C[i] * X[i*M + k];
            DY[k] = Ycalc[k] - Y[k];
            TSS = TSS + W[k] * (Y[k] - YBAR) * (Y[k] - YBAR);
            RSS = RSS + W[k] * DY[k] * DY[k];
        }
        double SSQ = RSS / NDF;
        RYSQ = 1 - RSS / TSS;
        FReg = 9999999;
        if (RYSQ < 0.9999999)
            FReg = RYSQ / (1 - RYSQ) * NDF / (N - 1);
        SDV = sqrt(SSQ);

        // Calculate var-covar matrix and std error of coefficients
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                V[i*N + j] = V[i*N + j] * SSQ;
            SEC[i] = sqrt(V[i*N + i]);
        }
        return true;
    }

	bool SymmetricMatrixInvert(double* V, int N)
    {
        double* t = new double[N];
        double* Q = new double[N];
        double* R = new double[N];
        double AB;
        int K, L, M;

        // Invert a symetric matrix in V
        for (M = 0; M < N; M++)
            R[M] = 1;
        K = 0;
        for (M = 0; M < N; M++)
        {
            double Big = 0;
            for (L = 0; L < N; L++)
            {
                AB = fabs( V[L*N + L] );
                if ((AB > Big) && (R[L] != 0))
                {
                    Big = AB;
                    K = L;
                }
            }
            if (Big == 0)
            {
                return false;
            }
            R[K] = 0;
            Q[K] = 1 / V[K*N + K];
            t[K] = 1;
            V[K*N + K] = 0;
            if (K != 0)
            {
                for (L = 0; L < K; L++)
                {
                    t[L] = V[L*N + K];
                    if (R[L] == 0)
                        Q[L] = V[L*N + K] * Q[K];
                    else
                        Q[L] = -V[L*N + K] * Q[K];
                    V[L*N + K] = 0;
                }
            }
            if ((K + 1) < N)
            {
                for (L = K + 1; L < N; L++)
                {
                    if (R[L] != 0)
                        t[L] = V[K*N + L];
                    else
                        t[L] = -V[K*N + L];
                    Q[L] = -V[K*N + L] * Q[K];
                    V[K*N + L] = 0;
                }
            }
            for (L = 0; L < N; L++)
                for (K = L; K < N; K++)
                    V[L*N + K] = V[L*N + K] + t[L] * Q[K];
        }
        M = N;
        L = N - 1;
        for (K = 1; K < N; K++)
        {
            M = M - 1;
            L = L - 1;
            for (int J = 0; J <= L; J++)
                V[M*N + J] = V[J*N + M];
        }
        return true;
    }

	void matrix_inverse(double *Min, double *Mout, int actualsize) 
	{
		/* This function calculates the inverse of a square matrix
		 *
		 * matrix_inverse(double *Min, double *Mout, int actualsize)
		 *
		 * Min : Pointer to Input square Double Matrix
		 * Mout : Pointer to Output (empty) memory space with size of Min
		 * actualsize : The number of rows/columns
		 *
		 * Notes:
		 *  - the matrix must be invertible
		 *  - there's no pivoting of rows or columns, hence,
		 *        accuracy might not be adequate for your needs.
		 *
		 * Code is rewritten from c++ template code Mike Dinolfo
		 */
		/* Loop variables */
		int i, j, k;
		/* Sum variables */
		double sum,x;
    
		/*  Copy the input matrix to output matrix */
		for(i=0; i<actualsize*actualsize; i++) { Mout[i]=Min[i]; }
    
		/* Add small value to diagonal if diagonal is zero */
		for(i=0; i<actualsize; i++)
		{ 
			j=i*actualsize+i;
			if((Mout[j]<1e-12)&&(Mout[j]>-1e-12)){ Mout[j]=1e-12; }
		}
    
		/* Matrix size must be larger than one */
		if (actualsize <= 1) return;
    
		for (i=1; i < actualsize; i++) {
			Mout[i] /= Mout[0]; /* normalize row 0 */
		}
    
		for (i=1; i < actualsize; i++)  {
			for (j=i; j < actualsize; j++)  { /* do a column of L */
				sum = 0.0;
				for (k = 0; k < i; k++) {
					sum += Mout[j*actualsize+k] * Mout[k*actualsize+i];
				}
				Mout[j*actualsize+i] -= sum;
			}
			if (i == actualsize-1) continue;
			for (j=i+1; j < actualsize; j++)  {  /* do a row of U */
				sum = 0.0;
				for (k = 0; k < i; k++) {
					sum += Mout[i*actualsize+k]*Mout[k*actualsize+j];
				}
				Mout[i*actualsize+j] = (Mout[i*actualsize+j]-sum) / Mout[i*actualsize+i];
			}
		}
		for ( i = 0; i < actualsize; i++ )  /* invert L */ {
			for ( j = i; j < actualsize; j++ )  {
				x = 1.0;
				if ( i != j ) {
					x = 0.0;
					for ( k = i; k < j; k++ ) {
						x -= Mout[j*actualsize+k]*Mout[k*actualsize+i];
					}
				}
				Mout[j*actualsize+i] = x / Mout[j*actualsize+j];
			}
		}
		for ( i = 0; i < actualsize; i++ ) /* invert U */ {
			for ( j = i; j < actualsize; j++ )  {
				if ( i == j ) continue;
				sum = 0.0;
				for ( k = i; k < j; k++ ) {
					sum += Mout[k*actualsize+j]*( (i==k) ? 1.0 : Mout[i*actualsize+k] );
				}
				Mout[i*actualsize+j] = -sum;
			}
		}
		for ( i = 0; i < actualsize; i++ ) /* final inversion */ {
			for ( j = 0; j < actualsize; j++ )  {
				sum = 0.0;
				for ( k = ((i>j)?i:j); k < actualsize; k++ ) {
					sum += ((j==k)?1.0:Mout[j*actualsize+k])*Mout[k*actualsize+i];
				}
				Mout[j*actualsize+i] = sum;
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CMath  
{
public:
	CMath();
	~CMath();

	static BOOL	IsInf(double val)		{ return val > _INF_;	}

	static double GetMean(double* values, unsigned int size);
	static double GetMean(CArray<double,double>& values);

	static double GetVariance(double* values, unsigned int size);
	static double GetVariance(CArray<double,double>& values);

	static double GetStDev(double* values, unsigned int size);
	static double GetStDev(CArray<double,double>& values);

	static double GetCoeffVar(double* values, unsigned int size);
	static double GetCoeffVar(CArray<double, double>& values);	

	static double GetCovariance(double* px, double *py, unsigned int size);
	static double GetCovariance(CArray<double, double> &x, CArray<double, double> &y);
	
	static double GetCorrelation(double* px, double *py, unsigned int size);
	static double GetCorrelation(CArray<double, double> &x, CArray<double, double> &y);

	static double GetPresentValue(double fAmountEachPeriod, double fDiscountRate, double fPeriods);
	static double GetPresentValue(double *pPayments, int nCount, double fDiscountRate);
	static double GetIRR(double fInvestment, double *pPayments, int nCount, double fGuess=0.1);

	static void SimpleRegression(double* px, double* py, unsigned int size, double& slope, double& intercept, double& coeff);

	///////////////////////////////////////////////////////////////////////////////////////////////// 
	// random numbers and distributions
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// these are used for random number generation 
	static inline void		InitRandomSeeds(long nValue);
	static inline void		SetRandomSeed(long nValue);
	static inline long		GetRandomSeed();
	
	static inline double	GetDistribNormal(double fMean, double fStdev);
	static void				GetDistribNormal(double fMean, double fStdev, double *values, int size);

	static void				GetHistogram(double* pData, int nDataSize, int nBarCount, double* pValueRanges, int* pProbs);

protected:
	static long		m_nRandomSeed;
};

#endif // !defined(AFX_MATH_H__C9140AA3_9955_4D71_B812_20B339F80D08__INCLUDED_)
