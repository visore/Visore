#include <vimainplaybackwidget.h>
#include <ui_vimainplaybackwidget.h>

ViMainPlaybackWidget::ViMainPlaybackWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainPlaybackWidget();
	mUi->setupUi(this);

	QObject::connect(mUi->projectLoader, SIGNAL(trackChanged()), this, SLOT(loadTrack()));
	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(loadTrack()));
	clear();
}

ViMainPlaybackWidget::~ViMainPlaybackWidget()
{
	delete mUi;
	clear();
}

void ViMainPlaybackWidget::loadTrack()
{

}

void ViMainPlaybackWidget::clear()
{
	mUi->projectLoader->clear();
}
