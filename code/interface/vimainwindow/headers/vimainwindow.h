#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include "viaudioengine.h"

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

	public:
		ViMainWindow(QWidget *parent = 0);
		~ViMainWindow();
		void setEngine(ViAudioEngine *engine);

	private:
		void setRecording(bool active);
		void setPlaying(bool active);
		void setPausing(bool active);

	private:
		Ui::ViMainWindow *mUi;

	private:
		ViAudioEngine *mEngine;
		bool mIsRecording;
		bool mIsPlaying;
		bool mIsPaused;
};

#endif
