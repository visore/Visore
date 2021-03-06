#ifndef VIGENERALCORRECTIONINGWIDGET_H
#define VIGENERALCORRECTIONINGWIDGET_H

#include <viwidget.h>
#include <vicorrectionmode.h>

namespace Ui
{
	class ViGeneralCorrectionWidget;
}

class ViGeneralCorrectionWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	private slots:

		void toggleDetector();
		void changeNoiseDetector();
		void changeNoiseSize(int size);

	public:

		ViGeneralCorrectionWidget(QWidget *parent = 0);
		~ViGeneralCorrectionWidget();

		void setMode(ViCorrectionMode::Mode mode);

		int windowSize();
		ViProcessor::ChannelMode channelMode();
		ViProcessor::ProcessMode processMode();
		ViModifyProcessor::ModifyMode modifyMode();
		ViNoiseDetector* noiseDetector();

	private:

		Ui::ViGeneralCorrectionWidget *mUi;
		QString mDefaultDetector;

};

#endif
