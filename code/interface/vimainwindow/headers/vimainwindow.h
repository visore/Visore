#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include "viaudioengine.h"

class ViMainWindow : public QMainWindow
{
	Q_OBJECT

	private slots:
		void changeOutputPosition();

	public:
		ViMainWindow(QWidget *parent = 0);
		void setEngine(ViAudioEngine *engine);

	private:
		ViAudioEngine *mEngine;
		QBoxLayout *mLayout;
		QPushButton *m1;
		QPushButton *m2;
		QPushButton *m3;
		QPushButton *m4;
		QPushButton *m5;
		QPushButton *m6;
		QPushButton *m7;
		QWidget *mWidget;
};

#endif
