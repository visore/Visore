#ifndef VILINEEDIT_H
#define VILINEEDIT_H

#include <QLineEdit>
#include <QGridLayout>
#include "viwidget.h"

class ViLineEdit : public ViWidget
{

	public:

		enum Direction
		{
			Left = 0,
			Right = 1
		};

		ViLineEdit(QWidget *parent = 0);
		void pad(ViLineEdit::Direction direction, int width);
		void setText(QString text);
		QString text();

	private:

		QGridLayout mLayout;
		QLineEdit *mLineEdit;

};

#endif
