#ifndef VIPLAYBACKWIDGET_H
#define VIPLAYBACKWIDGET_H

#include <viwidget.h>

namespace Ui
{
    class ViPlaybackWidget;
}

class ViPlaybackWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void played();
		void paused();
		void stopped();
		void volumeChanged(int volume);
		void positionMoved(ViAudioPosition position);

	public slots:
		
		void play();
		void pause();
		void stop();
		void changeVolume(int volume);
		void changePosition(ViAudioPosition position);
		void changeDuration(ViAudioPosition duration);

	public:

		ViPlaybackWidget(QWidget *parent = 0);
		~ViPlaybackWidget();

		void clear();

	private:

		Ui::ViPlaybackWidget *mUi;

};

#endif
