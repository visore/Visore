#include "vioutputcontrolwidget.h"
#include "ui_vioutputcontrolwidget.h"

ViOutputControlWidget::ViOutputControlWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViOutputControlWidget();
	mUi->setupUi(this);
	populate();
	setDefaults();

	ViObject::connect(mUi->fileButton, SIGNAL(clicked()), this, SLOT(selectFileOutput()));
	ViObject::connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

ViOutputControlWidget::~ViOutputControlWidget()
{
	delete mUi;
}

void ViOutputControlWidget::selectFileOutput()
{
	ViCodec codec = ViCodecManager::selected(mUi->formatBox->itemData(mUi->formatBox->currentIndex()).toString());
	QString extensions = codec.abbreviation() + " (";
	QList<QString> list = codec.starExtensions();
	for(int i = 0; i < list.size(); ++i)
	{
		extensions += list[i];
		if(i < list.size() - 1)
		{
			extensions += " ";
		}
	}
	extensions += ")";
	QString file = QFileDialog::getSaveFileName(this, "Save Audio File", QDir::homePath() + QDir::separator() + "output." + codec.extensions()[0], extensions);
	mUi->fileLineEdit->setText(file);
}

void ViOutputControlWidget::save()
{
	/*ViAudioFormat format;
	format.setChannelCount(mUi->channelsBox->itemData(mUi->channelsBox->currentIndex()).toInt());
	format.setSampleRate(mUi->sampleRateBox->itemData(mUi->sampleRateBox->currentIndex()).toInt());
	format.setSampleSize(mUi->sampleSizeBox->itemData(mUi->sampleSizeBox->currentIndex()).toInt());
	format.setSampleType(QAudioFormat::SampleType (mUi->sampleTypeBox->itemData(mUi->sampleTypeBox->currentIndex()).toInt()));
	format.setByteOrder(QAudioFormat::Endian (mUi->byteOrderBox->itemData(mUi->byteOrderBox->currentIndex()).toInt()));
	format.setCodec(ViCodecManager::selected(mUi->formatBox->itemData(mUi->formatBox->currentIndex()).toString()));
	mEngine->createOutputFile(mUi->fileLineEdit->text(), format);*/
}

void ViOutputControlWidget::populate()
{
	QList<ViCodec> codecs = ViCodecManager::selected(ViCodec::OutputType);
	codecs.append(ViCodecManager::selected(ViCodec::InputOutputType));
	for(int i = 0; i < codecs.size(); ++i)
	{
		mUi->formatBox->addItem(codecs[i].abbreviation() + " (" + codecs[i].name() + ")", codecs[i].abbreviation());
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

void ViOutputControlWidget::setDefaults()
{
	for(int i = 0; i < mUi->formatBox->count(); ++i)
	{
		if(mUi->formatBox->itemData(i) == "WAV")
		{
			mUi->formatBox->setCurrentIndex(i);
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
