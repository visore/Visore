#include <viwaveinfotoolbar.h>

ViWaveInfoToolbar::ViWaveInfoToolbar(QWidget *parent)
	: ViWidgetToolbar(Qt::AlignLeft | Qt::AlignBottom, parent)
{
	mWidget = new QWidget(this);
	mLayout = new QGridLayout(mWidget);
	mMaxLabel = new QLabel(mWidget);
	mMinLabel = new QLabel(mWidget);
	mMaxAvgLabel = new QLabel(mWidget);
	mMinAvgLabel = new QLabel(mWidget);

	mMaxLabel->setAlignment(Qt::AlignRight);
	mMinLabel->setAlignment(Qt::AlignRight);
	mMaxAvgLabel->setAlignment(Qt::AlignRight);
	mMinAvgLabel->setAlignment(Qt::AlignRight);

	mLayout->addWidget(new QLabel(trUtf8("Positive \u2191: "), mWidget), 0, 0); 
	mLayout->addWidget(mMaxLabel, 0, 1); 
	mLayout->addWidget(new QLabel(trUtf8("Negative \u2193: "), mWidget), 1, 0); 
	mLayout->addWidget(mMinLabel, 1, 1); 
	mLayout->addWidget(new QLabel(trUtf8("Positive \u2205: "), mWidget), 2, 0); 
	mLayout->addWidget(mMaxAvgLabel, 2, 1); 
	mLayout->addWidget(new QLabel(trUtf8("Negative \u2205: "), mWidget), 3, 0); 
	mLayout->addWidget(mMinAvgLabel, 3, 1); 

	mWidget->setLayout(mLayout);
	mWidget->setStyleSheet("color: " + ViThemeManager::color(ViThemeColors::TextColor1).name() + "; font-size: 11px;");
	addWidget(mWidget);

	clear();
}

ViWaveInfoToolbar::~ViWaveInfoToolbar()
{
	delete mLayout;
	delete mWidget;
}

void ViWaveInfoToolbar::setValues(qreal maximum, qreal minimum, qreal maximumAverage, qreal minimumAverage)
{
	mMaxLabel->setText(QString::number(maximum, 'f', 4));
	mMinLabel->setText(QString::number(minimum, 'f', 4));
	mMaxAvgLabel->setText(QString::number(maximumAverage, 'f', 4));
	mMinAvgLabel->setText(QString::number(minimumAverage, 'f', 4));
}

void ViWaveInfoToolbar::clear()
{
	setValues(0, 0, 0, 0);
}
