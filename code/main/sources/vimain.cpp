#include "vimainwindow.h"
#include "viaudioengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	QIcon logo("logo.png");
	application.setWindowIcon(logo);

	//Register SIGNAL/SLOT parameter types
	qRegisterMetaType<QSharedPointer<ViWaveFormChunk> >("QSharedPointer<ViWaveFormChunk>");

	ViAudioEngine engine;
	ViMainWindow window(&engine);
	window.setWindowIcon(logo);
	window.show();
	
	application.exec();
	return 0;
}

