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
		void clearSongInfo();
		void switchInfo();

	public:

		ViSongInfoWidget(QWidget *parent = 0);
		~ViSongInfoWidget();

	private:

		Ui::ViSongInfoWidget *mUi;
		QTimer *mTimer;

};

#endif
