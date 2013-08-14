#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<viadvancedlinearinterpolator.h>

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

	ViSampleChunk s(8);
	s[0] = 0.1;
	s[1] = 0.2;
	s[2] = 0.3;
	s[3] = 1;
	s[4] = 1;
	s[5] = 0.4;
	s[6] = 0.3;
	s[7] = 0.2;

	ViLinearInterpolator p;

	ViNoise n1(8);
	n1.set(3, 1);
	n1.set(4, 1);

	p.interpolate(s, n1);

	cout<<"**************"<<endl;
	for(int i = 0; i<8;++i) cout<<s[i]<<endl;




	s[0] = 0.1;
	s[1] = 0.2;
	s[2] = 0.3;
	s[3] = 1;
	s[4] = 1;
	s[5] = 0.4;
	s[6] = 0.3;
	s[7] = 0.2;
	ViAdvancedLinearInterpolator p2;

	p2.interpolate(s, n1);

	cout<<"**************"<<endl;
	for(int i = 0; i<8;++i) cout<<s[i]<<endl;


    ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
    window->show();
	
    application.exec();

	return 0;
}

