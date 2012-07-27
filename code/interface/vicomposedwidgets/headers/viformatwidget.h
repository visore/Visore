#ifndef VIFORMATWIDGET_H
#define VIFORMATWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViFormatWidget;
}

class ViFormatWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void changeCodec();
		void changeBitrate();

	public:

		ViFormatWidget(QWidget *parent = 0);
		~ViFormatWidget();
		ViAudioFormat format();

	private:

		QString codec();
		int sampleSize();
		int sampleRate();
		int channels();
		int quality();
		int bitrateMode();
		int bitrate();
		int minimumBitrate();
		int maximumBitrate();

		void setSampleSizeVisible(bool visible);
		void setSampleRateVisible(bool visible);
		void setChannelsVisible(bool visible);
		void setQualityVisible(bool visible);
		void setBitrateModeVisible(bool visible);
		void setBitrateVisible(bool visible);
		void setMinimumBitrateVisible(bool visible);
		void setMaximumBitrateVisible(bool visible);

	private:

		Ui::ViFormatWidget *mUi;

};

#endif
