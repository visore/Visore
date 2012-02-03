#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include "viaudioengine.h"
#include "viwaveformwidget.h"

namespace Ui
{
    class ViMainWindow;
}

class ViMainWindow : public QMainWindow
{
	Q_OBJECT

	private slots:
		void reset();
		void record();
		void save();
		void play();
		void pause();
		void stop();
		void selectLineInput();
		void selectFileInput();

	public:
		ViMainWindow(ViAudioEngine *engine, QWidget *parent = 0);
		~ViMainWindow();

	private:
		void setRecording(bool active);
		void setPlaying(bool active);
		void setPausing(bool active);

	private:
		Ui::ViMainWindow *mUi;

	private:
		ViAudioEngine *mEngine;
		ViWaveFormWidget *mWaveFormWidget;
		bool mIsRecording;
		bool mIsPlaying;
		bool mIsPaused;
};

#endif
