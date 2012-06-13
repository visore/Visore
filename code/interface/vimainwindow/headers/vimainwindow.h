#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "viloadingwidget.h"

namespace Ui
{
    class ViMainWindow;
}

class ViMainWindow : public QMainWindow
{
	Q_OBJECT

	public slots:

		void progress(short percentage);
		void hideLoading();
		void showLoading(bool animation = true, bool button = true, ViLoadingWidget::TextStyle style = ViLoadingWidget::Percentage, QString text = "");

	public:

		~ViMainWindow();
		static ViMainWindow* instance();
		void show();

	protected:

		ViMainWindow();
		void resizeEvent(QResizeEvent *event);
		void moveEvent(QMoveEvent *event);
		void changeEvent(QEvent *event);

	private:
		void initialize();

	private:

		static ViMainWindow *mWindow;

		Ui::ViMainWindow *mUi;
		ViAudioEngine *mEngine;
		ViLoadingWidget *mLoadingWidget;
};

#endif
