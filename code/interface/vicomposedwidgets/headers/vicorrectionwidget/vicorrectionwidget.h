#ifndef VICORRECTIONINGWIDGET_H
#define VICORRECTIONINGWIDGET_H

#include <viwidget.h>
#include <QTableWidget>
#include <vicorrectionmode.h>

namespace Ui
{
	class ViCorrectionWidget;
}

class ViCorrectionWidget : public ViWidget
{

	Q_OBJECT

	public slots:

		void changeCorrector(QString name);
		void changeMode(ViCorrectionMode::Mode mode);

	public:

		ViCorrectionWidget(QWidget *parent = 0);
		~ViCorrectionWidget();

		void clear();

		QStringList correctors();
		ViModifyProcessor* corrector();

	private:

		ViModifyProcessor* neuralCorrector(QTabWidget *tabWidget);

		void addTab(QTabWidget *tabWidget, const QString &text, QWidget *widget);

	private:

		Ui::ViCorrectionWidget *mUi;
		QStringList mCorrectors;
		int mCurrentCorrector;
		ViCorrectionMode::Mode mMode;

};

#endif
