#include <vimetadatawidget.h>
#include <ui_vimetadatawidget.h>
#include <visizeconverter.h>
#include <vitimeconverter.h>
#include <viaudiocodec.h>

ViMetadataWidget::ViMetadataWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMetadataWidget();
	mUi->setupUi(this);

	setStyleSheet("QLabel { min-height: 15px; }");

	mUi->coverWidget->setSize(300);

	mUi->titleLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->artistLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->albumLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->durationLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->fileSizeLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->codecLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->bitrateLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->qualityLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->sampleSizeLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->sampleRateLabel->setStyleSheet("min-width: 120px; max-width: 120px;");
	mUi->channelsLabel->setStyleSheet("min-width: 120px; max-width: 120px;");

	clear();
}

ViMetadataWidget::~ViMetadataWidget()
{
	delete mUi;
}

void ViMetadataWidget::clear()
{
	mUi->title->setText("");
	mUi->artist->setText("");
	mUi->album->setText("");
	mUi->duration->setText("");
	mUi->fileSize->setText("");
	mUi->codec->setText("");
	mUi->bitrate->setText("");
	mUi->quality->setText("");
	mUi->sampleSize->setText("");
	mUi->sampleRate->setText("");
	mUi->channels->setText("");

	mUi->metadataContainer->hide();
	mUi->fileContainer->hide();
	mUi->formatContainer->hide();
	mUi->coverWidget->hide();

	mUi->line1->hide();
	mUi->line2->hide();

	mUi->coverWidget->setImage(ViThemeManager::image("cover").path());
}

void ViMetadataWidget::setMetadata(const ViMetadata &metadata)
{
	mUi->metadataContainer->show();
	mUi->title->setText(metadata.title());
	mUi->artist->setText(metadata.artist());
	mUi->album->setText(metadata.album());

	mUi->coverWidget->show();
	if(metadata.hasCover()) mUi->coverWidget->setImage(metadata.cover());
	else mUi->coverWidget->setImage(ViThemeManager::image("cover").path());
}

void ViMetadataWidget::setFileInfo(const qint64 &milliseconds, const qint64 &fileBytes)
{
	mUi->fileContainer->show();
	mUi->line1->show();
	mUi->duration->setText(ViTimeConverter::toOptimalString(milliseconds, ViTimeConverter::Milliseconds));
	mUi->fileSize->setText(ViSizeConverter::toOptimalString(fileBytes, ViSizeConverter::Bytes));
}

void ViMetadataWidget::setFormat(const ViAudioFormat &format)
{
	mUi->formatContainer->show();
	mUi->line2->show();

	if(format.hasCodec()) mUi->codec->setText(format.codec()->abbreviation());
	else mUi->codec->setText("Unknown Codec");

	if(format.bitrateMode() == ViAudioBitrate::Unknown)
	{
		mUi->bitrate->hide();
		mUi->bitrateLabel->hide();
	}
	else
	{
		mUi->bitrate->show();
		mUi->bitrateLabel->show();
		mUi->bitrate->setText(QString::number(format.bitrate().normal()) + "kbps (" + ViAudioBitrate::toString(format.bitrateMode()) + " Bitrate)");
	}

	if(format.quality() == ViAudioFormat::None)
	{
		mUi->quality->hide();
		mUi->qualityLabel->hide();
	}
	else
	{
		mUi->quality->show();
		mUi->qualityLabel->show();
		mUi->quality->setText(ViAudioFormat::toString(format.quality()) + "Quality");
	}

	mUi->sampleSize->setText(QString::number(format.sampleSize()) + "bits (" + ViAudioFormat::toString(format.sampleType()) + ")");

	mUi->sampleRate->setText(QString::number(format.sampleRate()) + "Hz");

	mUi->channels->setText(QString::number(format.channels()) + " (" + ViAudioFormat::toStringChannels(format.channels()) + ")");
}
