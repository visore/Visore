#ifndef VIVOLUMEWIDGET_H
#define VIVOLUMEWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViVolumeWidget;
}

class ViVolumeWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void volumeChanged(int volume = 0);

	public:
		ViVolumeWidget(ViAudioEngine *engine, QWidget *parent = 0);
		~ViVolumeWidget();

	private:
		Ui::ViVolumeWidget *mUi;
};

#endif
