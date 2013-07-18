#include <viwavewidget.h>
#include <vibutton.h>
#include <viwavewidgetgroup.h>

#define TOOLBAR_SHOW_MILLISECONDS 2000

ViWaveWidget::ViWaveWidget(QWidget *parent)
	: ViWidget(parent)
{
	setMinimumHeight(150);

	mGroup = NULL;

	mBaseWidget = new ViWaveBaseWidget(this);
	mOverlayWidget = new ViWaveOverlayWidget(this);

	QObject::connect(mOverlayWidget, SIGNAL(pointerChanged(qint32)), this, SIGNAL(pointerChanged(qint32)));
	QObject::connect(mOverlayWidget, SIGNAL(pointerChanged(qint32)), this, SLOT(setPointer(qint32)));
	QObject::connect(mOverlayWidget, SIGNAL(zoomLevelChanged(qint16)), this, SLOT(zoom(qint16)));
	QObject::connect(mOverlayWidget, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(setPosition(ViAudioPosition)), Qt::DirectConnection);

	mControlToolbar = new ViWaveControlToolbar(this);
	QObject::connect(mControlToolbar, SIGNAL(zoomedIn()), this, SLOT(zoomIn()));
	QObject::connect(mControlToolbar, SIGNAL(zoomedOut()), this, SLOT(zoomOut()));
	QObject::connect(mControlToolbar, SIGNAL(toggledGroup()), this, SLOT(toggleGroup()));

	mInfoToolbar = new ViWaveInfoToolbar(this);
	QObject::connect(mOverlayWidget, SIGNAL(pointerValuesChanged(qreal, qreal, qreal, qreal)), mInfoToolbar, SLOT(setValues(qreal, qreal, qreal, qreal)));

	mToolbarTimer.setInterval(TOOLBAR_SHOW_MILLISECONDS);
	QObject::connect(&mToolbarTimer, SIGNAL(timeout()), this, SLOT(hideToolbars()));
	QObject::connect(mOverlayWidget, SIGNAL(mouseMoved()), &mToolbarTimer, SLOT(start()));
	QObject::connect(mOverlayWidget, SIGNAL(mouseMoved()), this, SLOT(showToolbars()));

	clear();
}

ViWaveWidget::~ViWaveWidget()
{
	delete mControlToolbar;
	delete mInfoToolbar;
	delete mBaseWidget;
	delete mOverlayWidget;
}

void ViWaveWidget::setZoom(qint16 level)
{
	if(level != mZoomLevel)
	{
		if(level >= 0 && level < MAXIMUM_ZOOM_LEVELS)
		{
			mZoomLevel = level;
		}
		mBaseWidget->setZoomLevel(mZoomLevel);
		mOverlayWidget->setZoomLevel(mZoomLevel);
		emit zoomChanged(mZoomLevel);
	}
}

void ViWaveWidget::setWaveForm(ViWaveForm *form, ViAudioFormat format)
{
	mBaseWidget->setWaveForm(form, format);
	mOverlayWidget->setWaveForm(form, format);
}

void ViWaveWidget::clear()
{
	mBaseWidget->clear();
	mOverlayWidget->clear();

	setZoom(6);
	setPosition(0);
	setPointer(0);
}

void ViWaveWidget::setGroup(ViWaveWidgetGroup *group)
{
	mGroup = group;
}

ViWaveWidgetGroup* ViWaveWidget::group()
{
	return mGroup;
}

bool ViWaveWidget::hasGroup()
{
	return mGroup != NULL;
}

void ViWaveWidget::toggleGroup()
{
	if(mGroup != NULL)
	{
		if(mGroup->hasWidget(this))
		{
			disconnectGroup();
		}
		else
		{
			connectGroup();
		}
	}
}

void ViWaveWidget::connectGroup()
{
	if(mGroup != NULL)
	{
		hideToolbars();
		mGroup->addWidget(this);
		showToolbars();
	}
}

void ViWaveWidget::disconnectGroup()
{
	if(mGroup != NULL)
	{
		hideToolbars();
		mGroup->removeWidget(this);
		showToolbars();
	}
}

void ViWaveWidget::showToolbars()
{
	if(!mControlToolbar->isVisible() || !mInfoToolbar->isVisible())
	{
		mControlToolbar->show();
		mInfoToolbar->show();
		emit toolbarsShown();
	}
}

void ViWaveWidget::hideToolbars()
{
	if(mControlToolbar->isVisible() || mInfoToolbar->isVisible())
	{
		mControlToolbar->hide();
		mInfoToolbar->hide();
		emit toolbarsHidden();
	}
}

void ViWaveWidget::setPointer(qint32 position)
{
	mOverlayWidget->setPointer(position);
}

void ViWaveWidget::setPosition(ViAudioPosition position)
{
	mBaseWidget->setPosition(position);
	emit positionChanged(position);
}

void ViWaveWidget::setPosition(qint64 sample)
{
	mOverlayWidget->setPosition(sample);
}

void ViWaveWidget::zoom(qint16 levels)
{
	setZoom(mZoomLevel + levels);
}

void ViWaveWidget::zoomIn()
{
	zoom(-1);
}

void ViWaveWidget::zoomOut()
{
	zoom(1);
}

void ViWaveWidget::resizeEvent(QResizeEvent *event)
{
	mBaseWidget->resize(event->size());
	mOverlayWidget->resize(event->size());

	mControlToolbar->refresh();
	mInfoToolbar->refresh();
}

void ViWaveWidget::enterEvent(QEvent *event)
{
	showToolbars();
}

void ViWaveWidget::leaveEvent(QEvent *event)
{
	QPoint mouse = QCursor::pos();
	QPoint position = mapToGlobal(pos());
	if(mouse.x() < position.x() || mouse.x() > position.x() + width() || mouse.y() < position.y() || mouse.y() > position.y() + height())
	{
		hideToolbars();
	}
}
