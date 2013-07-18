#ifndef VIMAINCORRELATIONWIDGET_H
#define VIMAINCORRELATIONWIDGET_H

#include <viwidget.h>
#include <vibutton.h>

namespace Ui
{
	class ViMainCorrelationWidget;
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
		ViButton *mButton;

};

class ViMainCorrelationWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void addProjects();
		void remove(int row);
		void correlateTracks();

	public:

		ViMainCorrelationWidget(QWidget *parent = 0);
		~ViMainCorrelationWidget();

	private:

		Ui::ViMainCorrelationWidget *mUi;
		QList<ViRemoveWidget*> mButtons;

};

#endif
