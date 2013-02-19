#ifndef VILINEEDIT_H
#define VILINEEDIT_H

#include <QLineEdit>
#include <QGridLayout>
#include "viwidget.h"

class ViLineEdit : public ViWidget
{

	Q_OBJECT

	signals:

		void textChanged(const QString &text);

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
		void clear();

		virtual void setSize(int width, int height);
		virtual void setSize(QSize size);
		virtual void setWidth(int width);
		virtual void setHeight(int height);

	private:

		QGridLayout mLayout;
		QLineEdit *mLineEdit;

};

#endif
