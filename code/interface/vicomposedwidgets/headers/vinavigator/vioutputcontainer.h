#ifndef VIOUTPUTCONTAINER_H
#define VIOUTPUTCONTAINER_H

#include "vivolumebarwidget.h"
#include "viplaybackwidget.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViOutputContainer;
}

class ViOutputContainer : public ViWidget
{
	Q_OBJECT

	public:
		ViOutputContainer(QWidget *parent = 0);
		~ViOutputContainer();

	private:
		Ui::ViOutputContainer *mUi;
};

#endif
