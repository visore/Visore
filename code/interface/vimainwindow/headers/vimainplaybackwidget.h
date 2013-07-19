#ifndef VIMAINPLAYBACKWIDGET_H
#define VIMAINPLAYBACKWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMainPlaybackWidget;
}

class ViMainPlaybackWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void loadTrack();
		void changeMetadata();

	public:

		ViMainPlaybackWidget(QWidget *parent = 0);
		~ViMainPlaybackWidget();

		void clear();

	private:

		Ui::ViMainPlaybackWidget *mUi;

};

#endif
