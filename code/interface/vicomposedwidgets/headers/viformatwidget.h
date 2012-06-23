#ifndef VIFORMATWIDGET_H
#define VIFORMATWIDGET_H

#include <QFileDialog>
#include "vicodecmanager.h"
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViFormatWidget;
}

class ViFormatWidget : public ViWidget
{
	Q_OBJECT

	public:

		ViFormatWidget(QWidget *parent = 0);
		~ViFormatWidget();
		ViAudioFormat format();

	private:

		void populate();
		void setDefaults();

	private:

		Ui::ViFormatWidget *mUi;

};

#endif
