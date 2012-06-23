#include "viformatwidget.h"
#include "ui_viformatwidget.h"

ViFormatWidget::ViFormatWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViFormatWidget();
	mUi->setupUi(this);
	populate();
	setDefaults();
}

ViFormatWidget::~ViFormatWidget()
{
	delete mUi;
}

ViAudioFormat ViFormatWidget::format()
{
	ViAudioFormat result;
	result.setCodec(ViCodecManager::selected(mUi->codecBox->itemData(mUi->codecBox->currentIndex()).toString()));
	result.setByteOrder((ViAudioFormat::Endian) mUi->byteOrderBox->itemData(mUi->byteOrderBox->currentIndex()).toInt());
	result.setSampleType((ViAudioFormat::SampleType) mUi->sampleTypeBox->itemData(mUi->sampleTypeBox->currentIndex()).toInt());
	result.setSampleRate(mUi->sampleRateBox->itemData(mUi->sampleRateBox->currentIndex()).toInt());
	result.setSampleSize(mUi->sampleSizeBox->itemData(mUi->sampleSizeBox->currentIndex()).toInt());
	result.setChannelCount(mUi->channelsBox->itemData(mUi->channelsBox->currentIndex()).toInt());
	return result;
}

void ViFormatWidget::populate()
{
	QList<ViCodec> codecs = ViCodecManager::selected(ViCodec::OutputType);
	codecs.append(ViCodecManager::selected(ViCodec::InputOutputType));
	for(int i = 0; i < codecs.size(); ++i)
	{
		mUi->codecBox->addItem(codecs[i].abbreviation() + " (" + codecs[i].name() + ")", codecs[i].abbreviation());
	}

	QList<ViAudioFormat::Endian> byteOrders = ViCodecManager::byteOrders();
	for(int i = 0; i < byteOrders.size(); ++i)
	{
		if(byteOrders[i] == ViAudioFormat::LittleEndian)
		{
			mUi->byteOrderBox->addItem("Little Endian", ViAudioFormat::LittleEndian);
		}
		else if(byteOrders[i] == ViAudioFormat::BigEndian)
		{
			mUi->byteOrderBox->addItem("Big Endian", ViAudioFormat::BigEndian);
		}
	}

	QList<ViAudioFormat::SampleType> sampleTypes = ViCodecManager::sampleTypes();
	for(int i = 0; i < sampleTypes.size(); ++i)
	{
		if(sampleTypes[i] == ViAudioFormat::SignedInt)
		{
			mUi->sampleTypeBox->addItem("Signed Integer", ViAudioFormat::SignedInt);
		}
		else if(sampleTypes[i] == ViAudioFormat::UnSignedInt)
		{
			mUi->sampleTypeBox->addItem("Unsigned Integer", ViAudioFormat::UnSignedInt);
		}
		else if(sampleTypes[i] == ViAudioFormat::Float)
		{
			mUi->sampleTypeBox->addItem("Float", ViAudioFormat::Float);
		}
	}

	QList<qint32> sampleRates = ViCodecManager::sampleRates();
	for(int i = 0; i < sampleRates.size(); ++i)
	{
		mUi->sampleRateBox->addItem(QString::number(sampleRates[i]) + " Hz", sampleRates[i]);
	}

	QList<qint8> sampleSizes = ViCodecManager::sampleSizes();
	for(int i = 0; i < sampleSizes.size(); ++i)
	{
		mUi->sampleSizeBox->addItem(QString::number(sampleSizes[i]) + " bit", sampleSizes[i]);
	}

	QList<qint8> channels = ViCodecManager::channels();
		for(int i = 0; i < channels.size(); ++i)
	{
		if(channels[i] == 1)
		{
			mUi->channelsBox->addItem("Mono (1 channel)", 1);
		}
		else if (channels[i] == 2)
		{
			mUi->channelsBox->addItem("Stereo (2 channels)", 2);
		}
	}
}

void ViFormatWidget::setDefaults()
{
	for(int i = 0; i < mUi->codecBox->count(); ++i)
	{
		if(mUi->codecBox->itemData(i) == "WAV")
		{
			mUi->codecBox->setCurrentIndex(i);
			break;
		}
	}

	for(int i = 0; i < mUi->byteOrderBox->count(); ++i)
	{
		if(mUi->byteOrderBox->itemData(i) == ViAudioFormat::LittleEndian)
		{
			mUi->byteOrderBox->setCurrentIndex(i);
			break;
		}
	}

	for(int i = 0; i < mUi->sampleTypeBox->count(); ++i)
	{
		if(mUi->sampleTypeBox->itemData(i) == ViAudioFormat::SignedInt)
		{
			mUi->sampleTypeBox->setCurrentIndex(i);
			break;
		}
	}

	for(int i = 0; i < mUi->sampleRateBox->count(); ++i)
	{
		if(mUi->sampleRateBox->itemData(i) == 44100)
		{
			mUi->sampleRateBox->setCurrentIndex(i);
			break;
		}
	}

	for(int i = 0; i < mUi->sampleSizeBox->count(); ++i)
	{
		if(mUi->sampleSizeBox->itemData(i) == 16)
		{
			mUi->sampleSizeBox->setCurrentIndex(i);
			break;
		}
	}

	for(int i = 0; i < mUi->channelsBox->count(); ++i)
	{
		if(mUi->channelsBox->itemData(i) == 2)
		{
			mUi->channelsBox->setCurrentIndex(i);
			break;
		}
	}
}
