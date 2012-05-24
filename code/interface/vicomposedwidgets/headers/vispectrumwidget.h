#ifndef VISPECTRUMWIDGET_H
#define VISPECTRUMWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"
#include "viloadingwidget.h"

namespace Ui
{
    class ViSpectrumWidget;
}

class ViSpectrumWidget : public ViWidget
{
	Q_OBJECT

	public slots:

		void recalculate();
		void replot();

	public:

		ViSpectrumWidget(QWidget *parent = 0);
		~ViSpectrumWidget();
		void setEngine(ViAudioEngine *engine);
		void show();

	private:

		Ui::ViSpectrumWidget *mUi;
		ViLoadingWidget *mLoadingWidget;

};

#endif
