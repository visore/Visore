#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vibenchmarker.h>
#include <vilinearinterpolator.h>
#include <viadvancedlinearinterpolator.h>
#include <vipiecewiseconstantinterpolator.h>
#include <visplineinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <vihermiteinterpolator.h>

qreal norm(qreal array[], qint32 size)
{
	qreal norm = 0;
	for(int i = 0; i < size; ++i)
	{
		norm += (array[i] * array[i]);
	}
	return qSqrt(norm);
}

void applyNorm(qreal array[], qint32 size, qreal norm)
{
	if(norm != 0)
	{
		for(int i = 0; i < size; ++i)
		{
			array[i] /= norm;
		}
	}
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

	//Register SIGNAL/SLOT parameter types
	qRegisterMetaType<ViAudioPosition>("ViAudioPosition");
	qRegisterMetaType<ViRealSpectrum>("ViRealSpectrum");

	bool BENCH = true;
	//bool BENCH = false;

	ViBenchMarker bench;
	if(BENCH)
	{

		bench.benchmark("/home/visore/Visore Projects/a2.flac");
	}
	else
	{

		/*
		s[0] = 0.5;
		s[1] = 0.4;
		s[2] = 0.3;
		s[3] = 1;
		s[4] = 1;
		s[5] = 1;
		s[6] = 0.3;
		s[7] = 0.4;
		s[8] = 0.5;
		*/

		/*
		s[0] = 0.1;
		s[1] = 0.2;
		s[2] = 0.3;
		s[3] = 1;
		s[4] = 1;
		s[5] = 1;
		s[6] = 0.3;
		s[7] = 0.2;
		s[8] = 0.1;
		*/

		/*ViSampleChunk s2(32);
		s2[0]=0.45474;
		s2[1]=0.46835;
		s2[2]=0.45020;
		s2[3]=0.38251;
		s2[4]=0.39502;
		s2[5]=0.63791;
		s2[6]=0.67032;
		s2[7]=0.76260;
		s2[8]=0.81223;
		s2[9]=0.60416;
		s2[10]=0.58548;
		s2[11]=0.51050;
		s2[12]=0.46307;
		s2[13]=0.49243;
		s2[14]=0.40735;
		s2[15]=0.50650;
		s2[16]=0.45303;
		s2[17]=0.54294;
		s2[18]=0.58569;
		s2[19]=0.53146;
		s2[20]=0.48492;
		s2[21]=0.43875;
		s2[22]=0.54388;
		s2[23]=0.63431;
		s2[24]=0.47104;
		s2[25]=0.58832;
		s2[26]=0.56326;
		s2[27]=0.47104;
		s2[28]=0.63431;
		s2[29]=0.54388;
		s2[30]=0.56326;
		s2[30]=0.53146;


		ViSampleChunk s(32);
		s[0]=0.45474;
		s[1]=0.46835;
		s[2]=0.45020;
		s[3]=0.38251;
		s[4]=0.39502;
		s[5]=0.63791;
		s[6]=0.67032;
		s[7]=0.76260;
		s[8]=0.81223;
		s[9]=0.60416;
		s[10]=0.58548;
		s[11]=0.51050;
		s[12]=0.46307;
		s[13]=0.49243;
		s[14]=0.40735;
		s[15]=0.50650;
		s[16]=0.45303;
		s[17]=0.54294;
		s[18]=0.58569;
		s[19]=0.53146;
		s[20]=0.48492;
		s[21]=0.43875;
		s[22]=0.54388;
		s[23]=0.63431;
		s[24]=0.47104;
		s[25]=0.58832;
		s[26]=0.56326;
		s2[27]=0.47104;
		s2[28]=0.63431;
		s2[29]=0.54388;
		s2[30]=0.56326;
		s2[30]=0.53146;

		ViNoise n(s.size());
		n.set(8,1);
		n.set(9,1);
		n.set(10,1);
		n.set(11,1);
		n.set(12,1);
		n.set(13,1);
		n.set(14,1);
		n.set(15,1);*/

		ViSampleChunk s(9);
		s[0]=0.1;
		s[1]=0.3;
		s[2]=0.4;
		s[3]=1;
		s[4]=1;
		s[5]=1;
		s[6]=0.3;
		s[7]=0.2;
		s[8]=0;

		ViNoise n(s.size());
		n.set(3,1);
		n.set(4,1);
		n.set(5,1);

		/*ViSampleChunk s(16);
				s[0]=0;
				s[1]=0;
				s[2]=-0.00003;
				s[3]=-0.00003;
				s[4]=1;
				s[5]=1;
				s[6]=1;
				s[7]=1;
				s[8]=1;
				s[9]=1;
				s[10]=1;
				s[11]=1;
				s[12]=0;
				s[13]=0;
				s[14]=-0.00003;
				s[15]=0;


		ViNoise n(s.size());
		n.set(4,1);
		n.set(5,1);
		n.set(6,1);
		n.set(7,1);
		n.set(8,1);
		n.set(9,1);
		n.set(10,1);
		n.set(11,1);*/

		/*ViSampleChunk s(4);
		s[0]=0;
		s[1]=1;
		s[2]=0.4;
		s[3]=0.4;

		ViNoise n(s.size());
		n.set(1,1);*/












		/*ViSampleChunk s(10);
		s[0] = 0.00180;
		s[1] = -0.04834;
		s[2] = 0.16000;
		s[3] = 0.08258;
		s[4] = 1; // s[16] = -0.60022;
		s[5] = 1; // s[17] = -0.77640;
		s[6] = 0.07126;
		s[7] = -0.13742;
		s[8] = -0.11526;
		s[9] = 0.17563;

		ViNoise n(s.size());
		n.set(4,1);
		n.set(5,1);
*/














		ViHermiteInterpolator al;
		al.setDegree(2);
		al.interpolate(s, n);
		for(int i = 0; i<s.size();++i)
		{
			/*if(n[i])cout<<"*\t";
			else cout<<" \t";
			cout<<s2[i]<<"\t"<<s[i]<<endl;*/
			cout<<i<<"\t"<<s[i]<<endl;
		}

		int x = 0;
		int y = 1/x;
	}

	ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
	//window->show();
	
	application.exec();

	return 0;
}

