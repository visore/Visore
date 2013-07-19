#ifndef VIVOLUMEWIDGET_H
#define VIVOLUMEWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViVolumeWidget;
}

class ViVolumeWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void volumeChanged(int volume);
		void muted();
		void unmuted();

	public slots:

		void mute(bool mute = true);
		void unmute(bool unmute = true);

	public:

		ViVolumeWidget(QWidget *parent = 0);
		~ViVolumeWidget();

		void clear();

	private:

		Ui::ViVolumeWidget *mUi;

};

#endif
