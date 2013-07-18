#ifndef VIMAINWAVEWIDGET_H
#define VIMAINWAVEWIDGET_H

#include <viwidget.h>
#include <viwavewidgetgroup.h>

namespace Ui
{
	class ViMainWaveWidget;
}

class ViMainWaveWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void clearProject();
		void generateWave();
		void drawWave();

	public:

		ViMainWaveWidget(QWidget *parent = 0);
		~ViMainWaveWidget();

		void clear();

	private:

		Ui::ViMainWaveWidget *mUi;
		ViWaveWidgetGroup mWaveGroup;
		QList<QPair<ViAudioObjectPointer, bool>> mAlignments;

};

#endif
