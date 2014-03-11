#include "CArma.h"

CArma::CArma(void)
{
    this->partialAutocorrelations = NULL;
    this->autocorrelations = NULL;
    this->autocovariances = NULL;

    this->arA = NULL;
    this->arB = NULL;

    this->maIta = NULL;
    this->maTemp = NULL;
    this->maR = NULL;
    this->maF1 = NULL;
    this->maF2 = NULL;

    setWindowSize(LENGTH);
}

CArma::~CArma(void)
{
    clearWindowSize();
    clearOrder();
}

Arma CArma::model()
{
    //series.averageTo0(sample, this->len);
    this->calAutocorrelations();
    this->calPartialAutocorrelations();
    Arma arma = this->recogniseModelType();
    if (arma.modelType == 1)			//MA(m)
    {
        arma.covariance = this->paraEstimate_ma(arma.factors,arma.ma);
    }
    else if(arma.modelType == 2)		//AR(n)
    {
        arma.covariance = this->paraEstimate_ma(arma.factors,arma.ar);
    }
    else						//ARMA(n,m)
    {
        this->paraEstimate_ma(arma.factors,arma.ar);
        arma.covariance = this->paraEstimate_ma(arma.factors + arma.ar,arma.ma);
    }
    return arma;
}

Arma CArma::model(int type)
{
    this->calAutocorrelations();
    //this->calPartialAutocorrelations();

    Arma arma;
    arma.ar = order;
    arma.ma = order-1;

    if (type == 1)			//MA(m)
    {
        arma.covariance = this->paraEstimate_ma(arma.factors,arma.ma);
    }
    else if(type == 2)		//AR(n)
    {
        arma.covariance = this->paraEstimate_ma(arma.factors,arma.ar);
    }
    else						//ARMA(n,m)
    {
        this->paraEstimate_ma(arma.factors,arma.ar);
        arma.covariance = this->paraEstimate_ma(arma.factors + arma.ar,arma.ma);
    }
    return arma;
}

//print something of the result for test
void CArma::print()
{
	cout<<"sum:\t"<<CMathHelper::getInstance().sum(this->sample, this->len)<<endl;
	cout<<"average:\t"<<CMathHelper::getInstance().average(this->sample, this->len)<<endl;
	cout<<"variance:\t"<<CMathHelper::getInstance().variance(this->sample, this->len)<<endl;
	cout<<"autocovariance:\t"<<CMathHelper::getInstance().autocovariance(this->sample, this->len, 0)<<endl;
	cout<<"autocovariance:\t"<<CMathHelper::getInstance().autocovariance(this->sample, this->len, 3)<<endl;
	this->calAutocovariances();
	this->calAutocorrelations();
	cout<<"autocorrelation:\t";
	for(int i = 0; i < this->len; ++i)
	{
		cout<<this->autocorrelations[i]<<",";
	}
	cout<<endl;
	this->calPartialAutocorrelations();
	cout<<"partialAutocorrelation:\t";
	for(int i = 0; i < this->len; ++i)
	{
        cout<<this->partialAutocorrelations[i][i]<<",";
	}
	cout<<endl;

	cout<<this->recogniseModelType().modelType<<endl;
	this->model();
}

//calculate the autocovariances of sample numbers from k=0 to k=N-1
void CArma::calAutocovariances()
{
    static int i;
    for(i = 0; i < this->len; ++i)
	{
		this->autocovariances[i] = CMathHelper::getInstance().autocovariance(this->sample, this->len, i);
	}
}

//calculate the autocorrelations of sample numbers from k=0 to k=N-1
void CArma::calAutocorrelations()
{
    static int i;
    for(i = 0; i < this->len; ++i)
	{
		this->autocorrelations[i] = CMathHelper::getInstance().autocorrelation(this->autocovariances[0], this->autocovariances[i]);
	}
}

//calculate the partial autocorrelations of sample numbers from k=0 to k=N-1
void CArma::calPartialAutocorrelations()
{
    static int i, j;
    for(i = 0; i < this->len; ++i)
    {
        this->partialAutocorrelations[i][i] = CMathHelper::getInstance().partialAutocorrelation(this->autocorrelations, this->len, i, i, this->partialAutocorrelations);
        for(j = 0; j < i; ++j)
        {
            this->partialAutocorrelations[i][j] = CMathHelper::getInstance().partialAutocorrelation(this->autocorrelations, this->len, i, j, this->partialAutocorrelations);
		}
    }
}

Arma CArma::recogniseModelType()
{
	Arma arma;
	int i;
	double variance1, variance2;
    double pa[this->windowSize];
	for(int k = 0; k < this->len; ++k)
	{
        pa[k] = this->partialAutocorrelations[k][k];
	}
	for (i = 1; i < len; ++i)
	{
		variance1 = 1;
		for (int j = 0; j < i; ++j)
		{
			variance1 += 2 * this->autocorrelations[i] * this->autocorrelations[i];
		}
		variance1 /= this->len;
		variance2 = 1 / this->len;
		
		if (CDistributionChecker::getInstance().isNormalDistribution(pa + i - 1,this->len - i + 1, variance2, 0))
        {
			arma.modelType = 2;
			arma.ar = i;
			break;
		}

		if (CDistributionChecker::getInstance().isNormalDistribution(this->autocorrelations + i,this->len - i, variance1,0))
        {
			arma.modelType = 1;
			arma.ma = i - 1;
			break;
        }
		
	}
	return arma;
}

double CArma::paraEstimate_ar(double *paras, int order)
{
    static double var;
    static int i, j;

    for(i = 0; i < order; i++)
	{
        for(j = 0; j < i; ++j)
		{
            arA[order*i + j] = arB[order*j + i] = this->autocorrelations[i - j];
		}
	}
    CMatrixHelper::getInstance().Invert(arA, order);
    CMatrixHelper::getInstance().Multiply(arA,order,arB,paras);

	var = this->autocovariances[0];
    for(int i = 0; i <= order; ++i)
    {
		var -= paras[i]*this->autocovariances[i];
	}
	return var;
}

double CArma::paraEstimate_ma(double *paras, int order)
{
    static double var, dis;
    static int i, j, rpt;
	
    for(i = 0; i <= order; ++i)
	{
        maIta[i] = 0.1;
        maR[i] = this->autocovariances[i];
	}	

	for(rpt = 0; rpt < MAXRPT; rpt++)
	{
        for(i = 0; i <= order; ++i)
		{
            for(j = 0; j < order; ++j)
			{
                if(j <= order - i) maF1[i*order+j] = maIta[(i+j)%order];
                else maF1[i*order+j] = 0;

                if(i <= j)maF2[i*order+j] = maIta[j-i];
                else maF2[i*order+j] = 0;
			}
		}

        CMatrixHelper::getInstance().Add(maF1, order+1, order+1, maF2, maF1);
        CMatrixHelper::getInstance().Invert(maF1, order+1);
        CMatrixHelper::getInstance().Multiply(maF1, order+1, maR, maTemp);
        CMatrixHelper::getInstance().Minus(maIta, order+1, 1, maTemp, maTemp);

        dis = CMatrixHelper::getInstance().Distance(maIta, order+1, maTemp);
        if(dis <= MINDIS) break;
        else CMatrixHelper::getInstance().Copy(maTemp, order+1, 1, maIta);
	}
    if(rpt < MAXRPT)
    {
        var = maTemp[0] * maTemp[0];
        for(i = 1; i <= order; ++i)
		{
            paras[i-1] = maTemp[i]/maTemp[0];
		}
	}
    //else cout<<"no answer!"<<endl;
	return var;
}

double CArma::getQ(Arma arma)
{
    /*static double estiValue, Q;
    static int i, ar, ma;

    estiValue = 0;
    Q = 0;

    for(i = arma.ar; i < this->len; ++i)
	{
        for(ar = 0; ar < arma.ar; ++ar)
		{
			estiValue += arma.factors[ar]*sample[i-ar-1];
		}
		estiValue -= CGauss::getInstance().gauss(arma.covariance);
        for(ma = 0; ma < arma.ma; ++ma)
		{
			estiValue -= arma.factors[arma.ar + ma]*CGauss::getInstance().gauss(arma.covariance);
		}
        Q += (sample[i]-estiValue) * (sample[i]-estiValue);
	}
    Q /= ((this->len - arma.ar) - (arma.ar + arma.ma + 1));
    return Q;*/

   /* static double estiValue, Q;
        static int i, ar, ma;

        estiValue = 0;
        Q = 0;


            for(ar = 0; ar < arma.ar; ++ar)
            {
                estiValue += arma.factors[ar]*sample[ar-1];
            }
            estiValue += CGauss::getInstance().gauss(arma.covariance);
            for(ma = 0; ma < arma.ma; ++ma)
            {
                estiValue += arma.factors[arma.ar + ma]*CGauss::getInstance().gauss(arma.covariance);
            }

            return estiValue;*/

    static double estiValue, Q;
           static int i, ar, ma;

           estiValue = 0;
           Q = 0;


               for(ar = 0; ar < arma.ar; ++ar)
               {
                   estiValue += arma.factors[ar]*sample[len-ar-1];
               }
               //cout<<estiValue<<" "<<arma.factors[0]<<endl;
               estiValue += CGauss::getInstance().gauss(arma.covariance);
               for(ma = 0; ma < arma.ma; ++ma)
               {
                   estiValue += arma.factors[arma.ar + ma]*CGauss::getInstance().gauss(arma.covariance);
               }

               return estiValue;

}

void CArma::setSamples(double *sample, int len)
{
    this->sample = sample;
    this->len = len;
}

void CArma::setOrder(int order)
{
    clearOrder();
    this->order = order;
    initializeOrder();
}

void CArma::setWindowSize(int size)
{
    clearWindowSize();
    this->windowSize = size;
    initializeWindowSize();
}

void CArma::initializeOrder()
{
    arA = new double[order*order];
    arB = new double[order];

    maIta = new double[order+1];
    maTemp = new double[order+1];
    maR = new double[order + 1];
    maF1 = new double[(order+1)*(order+1)];
    maF2 = new double[(order+1)*(order+1)];
}

void CArma::clearOrder()
{
    if(this->arA != NULL)
    {
        delete [] this->arA;
        this->arA = NULL;
    }
    if(this->arB != NULL)
    {
        delete [] this->arB;
        this->arB = NULL;
    }

    if(this->maIta != NULL)
    {
        delete [] this->maIta;
        this->maIta = NULL;
    }
    if(this->maTemp != NULL)
    {
        delete [] this->maTemp;
        this->maTemp = NULL;
    }
    if(this->maR != NULL)
    {
        delete [] this->maR;
        this->maR = NULL;
    }
    if(this->maF1 != NULL)
    {
        delete [] this->maF1;
        this->maF1 = NULL;
    }
    if(this->maF2 != NULL)
    {
        delete [] this->maF2;
        this->maF2 = NULL;
    }
}

void CArma::initializeWindowSize()
{
    this->partialAutocorrelations = new double *[this->windowSize];
    for(int i = 0; i < this->windowSize; ++i)
    {
        this->partialAutocorrelations[i] = new double[this->windowSize];
    }
    this->autocorrelations = new double[this->windowSize];
    this->autocovariances = new double[this->windowSize];
}

void CArma::clearWindowSize()
{
    if(this->partialAutocorrelations != NULL)
    {
        for(int i = 0; i < this->windowSize; ++i)
        {
            delete [] this->partialAutocorrelations[i];
        }
        delete [] this->partialAutocorrelations;
        this->partialAutocorrelations = NULL;
    }
    if(this->autocorrelations != NULL)
    {
        delete [] this->autocorrelations;
        this->autocorrelations = NULL;
    }
    if(this->autocovariances != NULL)
    {
        delete [] this->autocovariances;
        this->autocovariances = NULL;
    }
}

double CArma::predict()
{
    return getQ(model(3));
}
