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
		void unpaused();
		void stopped();

	public slots:
		
		void play();
		void pause();
		void stop();

	public:

		ViPlaybackWidget(QWidget *parent = 0);
		~ViPlaybackWidget();

		void clear();

	private:

		Ui::ViPlaybackWidget *mUi;

};

#endif
