#ifndef VIRECORDINGPROJECTWIDGET_H
#define VIRECORDINGPROJECTWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViRecordingProjectWidget;
}

class ViRecordingProjectWidget : public ViWidget
{
	Q_OBJECT

	public:

		ViRecordingProjectWidget(QWidget *parent = 0);
		~ViRecordingProjectWidget();

	private:

		Ui::ViRecordingProjectWidget *mUi;

};

#endif
