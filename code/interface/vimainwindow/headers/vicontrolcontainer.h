#ifndef VICONTROLCONTAINER_H
#define VICONTROLCONTAINER_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViControlContainer;
}

class ViControlContainer : public ViWidget
{
	Q_OBJECT

	public:
		ViControlContainer(QWidget *parent = 0);
		~ViControlContainer();
		void setEngine(ViAudioEngine *engine);

	private:
		Ui::ViControlContainer *mUi;
};

#endif
