#ifndef VIMAINWINDOW_H
#define VIMAINWINDOW_H

#include <QMainWindow>

class ViMainWindow : public QMainWindow
{
	public:
		ViMainWindow(QWidget *parent = 0);
};

#ifdef __cplusplus
extern "C" {
#endif

QMainWindow* create(){
   return new ViMainWindow;
}

#ifdef __cplusplus
}
#endif

#endif
