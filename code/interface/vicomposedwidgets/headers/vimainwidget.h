#ifndef VIMAINWIDGET_H
#define VIMAINWIDGET_H

#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViMainWidget;
}

class ViMainWidget : public ViWidget
{

	public:
		ViMainWidget(QWidget *parent = 0);
		~ViMainWidget();

	private:
		Ui::ViMainWidget *mUi;
};

#endif
