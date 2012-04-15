#ifndef VIVOLUMEBARWIDGET_H
#define VIVOLUMEBARWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViVolumeBarWidget;
}

class ViVolumeBarWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void volumeChanged(int volume = 0);
		void mute(bool value);

	public:
		ViVolumeBarWidget(QWidget *parent = 0);
		~ViVolumeBarWidget();
		void setEngine(ViAudioEngine *engine);

	private:
		Ui::ViVolumeBarWidget *mUi;
};

#endif