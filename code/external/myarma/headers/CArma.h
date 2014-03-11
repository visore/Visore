#pragma once
#define LENGTH 100
#define MAXRPT 1//1000
#define MINDIS 1
#include <iostream>
#include "CMathHelper.h"
#include "CDeterminantHelper.h"
#include "CDistributionChecker.h"
#include "CSeries.h"
#include "CMatrixHelper.h"
#include "CGauss.h"
using namespace std;

typedef struct Arma
{
	int modelType;
	int ar;
	int ma;
	double factors[10];
	double covariance;

public:
	Arma()
	{
		modelType = 3;
		ar = 0; 
		ma = 0;
		factors[0] = 1;
	}
} Arma ;

class CArma
{
private:

    int windowSize;
	int len;
    int order;

    double *sample;
    double *autocorrelations;
    double **partialAutocorrelations;
    double *autocovariances;

    double *arA;
    double *arB;
    double *maIta;
    double *maTemp;
    double *maR;
    double *maF1;
    double *maF2;

private:

    Arma model();
    Arma model(int type); // 1 = MA, 2 = AR, other = ARMA
    void calAutocorrelations();
    void calPartialAutocorrelations();
    void calAutocovariances();
    Arma recogniseModelType();
    double paraEstimate_ar(double* paras, int order);
    double paraEstimate_ma(double* paras, int order);
    double getQ(Arma arma);

    void initializeWindowSize();
    void clearWindowSize();

    void initializeOrder();
    void clearOrder();
	
public:

    CArma(void);
	~CArma(void);

    void setSamples(double* sample, int len);
    void setWindowSize(int size);
    void setOrder(int order);
    double predict();
	
	void print();
	
};

