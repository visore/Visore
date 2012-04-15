#ifndef VIVOLUMEDIALWIDGET_H
#define VIVOLUMEDIALWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViVolumeDialWidget;
}

class ViVolumeDialWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void volumeChanged(int volume = 0);

	public:
		ViVolumeDialWidget(QWidget *parent = 0);
		~ViVolumeDialWidget();

	private:
		Ui::ViVolumeDialWidget *mUi;
};

#endif
