#ifndef VIPROJECTRECORDINGWIDGET_H
#define VIPROJECTRECORDINGWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViProjectRecordingWidget;
}

class ViProjectRecordingWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void start();
		void changeType();

	public:

		ViProjectRecordingWidget(QWidget *parent = 0);
		~ViProjectRecordingWidget();

	private:

		Ui::ViProjectRecordingWidget *mUi;

};

#endif
