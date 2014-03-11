#include "CDeterminantHelper.h"

CDeterminantHelper::CDeterminantHelper(void)
{
}

CDeterminantHelper::~CDeterminantHelper(void)
{
}

//calculate the value of the determinant
double CDeterminantHelper::calculate(double *determinant, int len)
{
	//return 0.0;
	if(len < 1)
    {
        return -1;
    }

    if(len == 1)
    {
        return *determinant;
    }
    
    double *curr=new double[(len-1)*(len-1)];
    double ans=0;
    int i,r,c,cc;
    
    for(i=1;i<=len;++i)
    {
        for(r=2;r<=len;++r)
        {
            cc=-1;
            for(c=1;c<=len;++c)
            {
                if(c!=i)
                {
                    *(curr+(r-2)*(len-1)+(++cc))=*(determinant+(r-1)*len+(c-1));
                }
            }
        }
        ans+=*(determinant+i-1)*pow((double)-1,1+i)*calculate(curr,len-1);
    }
    
    delete [] curr;
    return ans;
}
