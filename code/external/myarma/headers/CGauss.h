#pragma once
#define PI 3.14159265

class CGauss
{
public:
	CGauss(void);
	~CGauss(void);
	static CGauss getInstance();
	double gauss(double variance);
private:
	static CGauss instance;
};
