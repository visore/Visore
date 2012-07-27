#include "viformatwidget.h"
#include "ui_viformatwidget.h"
#include "viaudiomanager.h"
#include "viaudiocodec.h"

ViFormatWidget::ViFormatWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViFormatWidget();
	mUi->setupUi(this);
	QObject::connect(mUi->codecBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCodec()));
	QObject::connect(mUi->bitrateModeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBitrate()));
	
	ViCodecList codecs = ViAudioManager::codecs();
	for(int i = 0; i < codecs.size(); ++i)
	{
		mUi->codecBox->addItem(codecs[i]->abbreviations()[0], codecs[i]->abbreviations()[0]);
	}
	changeCodec();
}

ViFormatWidget::~ViFormatWidget()
{
	delete mUi;
}

ViAudioFormat ViFormatWidget::format()
{
	ViAudioFormat result;
	result.setCodec(ViAudioManager::codec(codec()));
	result.setByteOrder(ViAudioFormat::LittleEndian);
	result.setSampleType(ViAudioFormat::SignedInt);
	result.setSampleRate(sampleRate());
	result.setSampleSize(sampleSize());
	result.setChannelCount(channels());
	result.setQuality(ViAudioFormat::Quality(quality()));
	result.setBitrate(ViAudioBitrate(ViAudioBitrate::Mode(bitrateMode()), bitrate(), minimumBitrate(), maximumBitrate()));
	return result;
}

void ViFormatWidget::changeCodec()
{
	ViAudioCodec *currentCodec = ViAudioManager::codec(codec());
	if(currentCodec != NULL)
	{
		mUi->sampleSizeBox->clear();
		mUi->sampleRateBox->clear();
		mUi->channelsBox->clear();
		mUi->qualityBox->clear();
		mUi->bitrateModeBox->clear();

		QList<QString> names;
		QList<int> values;
		ViFormatMap formats;

		formats = currentCodec->supportedSampleSizes();
		if(formats.isEmpty())
		{
			setSampleSizeVisible(false);
		}
		else
		{
			setSampleSizeVisible(true);
			values = formats.keys();
			names = formats.values();
			for(int i = 0; i < formats.size(); ++i)
			{
				mUi->sampleSizeBox->addItem(names[i], values[i]);
			}
			mUi->sampleSizeBox->setCurrentIndex(mUi->sampleSizeBox->findData(formats.defaultValue()));
		}

		formats = currentCodec->supportedSampleRates();
		if(formats.isEmpty())
		{
			setSampleRateVisible(false);
		}
		else
		{
			setSampleRateVisible(true);
			values = formats.keys();
			names = formats.values();
			for(int i = 0; i < formats.size(); ++i)
			{
				mUi->sampleRateBox->addItem(names[i], values[i]);
			}
			mUi->sampleRateBox->setCurrentIndex(mUi->sampleRateBox->findData(formats.defaultValue()));
		}

		formats = currentCodec->supportedChannels();
		if(formats.isEmpty())
		{
			setChannelsVisible(false);
		}
		else
		{
			setChannelsVisible(true);
			values = formats.keys();
			names = formats.values();
			for(int i = 0; i < formats.size(); ++i)
			{
				mUi->channelsBox->addItem(names[i], values[i]);
			}
			mUi->channelsBox->setCurrentIndex(mUi->channelsBox->findData(formats.defaultValue()));
		}

		formats = currentCodec->supportedQualities();
		if(formats.isEmpty())
		{
			setQualityVisible(false);
		}
		else
		{
			setQualityVisible(true);
			values = formats.keys();
			names = formats.values();
			for(int i = 0; i < formats.size(); ++i)
			{
				mUi->qualityBox->addItem(names[i], values[i]);
			}
			mUi->qualityBox->setCurrentIndex(mUi->qualityBox->findData(formats.defaultValue()));
		}

		formats = currentCodec->supportedBitrateModes();
		if(formats.isEmpty())
		{
			setBitrateModeVisible(false);
		}
		else
		{
			setBitrateModeVisible(true);
			values = formats.keys();
			names = formats.values();
			for(int i = 0; i < formats.size(); ++i)
			{
				mUi->bitrateModeBox->addItem(names[i], values[i]);
			}
			mUi->bitrateModeBox->setCurrentIndex(mUi->bitrateModeBox->findData(formats.defaultValue()));
		}
	}

	changeBitrate();
}

void ViFormatWidget::changeBitrate()
{
	ViAudioCodec *currentCodec = ViAudioManager::codec(codec());

	mUi->bitrateBox->clear();
	mUi->minimumBitrateBox->clear();
	mUi->maximumBitrateBox->clear();

	QList<QString> names;
	QList<int> values;
	ViFormatMap formats;

	if(bitrateMode() == ViAudioBitrate::Variable)
	{
		formats = currentCodec->supportedBitrates();
		setBitrateVisible(false);
		setMinimumBitrateVisible(true);
		setMaximumBitrateVisible(true);
		values = formats.keys();
		names = formats.values();
		for(int i = 0; i < formats.size(); ++i)
		{
			mUi->minimumBitrateBox->addItem(names[i], values[i]);
			mUi->maximumBitrateBox->addItem(names[i], values[i]);
		}
		mUi->minimumBitrateBox->setCurrentIndex(mUi->minimumBitrateBox->findData(formats.defaultValue()));
		mUi->maximumBitrateBox->setCurrentIndex(mUi->maximumBitrateBox->findData(formats.defaultValue()));
	}
	else if(bitrateMode() == ViAudioBitrate::Constant || bitrateMode() == ViAudioBitrate::Average)
	{
		formats = currentCodec->supportedBitrates();
		setBitrateVisible(true);
		setMinimumBitrateVisible(false);
		setMaximumBitrateVisible(false);
		values = formats.keys();
		names = formats.values();
		for(int i = 0; i < formats.size(); ++i)
		{
			mUi->bitrateBox->addItem(names[i], values[i]);
		}
		mUi->bitrateBox->setCurrentIndex(mUi->bitrateBox->findData(formats.defaultValue()));
	}
	else
	{
		setBitrateVisible(false);
		setMinimumBitrateVisible(false);
		setMaximumBitrateVisible(false);
	}
}

QString ViFormatWidget::codec()
{
	return mUi->codecBox->itemData(mUi->codecBox->currentIndex()).toString();
}

int ViFormatWidget::sampleSize()
{
	return mUi->sampleSizeBox->itemData(mUi->sampleSizeBox->currentIndex()).toInt();
}

int ViFormatWidget::sampleRate()
{
	return mUi->sampleRateBox->itemData(mUi->sampleRateBox->currentIndex()).toInt();
}

int ViFormatWidget::channels()
{
	return mUi->channelsBox->itemData(mUi->channelsBox->currentIndex()).toInt();
}

int ViFormatWidget::quality()
{
	return mUi->qualityBox->itemData(mUi->qualityBox->currentIndex()).toInt();
}

int ViFormatWidget::bitrateMode()
{
	return mUi->bitrateModeBox->itemData(mUi->bitrateModeBox->currentIndex()).toInt();
}

int ViFormatWidget::bitrate()
{
	return mUi->bitrateBox->itemData(mUi->bitrateBox->currentIndex()).toInt();
}

int ViFormatWidget::minimumBitrate()
{
	return mUi->minimumBitrateBox->itemData(mUi->minimumBitrateBox->currentIndex()).toInt();
}

int ViFormatWidget::maximumBitrate()
{
	return mUi->maximumBitrateBox->itemData(mUi->maximumBitrateBox->currentIndex()).toInt();
}

void ViFormatWidget::setSampleSizeVisible(bool visible)
{
	mUi->sampleSizeBox->setVisible(visible);
	mUi->sampleSizeLabel->setVisible(visible);
}

void ViFormatWidget::setSampleRateVisible(bool visible)
{
	mUi->sampleRateBox->setVisible(visible);
	mUi->sampleRateLabel->setVisible(visible);
}
void ViFormatWidget::setChannelsVisible(bool visible)
{
	mUi->channelsBox->setVisible(visible);
	mUi->channelsLabel->setVisible(visible);
}

void ViFormatWidget::setQualityVisible(bool visible)
{
	mUi->qualityBox->setVisible(visible);
	mUi->qualityLabel->setVisible(visible);
}

void ViFormatWidget::setBitrateModeVisible(bool visible)
{
	mUi->bitrateModeBox->setVisible(visible);
	mUi->bitrateModeLabel->setVisible(visible);
}

void ViFormatWidget::setBitrateVisible(bool visible)
{
	mUi->bitrateBox->setVisible(visible);
	mUi->bitrateLabel->setVisible(visible);
}

void ViFormatWidget::setMinimumBitrateVisible(bool visible)
{
	mUi->minimumBitrateBox->setVisible(visible);
	mUi->minimumBitrateLabel->setVisible(visible);
}

void ViFormatWidget::setMaximumBitrateVisible(bool visible)
{
	mUi->maximumBitrateBox->setVisible(visible);
	mUi->maximumBitrateLabel->setVisible(visible);
}
