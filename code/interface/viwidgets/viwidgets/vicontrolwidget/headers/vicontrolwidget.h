#ifndef VICONTROLWIDGET_H
#define VICONTROLWIDGET_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViControlWidget;
}

class ViControlWidget : public ViWidget
{
	Q_OBJECT

	public:
		ViControlWidget(QWidget *parent = 0);
		~ViControlWidget();
		void setEngine(ViAudioEngine *engine);

	private:
		Ui::ViControlWidget *mUi;
};

#endif
