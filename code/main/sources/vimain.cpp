//#include "viessentials.h"
#include "viaudioengine.h"
#include <QApplication>
//#include <QMainWindow>

void run();

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
ViAudioEngine *e = new ViAudioEngine();
	//ViLibrary<ViAudioEngine> interface("/home/visore/Visore/install/system/libviaudioengine.so");
	//ViLibrary<QMainWindow> interface("/home/visore/Visore/install/i//nterface/libvimainwindow.so");
	//if(!interface.open()) ViLogger::debug("Library cannot be loaded!");
	//ViAudioEngine *engine = interface.create();
	//QMainWindow *window = interface.create();
	//window->show();
	//delete window;
//ViAudioEngine *e = new ViAudioEngine();

	return application.exec();
}

