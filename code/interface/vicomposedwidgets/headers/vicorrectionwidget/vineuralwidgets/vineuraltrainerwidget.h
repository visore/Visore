#ifndef ViNeuralTrainerWidget_H
#define ViNeuralTrainerWidget_H

#include <viwidget.h>
#include <vitrainer.h>
#include <vicorrectionmode.h>

namespace Ui
{
	class ViNeuralTrainerWidget;
}

class ViNeuralTrainerWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	public:

		ViNeuralTrainerWidget(QWidget *parent = 0);
		~ViNeuralTrainerWidget();

		void setMode(ViCorrectionMode::Mode mode);

		ViTrainer* trainer();

	private:

		Ui::ViNeuralTrainerWidget *mUi;
		QString mDefaultTrainer;

};

#endif
