#ifndef VISCROLLAREA_H
#define VISCROLLAREA_H

#include <QScrollArea>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

class ViScrollArea : public QScrollArea
{

	Q_OBJECT

	public:

		ViScrollArea(QWidget *parent = 0);
        virtual ~ViScrollArea();

};

#endif
