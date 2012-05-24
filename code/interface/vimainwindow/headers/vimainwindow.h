#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "viwaveformwidget.h"
#include "viloadingwidget.h"
#include "visonginfowidget.h"
#include "vimenu.h"

namespace Ui
{
    class ViMainWindow;
}

class ViMainWindow : public QMainWindow
{
	Q_OBJECT

	public slots:

		void progress(qreal percentage);
		void hideLoading();
		void showLoading(bool animation = true, bool button = true, ViLoadingWidget::TextStyle style = ViLoadingWidget::Percentage, QString text = "");

		void showWave();
		void showSpectrum();

	public:

		~ViMainWindow();
		static ViMainWindow* instance();

	protected:

		ViMainWindow();
		void resizeEvent(QResizeEvent *event);

	private:
		void initialize();

	private:

		static ViMainWindow *mWindow;

		Ui::ViMainWindow *mUi;
		ViAudioEngine *mEngine;
		ViLoadingWidget *mLoadingWidget;

		ViWaveFormWidget *mOriginalWaveWidget;
		ViWaveFormWidget *mCorrectedWaveWidget;

		ViSongInfoWidget *mSongInfoWidget;
};

#endif
