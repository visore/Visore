#include "vimainwindow.h"
#include "ui_vimainwindow.h"

ViMainWindow::ViMainWindow(ViAudioEngine *engine, QWidget *parent)
	: QMainWindow(parent)
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mContentWidget = new ViContentWidget();
	mContentWidget->initialize(engine);	
	mUi->centralWidget->setContent(mContentWidget);
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
	delete mContentWidget;
}
