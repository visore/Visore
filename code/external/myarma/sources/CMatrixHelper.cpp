#include "CMatrixHelper.h"
#include <stdlib.h>
#include <math.h>

CMatrixHelper::CMatrixHelper(void)
{
}

CMatrixHelper::~CMatrixHelper(void)
{
}

CMatrixHelper CMatrixHelper::instance = CMatrixHelper();

//get an instance of CMatrixHelper
CMatrixHelper CMatrixHelper::getInstance()
{
	return instance;
}

double* CMatrixHelper::Invert(double a[],int n)
{
    int *is,*js,l,u,v;
	double d,p;
	is=(int*)malloc(n*sizeof(int));
	js=(int*)malloc(n*sizeof(int));

    int end = n-1;
    for (k=0; k<=end; k++)
	{ 
		d=0.0;
        for (i=k; i<=end; i++)
            for (j=k; j<=end; j++)
			{ 
				l=i*n+j; 

				p=double(fabs(double(a[l])));            
				if (p>d) 
				{ 
					d=p; 
					is[k]=i; 
					js[k]=j;
				}
			}
			if (d+1.0==1.0)
			{ 
				free(is); 
				free(js); 
                //printf("err**not inv\n");
				return(0);
			}
			if (is[k]!=k)
                for (j=0; j<=end; j++)
				{ 
					u=k*n+j; 
					v=is[k]*n+j;
					p=a[u]; 
					a[u]=a[v]; 
					a[v]=p;
				}
				if (js[k]!=k)
                    for(i=0; i<=end; i++)
					{ 
						u=i*n+k;
						v=i*n+js[k];
						p=a[u]; 
						a[u]=a[v]; 
						a[v]=p;
					}
					l=k*n+k;
					a[l]=(double)1.0/a[l];
                    for (j=0; j<=end; j++)
						if (j!=k)
						{ 
							u=k*n+j; 
							a[u]=a[u]*a[l];
						}
						for (i=0; i<=n-1; i++)
							if (i!=k)
                                for (j=0; j<=end; j++)
									if (j!=k)
									{ u=i*n+j;
						a[u]=a[u]-a[i*n+k]*a[k*n+j];
						}
                        for (i=0; i<=end; i++)
							if (i!=k)
							{ 
								u=i*n+k; 
								a[u]=-a[u]*a[l];
							}
	}

    for (k=end; k>=0; k--)
	{ 
		if (js[k]!=k)
            for (j=0; j<=end; j++)
			{ 
				u=k*n+j; 
				v=js[k]*n+j;
				p=a[u]; 
				a[u]=a[v]; 
				a[v]=p;
			}
			if (is[k]!=k)
                for (i=0; i<=end; i++)
				{ 
					u=i*n+k; 
					v=i*n+is[k];
					p=a[u]; 
					a[u]=a[v]; 
					a[v]=p;
				}
	}

	free(is); 
	free(js);

	return a;

}

void CMatrixHelper::Multiply(double* a, int order, double* b, double* result)
{
    for(i = 0; i < order; ++i)
	{
		result[i] = 0;
        for(j = 0; j < order; ++j)
		{
			result[i] += a[i * order + j] * b[j];
		}
	}
}

void CMatrixHelper::Add(double *a, int rows, int columns, double *b, double *result)
{
    int end = rows*columns;
    for(i = 0; i < end; ++i)
	{
		result[i] = a[i] + b[i];
	}
}

void CMatrixHelper::Minus(double *a, int rows, int columns, double *b, double *result)
{
    int end = rows*columns;
    for(i = 0; i < end; ++i)
	{
		result[i] = a[i] - b[i];
	}
}

double CMatrixHelper::Distance(double *a, int order, double *b)
{
	double result = 0;
    int end = order*order;
    for(i = 0; i < end; ++i)
	{
		result += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return sqrt(result);
}

void CMatrixHelper::Copy(double *src, int rows, int columns, double *des)
{
    int end = rows*columns;
    for(i = 0; i < end; ++i)
	{
		des[i] = src[i];
	}
}
