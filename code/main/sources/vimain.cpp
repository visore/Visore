#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

#include<vibenchmarker.h>
#include<vibenchmarker3.h>
#include<vibenchmarker4.h>
#include <vipredictorbenchmarker.h>
#include <viinterpolatorbenchmarker.h>
#include <vinoisedetectorbenchmarker.h>

#include <viinterpolatorbatcher.h>
#include <vinoisebatcher.h>

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

	QString arg = "";
	if(argc > 1) arg = QString(argv[1]).replace("-", "");



    bool BENCH = true;
    //bool BENCH = false;

	//ViBenchmarker bench;
	ViPredictorBenchmarker bench;
	//ViInterpolatorBenchmarker bench;
	//ViNoiseDetectorBenchmarker bench;

//	ViInterpolatorBatcher bench;
	//ViNoiseBatcher bench;

	if(BENCH)
	{
		bench.benchmark(arg);
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

