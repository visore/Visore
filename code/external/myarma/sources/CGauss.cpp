#include "CGauss.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

CGauss::CGauss(void)
{
	srand((unsigned)time(NULL));
}

CGauss::~CGauss(void)
{
}

CGauss CGauss::instance = CGauss();

CGauss CGauss::getInstance()
{
	return instance;
}

double CGauss::gauss(double variance)//n0 is 2*sigma*sigma, sigma*sigma is variance
{
	double u1;
	double u0;
	double gaussnoise;
	double n0 = 2*variance;

	u0 = (double)rand()/(double)RAND_MAX;
	u1 = (double)rand()/(double)RAND_MAX;
	if ( u0 < 1e-100)
	{
		u0 = 1e-100;
	}
	gaussnoise = sqrt( -n0*log(u0) )*cos( 2*PI*u1 );
	return gaussnoise;
}
