#ifndef VIMAINRECORDINGWIDGET_H
#define VIMAINRECORDINGWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
	class ViMainRecordingWidget;
}

class ViMainRecordingWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void load();
		void start();
		void changeType();
		void clear();
		void finishRecording();

	public:

		ViMainRecordingWidget(QWidget *parent = 0);
		~ViMainRecordingWidget();

	private:

		Ui::ViMainRecordingWidget *mUi;
		ViProject *mProject;

};

#endif
