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

	public slots:

		void progress(qreal percentage);

	private slots:

		void displayNextImage();

	public:

		enum TextStyle
		{
			None = 0,
			Percentage = 1,
			Text = 2
		};
		ViLoadingWidget(QWidget *parent = 0, bool animation = true, bool button = true, ViLoadingWidget::TextStyle style = ViLoadingWidget::Percentage);
		~ViLoadingWidget();

		void showAnimation(bool show);
		void showButton(bool show);
		void setTextStyle(ViLoadingWidget::TextStyle style);

		void setVisible(bool visible);
		void setText(QString text = "");

	private:

		void start();
		void stop();

	private:

		Ui::ViLoadingWidget *mUi;
		QTimer *mTimer;
		int mCurrentImage;
		ViLoadingWidget::TextStyle mTextStyle;
		bool mHasAnimation;

};

#endif
