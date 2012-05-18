#ifndef VISPECTRUMWIDGET_H
#define VISPECTRUMWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViSpectrumWidget;
}

class ViSpectrumWidget : public ViWidget
{
	Q_OBJECT

	public:
		ViSpectrumWidget(QWidget *parent = 0);
		~ViSpectrumWidget();
		void setEngine(ViAudioEngine *engine);
		void recalculate();

	private:
		Ui::ViSpectrumWidget *mUi;
};

#endif
