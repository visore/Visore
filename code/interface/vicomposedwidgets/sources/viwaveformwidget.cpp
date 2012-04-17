#include "viwaveformwidget.h"

#define CONTENT_MARGIN -4

ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent)
	: ViWidget(parent)
{
	mBaseWidget = new ViWaveBaseWidget(engine, type, this);

	mOverlayWidget = new ViWaveOverlayWidget(engine, type, this);
	ViObject::connect(mOverlayWidget, SIGNAL(pointerMoved(qint32)), this, SIGNAL(pointerMoved(qint32)));
	ViObject::connect(mOverlayWidget, SIGNAL(pointerMoved(qint32)), this, SLOT(setPointer(qint32)));

	parent->setObjectName("parent");
	parent->setStyleSheet("\
		.QWidget#parent{\
			border-radius: 10px;\
			border-style: solid;\
			border-color: rgb(" + QString::number(ViThemeManager::color(14).red()) + ", " + QString::number(ViThemeManager::color(14).green()) + ", " + QString::number(ViThemeManager::color(14).blue()) + ");\
			border-width: 5px;\
		}\
	");
	parent->setContentsMargins(CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN);

	mControlToolbar = new ViWidgetToolbar(Qt::AlignCenter | Qt::AlignRight, this);
	mControlToolbar->setEngine(engine);

	ViObject::connect(&mZoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
	mZoomInButton.setIcon(ViThemeManager::image("zoomin.png", ViThemeImage::Normal, ViThemeManager::Icon).icon());
	mZoomInButton.setText("Zoom In");
	mZoomInButton.setToolButtonStyle(Qt::ToolButtonIconOnly);
	mControlToolbar->addWidget(&mZoomInButton);

	ViObject::connect(&mZoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
	mZoomOutButton.setIcon(ViThemeManager::image("zoomout.png", ViThemeImage::Normal, ViThemeManager::Icon).icon());
	mZoomOutButton.setText("Zoom Out");
	mZoomOutButton.setToolButtonStyle(Qt::ToolButtonIconOnly);
	mControlToolbar->addWidget(&mZoomOutButton);

	mInfoToolbar = new ViWidgetToolbar(Qt::AlignLeft | Qt::AlignBottom, this);
	mInfoToolbar->setEngine(engine);

	mMinLabel1.setTextFormat(Qt::RichText);
	mMaxLabel1.insertHtml("Positive &#x2205; :");
	mMinLabel1.setText(trUtf8("Σ"));

	mMaxAvgLabel1.setText("Positive \u2191:");
	mMinAvgLabel1.setText("");
	mMaxLabel2.setText("0");
	mMinLabel2.setText("0");
	mMaxAvgLabel2.setText("0");
	mMinAvgLabel2.setText("0");

	mInfoLayout.addWidget(&mMaxLabel1, 0, 0); 
	mInfoLayout.addWidget(&mMaxLabel2, 0, 1); 
	mInfoLayout.addWidget(&mMinLabel1, 1, 0); 
	mInfoLayout.addWidget(&mMinLabel2, 1, 1); 
	mInfoLayout.addWidget(&mMaxAvgLabel1, 0, 2); 
	mInfoLayout.addWidget(&mMaxAvgLabel2, 0, 3); 
	mInfoLayout.addWidget(&mMinAvgLabel1, 1, 2); 
	mInfoLayout.addWidget(&mMinAvgLabel2, 1, 3); 

	mInfoWidget.setLayout(&mInfoLayout);
	mInfoToolbar->addWidget(&mInfoWidget);

	setZoomLevel(6);

}

ViWaveFormWidget::~ViWaveFormWidget()
{
	delete mControlToolbar;
	delete mBaseWidget;
	delete mOverlayWidget;
}

void ViWaveFormWidget::setZoomLevel(qint16 level)
{
	if(level >= 0 && level < MAXIMUM_ZOOM_LEVELS)
	{
		mZoomLevel = level;
	}
	mBaseWidget->setZoomLevel(mZoomLevel);
	mOverlayWidget->setZoomLevel(mZoomLevel);
}

void ViWaveFormWidget::setPointer(qint32 position)
{
	mOverlayWidget->setPointer(position);
}

void ViWaveFormWidget::zoomIn()
{
	setZoomLevel(mZoomLevel - 1);
}

void ViWaveFormWidget::zoomOut()
{
	setZoomLevel(mZoomLevel + 1);
}

void ViWaveFormWidget::resizeEvent(QResizeEvent *event)
{
	mBaseWidget->resize(event->size());
	mOverlayWidget->resize(event->size());
	mControlToolbar->refresh();
}

void ViWaveFormWidget::enterEvent(QEvent *event)
{
	mControlToolbar->show();
	mInfoToolbar->show();
}

void ViWaveFormWidget::leaveEvent(QEvent *event)
{
	QPoint mouse = QCursor::pos();
	QPoint position = mapToGlobal(pos());
	if(mouse.x() < position.x() || mouse.x() > position.x() + width() || mouse.y() < position.y() || mouse.y() > position.y() + height())
	{
		mControlToolbar->hide();
		mInfoToolbar->hide();
	}
}
