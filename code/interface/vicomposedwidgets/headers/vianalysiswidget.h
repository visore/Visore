#ifndef VIANALYSISWIDGET_H
#define VIANALYSISWIDGET_H

#include "vithememanager.h"
#include "viwidget.h"
#include "vispectrumwidget.h"

namespace Ui
{
    class ViAnalysisWidget;
}

class ViAnalysisWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void openSpectrum();

	public:
		ViAnalysisWidget(QWidget *parent = 0);
		~ViAnalysisWidget();

	private:
		Ui::ViAnalysisWidget *mUi;
		ViSpectrumWidget mSpectrum;

};

#endif
