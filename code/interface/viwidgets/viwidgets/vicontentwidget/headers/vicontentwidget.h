#ifndef VIWINDOWWIDGET_H
#define VIWINDOWWIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include "viaudioengine.h"
#include "viwaveformwidget.h"
#include "vivolumedialwidget.h"
#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViContentWidget;
}

class ViContentWidget : public QWidget
{
	Q_OBJECT

	private slots:
		void selectLineInput();
		void selectFileInput();

	public:
		ViContentWidget(QWidget *parent = 0);
		~ViContentWidget();
		void initialize(ViAudioEngine *engine);

	private:
		Ui::ViContentWidget *mUi;

	private:
		ViAudioEngine *mEngine;

		ViPlaybackWidget *mPlaybackWidget;
		ViWaveFormWidget *mOriginalWaveWidget;
		ViWaveFormWidget *mCorrectedWaveWidget;
		//ViVolumeDialWidget *mVolumeWidget;
		ViVolumeBarWidget *mVolumeWidget;
};

#endif
