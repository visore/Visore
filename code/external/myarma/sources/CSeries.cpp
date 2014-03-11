#include "CSeries.h"
#include <stdio.h>

CSeries::CSeries(void)
{
	this->times = 0;
}

CSeries::~CSeries(void)
{
}

const int CSeries::floor[15][15] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2},
	{0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{0, 0, 0, 0, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3},
	{0, 0, 0, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4},
	{0, 0, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5},
	{0, 0, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 5, 5, 6},
	{0, 0, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 6},
	{0, 0, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7},
	{0, 0, 2, 3, 3, 4, 5, 5, 5, 6, 6, 7, 7, 7, 7},
	{0, 0, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8},
	{0, 2, 2, 3, 4, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8},
	{0, 2, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9},
	{0, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 8, 9, 9, 9},
	{0, 2, 3, 3, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10}
				};
const int CSeries::ceil[15][15] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 9,10,10,11,11, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 9,10,11,12,12,13,13,13,13, 0, 0, 0},
	{0, 0, 0, 0,11,12,13,13,14,14,14,14,15,15,15},
	{0, 0, 0, 0,11,12,13,14,14,15,15,16,16,16,16},
	{0, 0, 0, 0, 0,13,14,14,15,16,16,16,17,17,17},
	{0, 0, 0, 0, 0,13,14,15,16,16,17,17,18,18,18},
	{0, 0, 0, 0, 0,13,14,15,15,17,17,18,19,19,19},
	{0, 0, 0, 0, 0,13,14,16,16,17,18,19,19,20,20},
	{0, 0, 0, 0, 0, 0,15,16,17,18,19,19,20,20,21},
	{0, 0, 0, 0, 0, 0,15,16,17,18,19,20,20,21,22},
	{0, 0, 0, 0, 0, 0,15,16,18,18,19,20,21,22,22}
				};


int CSeries::getTimes()
{
	return this->times;
}

double* CSeries::toStationary(double *sample,  int length)
{
	while(/*this->stationaryChecker(sample + times,  length - times) == 0 ||*/ times<2){
		
		times++;
		for(int i = length - 1; i >= times; --i)
		{
			sample[i] = sample[i] - sample[i-1];
		}
		cout<<times<<":"<<endl;
		for(int i = 0; i<length; ++i)
		{
			cout<<sample[i]<<",";
		}
		cout<<endl;
		this->stationaryChecker(sample + times,  length - times);
	}
	cout<<"Æ½ÎÈ»¯ºó£º";
	cout<<"times:"<<times<<endl;
	for(int i = 0; i< length; ++i)
	{
		cout<<sample[i]<<", ";
	}
	cout<<endl;
	return sample;
}

//to make the average of the series equal 0
void CSeries::averageTo0(double* sample,  int length)
{
	double average = CMathHelper::getInstance().average(sample,  length);
	//double* result = (double*)malloc(length * sizeof(double));

	for (int i = 0; i < length; ++i)
	{
		sample[i] = sample[i] - average;
	}
}

int CSeries::stationaryChecker(double* sample,  int length)
{
	int n_positive,  n_negative,  n;
	int n_way;
	double E_way;
	double var_way;
	int flag;
	double z;
	double average = CMathHelper::getInstance().average(sample, length);
	n_negative = n_positive = 0;
	n_way = 1;
	flag = (sample[0] >= average);

	for (int i = 0; i < length; ++i)
	{
		if(sample[i] >= average)
		{
			cout<<"+";
			n_positive++;
			if(flag == 0)
			{
				n_way ++;
				flag = 1;
			}
		}
		else if (sample[i] < average)
		{
			cout<<"-";
			n_negative++;
			if (flag == 1)
			{
				n_way ++;
				flag = 0;
			}
		}
	}
	cout<<endl;
	if(n_negative <= 15 && n_positive <= 15)
	{
		return CSeries::stationaryTable(n_positive, n_negative, n_way);
	}

	n = n_negative + n_positive;
	double temp = 2.0 * n_negative * n_positive;
	E_way = temp / n + 1;
	var_way = sqrt(temp * (temp - n) / (n * n * (n - 1)));
	z = (n_way - E_way) / var_way;
	if(abs(z) <= 1.96)
		return 1;
	else 
		return 0;
}

int CSeries::stationaryTable(int n_positive,  int n_negative,  int n_way)
{
	if(CSeries::ceil[n_positive-1][n_negative-1] == 0 || CSeries::floor[n_positive-1][n_negative-1] == 0)
		return 0;
	else if(n_way >= CSeries::floor[n_positive-1][n_negative-1] && n_way <= CSeries::ceil[n_positive-1][n_negative-1])
		return 1;
	else
		return 0;
}
