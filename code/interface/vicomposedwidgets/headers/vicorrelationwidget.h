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

		void setObject(ViAudioObjectPointer object);
		void setObjects(ViAudioObjectQueue objects);
		void setProject(ViProject *project);

	protected:

        QString percentage(qreal value);
        void adjustTables();

	private:

		Ui::ViCorrelationWidget *mUi;
		ViAudioObjectQueue mObjects;
		ViProject *mProject;

};

#endif
