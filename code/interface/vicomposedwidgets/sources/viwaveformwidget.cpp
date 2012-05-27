#include "viwaveformwidget.h"

ViWaveFormWidget::ViWaveFormWidget(QWidget *parent)
	: ViWidget(parent)
{
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	delete mControlToolbar;
	delete mBaseWidget;
	delete mOverlayWidget;
}

void ViWaveFormWidget::setBufferType(ViAudioBuffer::ViAudioBufferType type)
{
	mBaseWidget = new ViWaveBaseWidget(this);
	mBaseWidget->setBufferType(type);
	mOverlayWidget = new ViWaveOverlayWidget(this);
	mOverlayWidget->setBufferType(type);
	ViObject::connect(mOverlayWidget, SIGNAL(pointerMoved(qint32)), this, SIGNAL(pointerMoved(qint32)));
	ViObject::connect(mOverlayWidget, SIGNAL(pointerMoved(qint32)), this, SLOT(setPointer(qint32)));
	ViObject::connect(mOverlayWidget, SIGNAL(pointerValuesChanged(qreal, qreal, qreal, qreal)), this, SLOT(updateSampleValues(qreal, qreal, qreal, qreal)));
	ViObject::connect(mOverlayWidget, SIGNAL(zoomLevelChanged(qint16)), this, SLOT(zoom(qint16)));

	mControlToolbar = new ViWidgetToolbar(Qt::AlignCenter | Qt::AlignRight, this);

	mZoomInButton = new QToolButton(mControlToolbar);
	ViObject::connect(mZoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
	mZoomInButton->setIcon(ViThemeManager::image("zoomin.png", ViThemeImage::Normal, ViThemeManager::Icon).icon());
	mZoomInButton->setText("Zoom In");
	mZoomInButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	mControlToolbar->addWidget(mZoomInButton);

	mZoomOutButton = new QToolButton(mControlToolbar);
	ViObject::connect(mZoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
	mZoomOutButton->setIcon(ViThemeManager::image("zoomout.png", ViThemeImage::Normal, ViThemeManager::Icon).icon());
	mZoomOutButton->setText("Zoom Out");
	mZoomOutButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	mControlToolbar->addWidget(mZoomOutButton);

	mInfoToolbar = new ViWidgetToolbar(Qt::AlignLeft | Qt::AlignBottom, this);
	mInfoWidget = new QWidget(mInfoToolbar);
	mInfoLayout = new QGridLayout(mInfoWidget);
	mMaxLabel1 = new QLabel(mInfoWidget);
	mMinLabel1 = new QLabel(mInfoWidget);
	mMaxAvgLabel1 = new QLabel(mInfoWidget);
	mMinAvgLabel1 = new QLabel(mInfoWidget);
	mMaxLabel2 = new QLabel(mInfoWidget);
	mMinLabel2 = new QLabel(mInfoWidget);
	mMaxAvgLabel2 = new QLabel(mInfoWidget);
	mMinAvgLabel2 = new QLabel(mInfoWidget);

	mMaxLabel1->setText(trUtf8("Positive \u2191: "));
	mMinLabel1->setText(trUtf8("Negative \u2193: "));
	mMaxAvgLabel1->setText(trUtf8("Positive \u2205: "));
	mMinAvgLabel1->setText(trUtf8("Negative \u2205: "));
	mMaxLabel2->setText("0.0000");
	mMinLabel2->setText("0.0000");
	mMaxAvgLabel2->setText("0.0000");
	mMinAvgLabel2->setText("0.0000");

	mMaxLabel2->setAlignment(Qt::AlignRight);
	mMinLabel2->setAlignment(Qt::AlignRight);
	mMaxAvgLabel2->setAlignment(Qt::AlignRight);
	mMinAvgLabel2->setAlignment(Qt::AlignRight);

	mInfoLayout->addWidget(mMaxLabel1, 0, 0); 
	mInfoLayout->addWidget(mMaxLabel2, 0, 1); 
	mInfoLayout->addWidget(mMinLabel1, 1, 0); 
	mInfoLayout->addWidget(mMinLabel2, 1, 1); 
	mInfoLayout->addWidget(mMaxAvgLabel1, 2, 0); 
	mInfoLayout->addWidget(mMaxAvgLabel2, 2, 1); 
	mInfoLayout->addWidget(mMinAvgLabel1, 3, 0); 
	mInfoLayout->addWidget(mMinAvgLabel2, 3, 1); 

	mInfoWidget->setLayout(mInfoLayout);
	mInfoWidget->setStyleSheet("color: " + ViThemeManager::color(4).name() + "; font-size: 11px;");
	mInfoToolbar->addWidget(mInfoWidget);

	setZoomLevel(6);
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

void ViWaveFormWidget::zoom(qint16 levels)
{
	setZoomLevel(mZoomLevel + levels);
}

void ViWaveFormWidget::zoomIn()
{
	zoom(-1);
}

void ViWaveFormWidget::zoomOut()
{
	zoom(1);
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption options;
	options.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &options, &painter, this);
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

void ViWaveFormWidget::updateSampleValues(qreal maximum, qreal minimum, qreal maximumAverage, qreal minimumAverage)
{
	mMaxLabel2->setText(QString::number(maximum, 'f', 4));
	mMinLabel2->setText(QString::number(minimum, 'f', 4));
	mMaxAvgLabel2->setText(QString::number(maximumAverage, 'f', 4));
	mMinAvgLabel2->setText(QString::number(minimumAverage, 'f', 4));
}
