#ifndef VILINEEDIT_H
#define VILINEEDIT_H

#include <QLineEdit>
#include <QGridLayout>
#include "viwidget.h"

class ViLineEdit : public QLineEdit
{

	Q_OBJECT

	signals:

        void doubleClicked();

	public:

		ViLineEdit(QWidget *parent = 0);

    protected:

        bool eventFilter(QObject *object, QEvent *event);


};

#endif
