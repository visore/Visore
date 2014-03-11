#pragma once
#include "CDeterminantHelper.h"
#include <iostream>
using namespace std;
class CMathHelper
{
public:
	static CMathHelper getInstance();
	CMathHelper(void);
	~CMathHelper(void);
	double sum(double* sample, int len);
	double variance(double* sample, int len);
	double average(double *sample, int len);
	double autocovariance(double *sample, int len, int k);
	double autocorrelation(double *sample, int len, int k);
	double autocorrelation(double acv_0, double acv_k);
	double partialAutocorrelation(double *autocorrelation, int len, int k);
    double partialAutocorrelation(double *autocorrelation, int len, int i, int j, double **partialAutocorrelations);

private:
	static CMathHelper instance;
};
