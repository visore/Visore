#include "vimainwindow.h"
#include "ui_vimainwindow.h"

ViMainWindow::ViMainWindow(ViAudioEngine *engine, QWidget *parent)
	: QMainWindow(parent)
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mUi->centralWidget->initialize(engine);	
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
}
