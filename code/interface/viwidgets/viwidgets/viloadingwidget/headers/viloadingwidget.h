#ifndef VILOADINGWIDGET_H
#define VILOADINGWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"
#include <QTimer>

namespace Ui
{
    class ViLoadingWidget;
}

class ViLoadingWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void displayNextImage();

	public:
		ViLoadingWidget(QWidget *parent = 0);
		~ViLoadingWidget();
		void setVisible(bool visible);

	private:
		void start();
		void stop();

	private:
		Ui::ViLoadingWidget *mUi;
		QTimer *mTimer;
		int mCurrentImage;
};

#endif
