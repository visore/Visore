#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vibenchmarker.h>
#include <vilinearinterpolator.h>
#include <viadvancedlinearinterpolator.h>

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

	ViBenchMarker bench;
	bench.benchmark("/home/visore/Visore Projects/a.flac");

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

	ViSampleChunk s(9);
	s[0] = -0.3;
	s[1] = -0.2;
	s[2] = -0.1;
	s[3] = 1;
	s[4] = 1;
	s[5] = 1;
	s[6] = -0.7;
	s[7] = -0.8;
	s[8] = -0.9;

	ViNoise n(9);
	n.set(3,1);
	n.set(4,1);
	n.set(5,1);

	ViAdvancedLinearInterpolator al;
	al.interpolate(s, n);
	for(int i = 0; i<s.size();++i)cout<<s[i]<<" ";
	cout<<endl;

	s[0] = -0.1;
	s[1] = -0.2;
	s[2] = -0.3;
	s[3] = 1;
	s[4] = 1;
	s[5] = 1;
	s[6] = -0.3;
	s[7] = -0.2;
	s[8] = -0.1;



	ViLinearInterpolator l;
	l.interpolate(s, n);
	for(int i = 0; i<s.size();++i)cout<<s[i]<<" ";
	cout<<endl;

	int x = 0;
	//int y = 1/x;


	ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
	//window->show();
	
	application.exec();

	return 0;
}

