#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>

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

    ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
    window->show();
	
    application.exec();

	return 0;
}

