#include "CDistributionChecker.h"

CDistributionChecker CDistributionChecker::instance = CDistributionChecker();

CDistributionChecker CDistributionChecker::getInstance()
{
	return instance;
}

CDistributionChecker::CDistributionChecker(void)
{
}

CDistributionChecker::~CDistributionChecker(void)
{
}

int CDistributionChecker::isNormalDistribution(double* sample, int length, double variance, double expectation)
{
	double average = CMathHelper::getInstance().average(sample, length);
	double u = (average - expectation) * sqrt((double)length) / variance;
	if(abs(u) > 1.96)
		return 0;
	else
		return 1;
}
