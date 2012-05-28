#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	QIcon logo("logo.png");
	application.setWindowIcon(logo);

	//Register SIGNAL/SLOT parameter types
	//qRegisterMetaType<QSharedPointer<ViWaveFormChunk> >("QSharedPointer<ViWaveFormChunk>");
	qRegisterMetaType<ViCoder::State>("ViCoder::State");

	ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
	window->show();
	
	application.exec();

	delete window;

	return 0;
}

