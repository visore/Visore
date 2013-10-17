#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vibenchmarker.h>
#include <vipiecewiseconstantinterpolator.h>
#include <visplineinterpolator.h>
#include <vipolynomialinterpolator.h>
#include <vihermiteinterpolator.h>
#include <vifourierinterpolator.h>
#include <vicosineinterpolator.h>
#include<vicrosscorrelator.h>


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
		QStringList paths;

		QDir dir1("/home/visore/Desktop/spin2014_2/classical/");
		QStringList files1 = dir1.entryList(QDir::Files);
		for(int i = 0; i < files1.size(); ++i) paths.append(dir1.absoluteFilePath(files1[i]));

		QDir dir2("/home/visore/Desktop/spin2014_2/country/");
		QStringList files2 = dir2.entryList(QDir::Files);
		for(int i = 0; i < files2.size(); ++i) paths.append(dir2.absoluteFilePath(files2[i]));

		QDir dir21("/home/visore/Desktop/spin2014_2/jazz/");
		QStringList files21 = dir21.entryList(QDir::Files);
		for(int i = 0; i < files21.size(); ++i) paths.append(dir21.absoluteFilePath(files21[i]));

		QDir dir3("/home/visore/Desktop/spin2014_2/metal/");
		QStringList files3 = dir3.entryList(QDir::Files);
		for(int i = 0; i < files3.size(); ++i) paths.append(dir3.absoluteFilePath(files3[i]));

		QDir dir4("/home/visore/Desktop/spin2014_2/pop/");
		QStringList files4 = dir4.entryList(QDir::Files);
		for(int i = 0; i < files4.size(); ++i) paths.append(dir4.absoluteFilePath(files4[i]));

		QDir dir5("/home/visore/Desktop/spin2014_2/raggae/");
		QStringList files5 = dir5.entryList(QDir::Files);
		for(int i = 0; i < files5.size(); ++i) paths.append(dir5.absoluteFilePath(files5[i]));

		QDir dir6("/home/visore/Desktop/spin2014_2/rock/");
		QStringList files6 = dir6.entryList(QDir::Files);
		for(int i = 0; i < files6.size(); ++i) paths.append(dir6.absoluteFilePath(files6[i]));

		QDir dir7("/home/visore/Desktop/spin2014_2/trance/");
		QStringList files7 = dir7.entryList(QDir::Files);
		for(int i = 0; i < files7.size(); ++i) paths.append(dir7.absoluteFilePath(files7[i]));

		bench.benchmark(paths);

		//bench.benchmark("/home/visore/Visore Projects/a2.flac");
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

*/
		/*ViSampleChunk s(32);
		s[0]=0.45474;
		s[1]=0.46835;
		s[2]=0.45020;
		s[3]=0.38251;
		s[4]=0.39502;
		s[5]=0.63791;
		s[6]=0.67032;
		s[7]=0.76260;
		s[8]=1;
		s[9]=1;
		s[10]=1;
		s[11]=1;
		s[12]=1;
		s[13]=1;
		s[14]=1;
		s[15]=1;
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
		s[27]=0.47104;
		s[28]=0.63431;
		s[29]=0.54388;
		s[30]=0.56326;
		s[31]=0.53146;

		ViSampleChunk s2 = s;

		ViNoise n(s.size());
		n.set(8,1);
		n.set(9,1);
		n.set(10,1);
		n.set(11,1);
		n.set(12,1);
		n.set(13,1);
		n.set(14,1);
		n.set(15,1);*/

		ViSampleChunk s(64);
		s[0]=	0.22629	;
		s[1]=	0.242	;
		s[2]=	0.26132	;
		s[3]=	0.28867	;
		s[4]=	0.31772	;
		s[5]=	0.34045	;
		s[6]=	0.34818	;
		s[7]=	0.34348	;
		s[8]=	0.33554	;
		s[9]=	0.32773	;
		s[10]=	0.31982	;
		s[11]=	0.31943	;
		s[12]=	0.32886	;
		s[13]=	0.341	;
		s[14]=	0.35687	;
		s[15]=	0.37149	;
		s[16]=	0	;
		s[17]=	-0.5	;
		s[18]=	-0.95	;
		s[19]=	1	;
		s[20]=	0.95	;
		s[21]=	0.98	;
		s[22]=	0.99	;
		s[23]=	1	;
		s[24]=	0.98	;
		s[25]=	0.99	;
		s[26]=	1	;
		s[27]=	0.98	;
		s[28]=	0.99	;
		s[29]=	1	;
		s[30]=	0.99	;
		s[31]=	0.98	;
		s[32]=	1	;
		s[33]=	0.99	;
		s[34]=	1	;
		s[35]=	1	;
		s[36]=	1	;
		s[37]=	0.99	;
		s[38]=	1	;
		s[39]=	0.5	;
		s[40]=	0	;
		s[41]=	-1	;
		s[42]=	0.68948	;
		s[43]=	0.66052	;
		s[44]=	0.63129	;
		s[45]=	0.61633	;
		s[46]=	0.62006	;
		s[47]=	0.63742	;
		s[48]=	0.65607	;
		s[49]=	0.66574	;
		s[50]=	0.664	;
		s[51]=	0.65933	;
		s[52]=	0.65866	;
		s[53]=	0.65784	;
		s[54]=	0.65317	;
		s[55]=	0.64435	;
		s[56]=	0.6268	;
		s[57]=	0.60355	;
		s[58]=	0.65933	;
		s[59]=	0.65866	;
		s[60]=	0.65784	;
		s[61]=	0.65317	;
		s[62]=	0.64435	;
		s[63]=	0.6268	;

		ViSampleChunk s2(64);
				s2[0]=	0.22629	;
				s2[1]=	0.242	;
				s2[2]=	0.26132	;
				s2[3]=	0.28867	;
				s2[4]=	0.31772	;
				s2[5]=	0.34045	;
				s2[6]=	0.34818	;
				s2[7]=	0.34348	;
				s2[8]=	0.33554	;
				s2[9]=	0.32773	;
				s2[10]=	0.31982	;
				s2[11]=	0.31943	;
				s2[12]=	0.32886	;
				s2[13]=	0.341	;
				s2[14]=	0.35687	;
				s2[15]=	0.37149	;
				s2[16]=	0.38007	;
				s2[17]=	0.3894	;
				s2[18]=	0.40787;
				s2[19]=	0.42075	;
				s2[20]=	0.4223	;
				s2[21]=	0.42343	;
				s2[22]=	0.42261	;
				s2[23]=	0.42282	;
				s2[24]=	0.4346	;
				s2[25]=	0.45703	;
				s2[26]=	0.48315	;
				s2[27]=	0.50418	;
				s2[28]=	0.5123	;
				s2[29]=	0.51309	;
				s2[30]=	0.52084;
				s2[31]=	0.54398	;
				s2[32]=	0.58112	;
				s2[33]=	0.62308	;
				s2[34]=	0.66165	;
				s2[35]=	0.68634	;
				s2[36]=	0.69403	;
				s2[37]=	0.6969	;
				s2[38]=	0.70367	;
				s2[39]=	0.71017	;
				s2[40]=	0.70956;
				s2[41]=	0.70456	;
				s2[42]=	0.68948	;
				s2[43]=	0.66052	;
				s2[44]=	0.63129	;
				s2[45]=	0.61633	;
				s2[46]=	0.62006	;
				s2[47]=	0.63742	;
				s2[48]=	0.65607	;
				s2[49]=	0.66574	;
				s2[50]=	0.664	;
				s2[51]=	0.65933	;
				s2[52]=	0.65866	;
				s2[53]=	0.65784	;
				s2[54]=	0.65317	;
				s2[55]=	0.64435	;
				s2[56]=	0.6268	;
				s2[57]=	0.60355	;
				s2[58]=	0.65933	;
				s2[59]=	0.65866	;
				s2[60]=	0.65784	;
				s2[61]=	0.65317	;
				s2[62]=	0.64435	;
				s2[63]=	0.6268	;



		ViNoise n(s.size());
		for(int i = 16; i < 42; ++i)
		{
			n.set(i,1);
		}


		//ViPiecewiseConstantInterpolator al;
		//ViCosineInterpolator al;
		//ViPolynomialInterpolator al(6);
		ViSplineInterpolator al(5);
		//ViFourierInterpolator al(1);
		//ViHermiteInterpolator al(1);
		al.interpolate(s, n);
		for(int i = 0; i<s.size();++i)
		{
			//cout<<s2[i]<<"\t"<<s[i]<<endl;
			cout<<s[i]<<endl;
		}

		ViCrossCorrelator c;
		cout<<"++: "<<c.execute(s,s2)<<endl;

		/*qreal t = 0;
		qreal t2 = 0;
		for(int i = 0; i<s2.size();++i)
		{
			t += qPow(s2[i]-s[i],2);
			t2 += qPow(s2[i],2);
		}
		cout<<"***: "<<t/t2<<endl;*/

		/*qreal avg = 0;
		for(int i = 0; i<s2.size();++i)
		{
			avg += s2[i];
		}
		avg /= s2.size();

		qreal a1 = 0;
		for(int i = 0; i<s2.size();++i)
		{
			a1 += qPow(s2[i]-avg,2);
		}

		qreal a2 = 0;
		for(int i = 0; i<s2.size();++i)
		{
			a2 += qPow(s[i]-s2[i],2);
		}

		cout<<a1<<" "<<a2<<endl;

		cout<<"SNR: "<<(10*log10(a1/a2))<<endl;*/


		int x = 0;
		int y = 1/x;
	}

	ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
	//window->show();
	
	application.exec();

	return 0;
}

