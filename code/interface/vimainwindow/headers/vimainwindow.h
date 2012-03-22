#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include "viwaveformwidget.h"
#include "vicontrolcontainer.h"
#include "viinputcontainer.h"

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

	private:
		void initialize();

	private:
		Ui::ViMainWindow *mUi;
		ViAudioEngine *mEngine;

		ViWaveFormWidget *mOriginalWaveWidget;
		ViWaveFormWidget *mCorrectedWaveWidget;

		ViControlContainer *mControlContainer;
		ViInputContainer *mInputContainer;
};

#endif
