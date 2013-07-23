#ifndef VICORRELATIONINGWIDGET_H
#define VICORRELATIONINGWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViCorrelationWidget;
}

class ViCorrelationWidget : public ViWidget
{
	Q_OBJECT

	public slots:

		void changeCorrelator(QString correlator);

	public:

		ViCorrelationWidget(QWidget *parent = 0);
		~ViCorrelationWidget();

        void clear();

		void setData(ViAudioObjectPointer object);
		void setData(ViAudioObjectQueue objects);
		void setData(ViProject *project);
		void setData(ViProject *project, ViAudioObjectQueue objects);

	protected:

        QString percentage(qreal value);

	private:

		Ui::ViCorrelationWidget *mUi;
		ViAudioObjectQueue mObjects;
		ViProject *mProject;

};

#endif
