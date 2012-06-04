#ifndef VIMATCHINGWIDGET_H
#define VIMATCHINGWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViMatchWidget;
}

class ViMatchWidget : public ViWidget
{
	Q_OBJECT

	public:

		ViMatchWidget(QWidget *parent = 0);
		~ViMatchWidget();

	private:

		Ui::ViMatchWidget *mUi;

};

#endif
