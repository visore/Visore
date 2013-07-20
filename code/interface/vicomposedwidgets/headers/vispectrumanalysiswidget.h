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

	signals:

		void windowSizeChanged();
		void windowFunctionChanged();

	private slots:

		void checkNotation();
		void replot();

	public:

		ViSpectrumAnalysisWidget(QWidget *parent = 0);
		~ViSpectrumAnalysisWidget();

		void clear();

		int windowSize();
		QString windowFunction();

		void setSpectrum(ViRealSpectrum *spectrum);

	private:

		Ui::ViSpectrumAnalysisWidget *mUi;
		ViRealSpectrum *mSpectrum;

};

#endif
