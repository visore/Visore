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

#include<vinearestneighbournoisedetector.h>


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
		/*QStringList paths;

		QDir dir1("/home/visore/Desktop/spin2014/classical/");
		QStringList files1 = dir1.entryList(QDir::Files);
		for(int i = 0; i < files1.size(); ++i) paths.append(dir1.absoluteFilePath(files1[i]));

		QDir dir2("/home/visore/Desktop/spin2014/country/");
		QStringList files2 = dir2.entryList(QDir::Files);
		for(int i = 0; i < files2.size(); ++i) paths.append(dir2.absoluteFilePath(files2[i]));

		QDir dir21("/home/visore/Desktop/spin2014/jazz/");
		QStringList files21 = dir21.entryList(QDir::Files);
		for(int i = 0; i < files21.size(); ++i) paths.append(dir21.absoluteFilePath(files21[i]));

		QDir dir3("/home/visore/Desktop/spin2014/metal/");
		QStringList files3 = dir3.entryList(QDir::Files);
		for(int i = 0; i < files3.size(); ++i) paths.append(dir3.absoluteFilePath(files3[i]));

		QDir dir4("/home/visore/Desktop/spin2014/pop/");
		QStringList files4 = dir4.entryList(QDir::Files);
		for(int i = 0; i < files4.size(); ++i) paths.append(dir4.absoluteFilePath(files4[i]));

		QDir dir5("/home/visore/Desktop/spin2014/raggae/");
		QStringList files5 = dir5.entryList(QDir::Files);
		for(int i = 0; i < files5.size(); ++i) paths.append(dir5.absoluteFilePath(files5[i]));

		QDir dir6("/home/visore/Desktop/spin2014/rock/");
		QStringList files6 = dir6.entryList(QDir::Files);
		for(int i = 0; i < files6.size(); ++i) paths.append(dir6.absoluteFilePath(files6[i]));

		QDir dir7("/home/visore/Desktop/spin2014/trance/");
		QStringList files7 = dir7.entryList(QDir::Files);
		for(int i = 0; i < files7.size(); ++i) paths.append(dir7.absoluteFilePath(files7[i]));

		bench.benchmark(paths);*/

		//bench.benchmark("/home/visore/Visore Projects/Noise1.flac");
	}
	else
	{
		ViSampleChunk s2(16);
				s2[0]=	0.22629	;
				s2[1]=	0.242	;
				s2[2]=	0.26132	;
				s2[3]=	0.28867	;
				s2[4]=	1	;
				s2[5]=	0.34045	;
				s2[6]=	0.34818	;
				s2[7]=	1	;
				s2[8]=	1	;
				s2[9]=	0.32773	;
				s2[10]=	0.31982	;
				s2[11]=	0.31943	;
				s2[12]=	1	;
				s2[13]=	0.37149	;
				s2[14]=	0.35687	;
				s2[15]=	0.37149	;


		ViNearestNeighbourNoiseDetector d;
		d.initialize(1, 16);
		d.calculateNoise(s2);
		ViSampleChunk &n = *d.noise(0).data();
		ViSampleChunk &n2 = *d.noise(0).mask();


		for(int i = 0; i < s2.size(); ++i)
		{
			cout << QString::number(s2[i], 'f', 4).toLatin1().data() << "\t"<< QString::number(n[i], 'f', 4).toLatin1().data()<<"\t"<<n2[i]<<endl;
		}

		int x = 0;
		int y = 1/x;
	}

	ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
	window->show();
	
	application.exec();

	return 0;
}

