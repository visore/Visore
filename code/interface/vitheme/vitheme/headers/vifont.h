#ifndef VIFONT_H
#define VIFONT_H

#include <QFont>
#include <QColor>

class ViFont : public QFont
{

	public:

		QString styleSheet();
		void setColor(QColor color);
		QColor color();

	private:

		QColor mColor;

};

#endif
