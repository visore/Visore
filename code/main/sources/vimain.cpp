#include "vimainwindow.h"
#include "viaudioengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	ViAudioEngine engine;
	ViMainWindow window(&engine);
	window.show();

	return application.exec();
}

