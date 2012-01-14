#include "vilibrary.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    //QApplication application(argc, argv);

	ViLibrary<QMainWindow> interface("/home/visore/Visore/install/interface/libvimainwindow.so");
	interface.open();
	QMainWindow *window = interface.create();
	//window->show();
	//delete window;

	return /*application.exec()*/0;
}
