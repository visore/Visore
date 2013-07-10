#ifndef VICORRECTIONINGWIDGET_H
#define VICORRECTIONINGWIDGET_H

#include <viwidget.h>
#include <QTableWidget>

namespace Ui
{
	class ViCorrectionWidget;
}

class ViCorrectionWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void selectCorrector();
		void selectMode();

	public:

		ViCorrectionWidget(QWidget *parent = 0);
		~ViCorrectionWidget();

		ViModifyProcessor* corrector();

	private:

		ViModifyProcessor* neuralCorrector(QTabWidget *tabWidget);

		void addTab(QTabWidget *tabWidget, const QString &text, QWidget *widget);

	private:

		Ui::ViCorrectionWidget *mUi;

};

#endif
