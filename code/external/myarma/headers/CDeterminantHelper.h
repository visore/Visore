#pragma once
#include <cmath>
using namespace std;

class CDeterminantHelper
{
public:
	CDeterminantHelper(void);
	~CDeterminantHelper(void);

	double calculate(double* determinant, int len);
};
