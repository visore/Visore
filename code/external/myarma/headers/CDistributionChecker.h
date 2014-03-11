#pragma once
#include <cmath>
#include "CMathHelper.h"

class CDistributionChecker
{
public:
	CDistributionChecker(void);
	~CDistributionChecker(void);
	static CDistributionChecker getInstance();

	int isNormalDistribution(double* sample, int length, double variance, double expectation);

private:
	static CDistributionChecker instance;
};
