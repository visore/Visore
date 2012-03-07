#ifndef VIWINDOW_H
#define VIWINDOW_H

#define BORDER_WIDTH 5
#define ICON_SIZE 40

#include <QPainter>
#include <QMainWindow>
#include <QWidget>
#include "vithememanager.h"

namespace Ui
{
    class ViWindow;
}

class ViWindow : public QWidget
{
	Q_OBJECT

	private slots:
		void close();
		void maximize(bool value);
		void minimize();
		void fullscreen(bool value);

	public:
		ViWindow(QMainWindow *parent = 0);
		~ViWindow();
		void setContent(QWidget *widget);

	protected:
		void calculateClipping();
		void resizeEvent(QResizeEvent *event);
		void paintEvent(QPaintEvent *event);

	private:
		Ui::ViWindow *mUi;
		QWidget *mParent;
		QPainterPath mClipper;
		QPoint mOldPosition;
};

#endif
