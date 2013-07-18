#include <viwaveinfotoolbar.h>

ViWaveInfoToolbar::ViWaveInfoToolbar(QWidget *parent)
	: ViWidgetToolbar(Qt::AlignLeft | Qt::AlignCenter, parent)
{
	mValid = false;

	mWidget = new QWidget(this);
	mLayout = new QGridLayout();
	mSampleLabel = new QLabel();
	mSecondLabel = new QLabel();
	mMaximumLabel = new QLabel();
	mMinimumLabel = new QLabel();
	mAverageLabel = new QLabel();

	mSampleLabel->setAlignment(Qt::AlignRight);
	mSecondLabel->setAlignment(Qt::AlignRight);
	mMaximumLabel->setAlignment(Qt::AlignRight);
	mMinimumLabel->setAlignment(Qt::AlignRight);
	mAverageLabel->setAlignment(Qt::AlignRight);

	/*mSampleLabel->setStyleSheet("text-align: center;");
	mSecondLabel->setStyleSheet("text-align: center;");
	mMaximumLabel->setStyleSheet("text-align: center;");
	mMinimumLabel->setStyleSheet("text-align: center;");
	mAverageLabel->setStyleSheet("text-align: center;");*/

	mLayout->addWidget(new QLabel("Sample: ", mWidget), 0, 0);
	mLayout->addWidget(mSampleLabel, 0, 1);
	mLayout->addWidget(new QLabel("Second: ", mWidget), 1, 0);
	mLayout->addWidget(mSecondLabel, 1, 1);
	mLayout->addWidget(new QLabel("Maximum: ", mWidget), 2, 0);
	mLayout->addWidget(mMaximumLabel, 2, 1);
	mLayout->addWidget(new QLabel("Minimum: ", mWidget), 3, 0);
	mLayout->addWidget(mMinimumLabel, 3, 1);
	mLayout->addWidget(new QLabel("Average: ", mWidget), 4, 0);
	mLayout->addWidget(mAverageLabel, 4, 1);

	mLayout->setSpacing(3);
	mLayout->setContentsMargins(0, 0, 0, 0);
	mWidget->setLayout(mLayout);
	mWidget->setMinimumWidth(110);
	mWidget->setStyleSheet("QLabel { min-height: 0px; height: 11px; font-size: 11px; color: " + ViThemeManager::color(ViThemeColors::TextColor1).name() + "; }");
	addWidget(mWidget);

	clear();
}

ViWaveInfoToolbar::~ViWaveInfoToolbar()
{
	delete mLayout;
	delete mWidget;
}

void ViWaveInfoToolbar::setValues(qreal maximum, qreal minimum, qreal average)
{
	if(mValid)
	{
		mMaximumLabel->setText(QString::number(maximum, 'f', 5));
		mMinimumLabel->setText(QString::number(minimum, 'f', 5));
		mAverageLabel->setText(QString::number(average, 'f', 5));
	}
}

void ViWaveInfoToolbar::setPosition(ViAudioPosition position)
{
	if(position.isValid())
	{
		mValid = true;
		mSampleLabel->setText(QString::number(position.samples() + 1, 'f', 0));
		qreal seconds = position.seconds();
		int decimalPlaces = 5;
		if(seconds >= 100) decimalPlaces = 4;
		mSecondLabel->setText(QString::number(seconds, 'f', decimalPlaces));
		refresh();
	}
	else
	{
		mValid = false;
		mMaximumLabel->setText("-");
		mMinimumLabel->setText("-");
		mAverageLabel->setText("-");
		mSampleLabel->setText("-");
		mSecondLabel->setText("-");
	}
}

void ViWaveInfoToolbar::clear()
{
	mValid = false;
	setValues(0, 0, 0);
	setPosition(ViAudioPosition());
}
