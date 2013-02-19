#ifndef VIWAVEANALYSISWIDGET_H
#define VIWAVEANALYSISWIDGET_H

#include <viwidget.h>
#include <viwavewidgetgroup.h>
#include <QHideEvent>

namespace Ui
{
    class ViWaveAnalysisWidget;
}

class ViWaveAnalysisWidget : public ViWidget
{

	Q_OBJECT

	public slots:

		void clear();

	private slots:

		void generateWave();
		void drawWave();

	public:

		ViWaveAnalysisWidget(QWidget *parent = 0);
		~ViWaveAnalysisWidget();

	protected:

		void hideEvent(QHideEvent *event);

	private:

		Ui::ViWaveAnalysisWidget *mUi;
		ViWaveWidgetGroup mWaveGroup;

};

#endif
