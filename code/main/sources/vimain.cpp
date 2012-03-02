#include "vimainwindow.h"
#include "viaudioengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	//Register SIGNAL/SLOT parameter types
	qRegisterMetaType<QSharedPointer<ViWaveFormChunk> >("QSharedPointer<ViWaveFormChunk>");

	ViAudioEngine engine;
	ViMainWindow window(&engine);
	window.show();

	return application.exec();
}

