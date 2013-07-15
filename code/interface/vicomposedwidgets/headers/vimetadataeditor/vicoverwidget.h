#ifndef VICOVERWIDGET_H
#define VICOVERWIDGET_H

#include <viwidget.h>

class ViCoverWidget : public ViWidget
{

	Q_OBJECT

	public:

		ViCoverWidget(QWidget *parent = 0);
		~ViCoverWidget();

		void clear();

		void setImage(QString path);
		void setImage(QImage image);

	protected:

		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent*);
		void leaveEvent(QEvent*);

	private:

		QImage mImage;
		QString mBorderColor1;
		QString mBorderColor2;

};

#endif
