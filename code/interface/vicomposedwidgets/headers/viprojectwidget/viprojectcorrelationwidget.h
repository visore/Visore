#ifndef VIPROJECTCORRELATIONWIDGET_H
#define VIPROJECTCORRELATIONWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"
#include "vipushbutton.h"

namespace Ui
{
    class ViProjectCorrelationWidget;
}

class ViRemoveWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void clicked(int row);

	private slots:

		void click();

	public:

		ViRemoveWidget(int row);
		~ViRemoveWidget();
		int row();
		void setRow(int row);

	private:

		int mRow;
		ViPushButton *mButton;

};

class ViProjectCorrelationWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void addProjects();
		void remove(int row);
		void start();

	public:

		ViProjectCorrelationWidget(QWidget *parent = 0);
		~ViProjectCorrelationWidget();

	private:

		Ui::ViProjectCorrelationWidget *mUi;
		QList<ViRemoveWidget*> mButtons;

};

#endif
