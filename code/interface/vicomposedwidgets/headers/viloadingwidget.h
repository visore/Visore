#ifndef VILOADINGWIDGET_H
#define VILOADINGWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"
#include "viprogressbar.h"
#include <QTimer>

namespace Ui
{
    class ViLoadingWidget;
}

class ViLoadingWidget : public ViWidget
{
	Q_OBJECT

	public slots:

		void start();
		void start(bool animation, bool button, QString text, ViProgressBar::TextStyle textStyle, ViProgressBar::ProgressStyle progressStyle);
		void stop();
		void setText(QString text = "");

	private slots:

		void progress(short percentage);
		void displayNextImage();

	public:

		~ViLoadingWidget();

		static ViLoadingWidget& instance();

		void showAnimation(bool show);
		void showButton(bool show);
		void setTextStyle(ViProgressBar::TextStyle style);
		void setProgressStyle(ViProgressBar::ProgressStyle style);

	protected:

		ViLoadingWidget();

	private:

		void startAnimation();
		void stopAnimation();

	private:

		Ui::ViLoadingWidget *mUi;
		QTimer mTimer;
		int mCurrentImage;
		bool mHasAnimation;

};

#endif
