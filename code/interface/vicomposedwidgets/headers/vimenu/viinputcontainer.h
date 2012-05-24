#ifndef VIINPUTCONTAINER_H
#define VIINPUTCONTAINER_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViInputContainer;
}

class ViInputContainer : public ViWidget
{
	Q_OBJECT

	public:
		ViInputContainer(QWidget *parent = 0);
		~ViInputContainer();

	private:
		Ui::ViInputContainer *mUi;
};

#endif
