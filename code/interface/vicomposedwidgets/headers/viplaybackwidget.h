#ifndef VIPLAYBACKWIDGET_H
#define VIPLAYBACKWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViPlaybackWidget;
}

class ViPlaybackWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void record(bool checked);
		void play(bool checked);
		void pause(bool checked);
		void stop(bool checked);
		void inputChanged(ViAudioConnection::Type type);

	public:
		ViPlaybackWidget(QWidget *parent = 0);
		~ViPlaybackWidget();

	private:
		Ui::ViPlaybackWidget *mUi;
		bool mIsRecording;
		bool mIsPlaying;
		bool mIsPausing;
};

#endif
