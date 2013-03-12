#ifndef VINEURONWIDGET_H
#define VINEURONWIDGET_H

#include <viwidget.h>

class ViNeuronWidget : public ViWidget
{

	public:

		ViNeuronWidget(ViWidget *parent = NULL);
		ViNeuronWidget(int x, int y, ViWidget *parent = NULL);
		ViNeuronWidget(int x, int y, int radius, ViWidget *parent = NULL);
		
		void setPosition(int x, int y);

		void setX(int x);
		int x();

		void setY(int y);
		int y();

		void setRadius(int radius);
		int radius();

	protected:

		void initailizeColors();

		void paintEvent(QPaintEvent *event);

	private:

		int mX;
		int mY;
		int mRadius;

		QColor mBorderColor;
		QColor mOuterColor;
		QColor mInnerColor;

};

#endif
