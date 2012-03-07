#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include "viwaveformwidget.h"
#include "vicontrolwidget.h"
#include "viinputwidget.h"

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

		ViControlWidget *mControlWidget;
		ViInputWidget *mInputWidget;
};

#endif
