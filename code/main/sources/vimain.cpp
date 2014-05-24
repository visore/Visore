#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vibenchmarker2.h>
#include<vibenchmarker3.h>
#include<vibenchmarker4.h>
#include <vipredictorbenchmarker.h>

#include<vifourierpolynomial.h>

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
		/*int dir = 8;
		int deg = 3;
		ViFourierPolynomial p;
		p.setDegree(deg);
		p.setDerivatives(dir);


		for(int i = 0; i <= dir;++i)
		{
			cout<<"************************: "<<i<<endl;
			for(int j = 1; j <= 1;++j)
			{
				cout<<p.get(deg, i)[j].toString().toLatin1().data()<<endl;
			}
		}*/





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

