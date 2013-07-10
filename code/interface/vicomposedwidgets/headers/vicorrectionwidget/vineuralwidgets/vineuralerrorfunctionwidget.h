#ifndef VINEURALERRORFUNCTIONWIDGET_H
#define VINEURALERRORFUNCTIONWIDGET_H

#include <viwidget.h>
#include <vicorrectionmode.h>
#include <vierrorfunction.h>

namespace Ui
{
	class ViNeuralErrorFunctionWidget;
}

class ViNeuralErrorFunctionWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	private slots:

		void addErrorFunction();
		void removeErrorFunction();

	public:

		ViNeuralErrorFunctionWidget(QWidget *parent = 0);
		~ViNeuralErrorFunctionWidget();

		void setMode(ViCorrectionMode::Mode mode);

		QList<ViErrorFunction*> errorFunctions();

	private:

		Ui::ViNeuralErrorFunctionWidget *mUi;
		QString mDefaultFunction;

};

#endif
