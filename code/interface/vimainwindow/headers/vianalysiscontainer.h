#ifndef VIANALYSISCONTAINER_H
#define VIANALYSISCONTAINER_H

#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViAnalysisContainer;
}

class ViAnalysisContainer : public ViWidget
{
	Q_OBJECT

	public:
		ViAnalysisContainer(QWidget *parent = 0);
		~ViAnalysisContainer();
		void setEngine(ViAudioEngine *engine);

	private:
		Ui::ViAnalysisContainer *mUi;
};

#endif
