#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

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
    class ViMainWindow;
}

class ViMainWindow : public QMainWindow
{
	Q_OBJECT

	private slots:
		void selectLineInput();
		void selectFileInput();

	public:
		ViMainWindow(ViAudioEngine *engine, QWidget *parent = 0);
		~ViMainWindow();

	private:
		Ui::ViMainWindow *mUi;

	private:
		ViAudioEngine *mEngine;

		ViPlaybackWidget *mPlaybackWidget;
		ViWaveFormWidget *mOriginalWaveWidget;
		ViWaveFormWidget *mCorrectedWaveWidget;
		//ViVolumeDialWidget *mVolumeWidget;
		ViVolumeBarWidget *mVolumeWidget;
};

#endif
