#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vibenchmarker2.h>
#include<vibenchmarker3.h>
#include<vibenchmarker4.h>
#include <vipredictorbenchmarker.h>

#include <vidifferentiater.h>

bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	// http://www.math.usm.edu/lambers/mat772/fall10/lecture6.pdf
	// http://bruce-shapiro.com/math481A/notes/19-Hermite-interpolation.pdf
	// http://www.phys.ufl.edu/~coldwell/wsteve/FDERS/The%20Lagrange%20Polynomial.htm

	static int i, j, m, k;
	qreal x;
	static qreal result;
	static qreal totalDevider; // qreal, since we devide

	qreal scaledx, scaledi, scaledm, scaledk;
	qreal value, value1, lagrangeSquared, lagrangeFirst, lagrangeFirst1;

	totalDevider = size + predictionCount - 1;
	//totalDevider=1;

	bool error;



	for(j = 0; j < predictionCount; ++j)
	{
		x = size + j;
		//x=size-1+0.5;
		scaledx=x/totalDevider;

		result = 0;
		for(i = 0; i < size; ++i)
		{
			scaledi=i/totalDevider;
			// Lagrange
			lagrangeSquared = 1;
			for(m = 0; m < size; ++m)
			{
				scaledm=m/totalDevider;
				if(i != m) lagrangeSquared *= (scaledx - scaledm) / qreal(scaledi - scaledm);
			}
			//lagrangeSquared = lagrangeSquared * lagrangeSquared;

			/*if(i==0)
			{
				lagrangeSquared = (x - 1) / (-1);
				lagrangeFirst = -1;
			}
			else if(i==1)
			{
				lagrangeSquared = x;
				lagrangeFirst = 1;
			}*/



			// Lagrange first derivative
			lagrangeFirst = 0;


			/*for(m = 0; m < size; ++m)
			{
				if(i != m) lagrangeFirst += 1 /qreal(i-m);
			}*/
cout<<"******: "<<size<<endl;
			lagrangeFirst = 1 ;
			for(m = 0; m < size; ++m)
			{
				scaledm=m/totalDevider;
				if(i != m)
				{
					//lagrangeFirst1 = 1 / qreal(scaledi - scaledk);
					lagrangeFirst *= 1 / qreal(scaledi - scaledm);




					/*for(m = 0; m < size; ++m)
					{
						scaledm=m/totalDevider;
						if(m!=k) lagrangeFirst1 *=  +
						//if(i != m) lagrangeFirst1 *= (x - m) / (i - m);
					}*/

					//lagrangeFirst += lagrangeFirst1;
				}
				//cout<<"******xxxxxxx: "<<lagrangeFirst<<"\t"<<1 / qreal(scaledi - scaledm)<<"\t"<<scaledi<<"\t"<<scaledm<<endl;
			}
			if(size==3)
			{
				lagrangeFirst1 = (2*scaledi);
				for(k = 0; k < size; ++k)
				{
					scaledk=k/totalDevider;
					if(k!=i) lagrangeFirst1 -= scaledk;
				}
				lagrangeFirst *= lagrangeFirst1;
			}
			else if(size==4)
			{
				lagrangeFirst1 = (3*scaledi*scaledi);

				qreal t = 0;
				for(int u = 0; u < size; ++u)
				{
					if(i!=u)t+=u/totalDevider;
				}
				lagrangeFirst1 += -2*scaledi*(t);

				for(int u = 0; u < size; ++u)
				{
					for(int u2 = u+1; u2 < size; ++u2)
					{
						if(u != u2 && u != i && u2 != i) lagrangeFirst1 += (u/totalDevider)*(u2/totalDevider);
					}
				}

				lagrangeFirst *= lagrangeFirst1;
			}



cout<<"-----------: "<<lagrangeFirst<<endl;
			// Normal polynomial (first part)
			//value = lagrangeSquared * (1 - (2 * lagrangeFirst * (scaledx-scaledi)));
			//result += samples[i] * value;

			// First derivative (second part)
			//value1 = lagrangeSquared  * (scaledx-scaledi);
			//result += (ViDifferentiate::derivative(1, samples, size, i, error)/totalDevider) * value1;

			result += (samples[i] * lagrangeSquared) + (lagrangeFirst * (ViDifferentiate::derivative(1, samples, size, i, error)/totalDevider));

			//cout<<value<<"\t"<<x<<"\t"<<i<<"\t"<<lagrangeSquared<<"\t"<<lagrangeFirst<<"\t"<<result<<endl;
		}
		predictedSamples[j] = result;
	}

	return true;
}

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	QIcon logo("logo.png");
	application.setWindowIcon(logo);
	application.setOrganizationName(ViManager::name());
	application.setOrganizationDomain(ViManager::url().toString());
	application.setApplicationName(ViManager::name());
	application.setApplicationVersion(ViManager::version().toString());

	// Set random seed
	srand(time(NULL));

	//Register SIGNAL/SLOT parameter types
	qRegisterMetaType<ViAudioPosition>("ViAudioPosition");
	qRegisterMetaType<ViRealSpectrum>("ViRealSpectrum");

    bool BENCH = true;
    //bool BENCH = false;

	//ViBenchMarker4 bench;
	ViPredictorBenchmarker bench;
	if(BENCH)
	{
		/*int size = 5;
		qreal s[] = {1,2,3,4,5};
		//int size = 2;
		//qreal s[] = {1,4};

		int psize = 2;
		qreal p[2];

		for(int j = 1; j <=4; ++j)
		{
			cout<<"+++++++++++++: "<<j<<endl;
			predict(s, j, p, psize);
			for(int i =0;i<psize;++i)cout<<p[i]<<" ["<<s[j-1]+i<<", "<<s[j-1]+1+i<<"]"<<endl;
			cout<<"+++++++++++++: "<<j<<endl;
		}


		exit(-1);*/


		bench.benchmark();
	}
	else
	{
		ViMainWindow *window = ViMainWindow::instance();
		window->setWindowIcon(logo);
		window->show();
	}
	
	application.exec();

	return 0;
}

