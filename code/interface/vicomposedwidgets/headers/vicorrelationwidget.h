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

    private slots:

        void updateCorrelators();
        void changeCorrelator();

	public:

		ViCorrelationWidget(QWidget *parent = 0);
		~ViCorrelationWidget();

        void clear();

        void setProject(ViProject *project);

	protected:

        QString percentage(qreal value);
        void adjustTables();

	private:

		Ui::ViCorrelationWidget *mUi;
        ViProject *mProject;

};

#endif
