#ifndef VILOADINGWIDGET_H
#define VILOADINGWIDGET_H

#include <viwidget.h>
#include <vithememanager.h>
#include <viprogressbar.h>
#include <visingleton.h>
#include <QTime>
#include <QTimer>

namespace Ui
{
    class ViLoadingWidget;
}

class ViLoadingWidget : public ViWidget, public ViSingleton<ViLoadingWidget>
{

	Q_OBJECT

	friend class ViSingleton<ViLoadingWidget>;

	public slots:

		void setText(QString text = "");

	private slots:

		void progress(qreal percentage);
		void updateTimes();

	public:

		~ViLoadingWidget();

		void showAnimation(bool show);
		void showButton(bool show);
		void setTextStyle(ViProgressBar::TextStyle style);
		void setProgressStyle(ViProgressBar::ProgressStyle style);

		static void start();
		static void start(bool animation, bool button, QString text, ViProgressBar::TextStyle textStyle, ViProgressBar::ProgressStyle progressStyle);
		static void stop();

	protected:

		ViLoadingWidget();
		qint64 remainingTime(qint64 elapsed, qreal percentage);
		void enableRemainingTime(bool enable);

	private:

		Ui::ViLoadingWidget *mUi;
		bool mHasAnimation;
		QTime mTime;
		QTimer mTimer;

};

typedef ViPointer<ViLoadingWidget> ViLoadingWidgetPointer;

#endif
