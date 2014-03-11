#include "CMathHelper.h"

CMathHelper::CMathHelper(void)
{
}

CMathHelper::~CMathHelper(void)
{
}

CMathHelper CMathHelper::instance = CMathHelper();

//get an instance of CMathHelper
CMathHelper CMathHelper::getInstance()
{
	return instance;
}

//calculate the sum of sample numbers
double CMathHelper::sum(double *sample, int len)
{
	double sum = 0;
	for(int i = 0; i < len; ++i)
	{
		sum += sample[i];
	}
	return sum;
}

//calculate the average of sample numbers
double CMathHelper::average(double *sample, int len)
{
	return this->sum(sample,len)/len;
}

//calculate the variance of sample numbers
double CMathHelper::variance(double *sample, int len)
{
	double nvari = 0; // the value of N*N*variance
	double sum = this->sum(sample, len);
	for(int i = 0; i < len; ++i)
	{
		double temp = len * sample[i] - sum;
		nvari += temp * temp;
	}

	return nvari / (len * len);
}

//calculate the autocovariance of sample numbers
double CMathHelper::autocovariance(double *sample, int len, int k)
{
	double nacv = 0; //the value of N*autocovariance
	for(int i = k; i < len; ++i)
	{
		nacv += sample[i] * sample[i-k];
	}
	return nacv / len;
}

//calculate the autocorrelation of sample numbers
double CMathHelper::autocorrelation(double *sample, int len, int k)
{
	double acv_0 = this->autocovariance(sample, len, 0);	//the autocovariance of sample numbers when k=0
	double acv_k = this->autocovariance(sample, len, k);	//the autocovariance of sample numbers when k=k

	return acv_k / acv_0;
}

//calculate the autocorrelation of sample numbers
double CMathHelper::autocorrelation(double acv_0, double acv_k)
{
	return acv_k / acv_0;
}

//calculate the partial autocorrelation of sample numbers
double CMathHelper::partialAutocorrelation(double *autocorrelation, int len, int k)
{
	double *numerator = new double[k * k];
	double *denominator = new double[k * k];
	for(int i = 0; i < k; ++i)
	{
		for(int j = 0; j <= i; ++j)
		{
			numerator[i + j * k] = numerator[j + i * k] = denominator[i + j * k] = denominator[j + i * k] = autocorrelation[i - j];
		}
		numerator[k + i * k - 1] = autocorrelation[i + 1];
	}
	CDeterminantHelper helper;
	double returnValue = helper.calculate(numerator, k) / helper.calculate(denominator, k);
	cout<<returnValue<<",";
	return returnValue;
}

//calculate the partial autocorrelation of sample numbers
double CMathHelper::partialAutocorrelation(double *autocorrelation, int len, int i, int j, double **partialAutocorrelations)
{
	if(i == 0 && j == 0)
		return autocorrelation[i+1];
	if(i == j)
	{
		double numerator = autocorrelation[i+1];
		double denominator = 1;
		for(int k = 0; k < i; ++k)
		{
			numerator -= (autocorrelation[i - k] * partialAutocorrelations[i-1][k]);
			denominator -= (autocorrelation[k+1] * partialAutocorrelations[i-1][k]);
		}
		return numerator/denominator;
	}
	else
	{
		return (partialAutocorrelations[i-1][j] - partialAutocorrelations[i][i]*partialAutocorrelations[i-1][i-j-1]);
	}
}

