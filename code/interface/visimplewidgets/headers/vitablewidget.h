#ifndef VITABLEWIDGET_H
#define VITABLEWIDGET_H

#include <QTableWidget>

class ViTableWidget : public QTableWidget
{

	public:

		ViTableWidget(QWidget *parent = 0);

	protected:

		virtual void paintEvent(QPaintEvent *event);

};

#endif
