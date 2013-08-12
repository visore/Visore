#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vimodelsolver.h>

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
	s[3] = 0.4;
	s[4] = 0.5;
	s[5] = 0.4;
	s[6] = 0.3;
	s[7] = 0.2;

	ViModelSolver so;
	so.setOrder(7);
	so.estimate(s);

	ViVector v = so.coefficients();
	STATICLOG(v.toString());
	qreal t = v[0];
	for(int i = 1; i < v.size(); ++i)
	{
		t += v[i] * qPow(8, i);
	}
	cout<<t<<endl;


    ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
    window->show();
	
    application.exec();

	return 0;
}

