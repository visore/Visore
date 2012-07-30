#ifndef VISPECTRUMANALYSISWIDGET_H
#define VISPECTRUMANALYSISWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViSpectrumAnalysisWidget;
}

class ViSpectrumAnalysisWidget : public ViWidget
{
	Q_OBJECT

	public slots:

		void recalculate();
		void replot();

	public:

		ViSpectrumAnalysisWidget(QWidget *parent = 0);
		~ViSpectrumAnalysisWidget();

	protected:

		void showEvent(QShowEvent *event);

	private:

		Ui::ViSpectrumAnalysisWidget *mUi;
		bool mWasInitialized;

};

#endif
