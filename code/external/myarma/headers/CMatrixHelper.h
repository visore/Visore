#pragma once

class CMatrixHelper
{
private:
	static CMatrixHelper instance;
    int i, j, k;

public:
	CMatrixHelper(void);
	~CMatrixHelper(void);
	
	static CMatrixHelper getInstance();
	double* Invert(double a[],int n);
	void Multiply(double* a, int order, double* b, double* result);
	void Add(double *a, int rows, int columns, double *b, double *result);
	void Minus(double *a, int rows, int columns, double *b, double *result);
	double Distance(double* a, int order, double* b);
	void Copy(double * src, int rows, int columns, double* des);
};
