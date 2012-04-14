#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "viwaveformwidget.h"
#include "vicontrolcontainer.h"
#include "viinputcontainer.h"
#include "viloadingwidget.h"

namespace Ui
{
    class ViMainWindow;
}

class ViMainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		ViMainWindow(ViAudioEngine *engine, QWidget *parent = 0);
		~ViMainWindow();
		void setLoading(bool load);

	protected:
		void resizeEvent(QResizeEvent *event);

	private:
		void initialize();

	private:
		Ui::ViMainWindow *mUi;
		ViAudioEngine *mEngine;
		ViLoadingWidget *mLoadingWidget;

		ViWaveFormWidget *mOriginalWaveWidget;
		ViWaveFormWidget *mCorrectedWaveWidget;

		ViControlContainer *mControlContainer;
		ViInputContainer *mInputContainer;
};

#endif
