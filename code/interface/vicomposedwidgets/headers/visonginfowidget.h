#ifndef VISONGINFOWIDGET_H
#define VISONGINFOWIDGET_H

#include "viwidget.h"
#include <QTimer>

namespace Ui
{
    class ViSongInfoWidget;
}

class ViSongInfoWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void changeSongInfo(ViSongInfo info);
		void switchInfo();

	public:
		ViSongInfoWidget(QWidget *parent = 0);
		~ViSongInfoWidget();
		void setEngine(ViAudioEngine *engine);

	private:
		Ui::ViSongInfoWidget *mUi;
		QTimer *mTimer;
};

#endif