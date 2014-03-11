#pragma once
#include <cmath>
#include <cstdlib>
#include "CMathHelper.h"

class CSeries
{
private:
	int times;
	static int stationaryTable(int n_positive, int n_negative, int n_way);
	static const int ceil[15][15];
	static const int floor[15][15];
public:
	CSeries(void);
	~CSeries(void);

	int getTimes();

	double* toStationary(double* sample, int length);

	void averageTo0(double* sample, int length);

	int stationaryChecker(double* sample, int length);
};
