#ifndef VINEURONWIDGET_H
#define VINEURONWIDGET_H

#include <viwidget.h>
#include <vineuron.h>
#include <QPoint>

class ViNeuronWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void repainted();

	public slots:

		void repaint();

	public:

		ViNeuronWidget(ViNeuron *neuron, ViWidget *parent = NULL);
		ViNeuronWidget(int x, int y, ViNeuron *neuron, ViWidget *parent = NULL);
		ViNeuronWidget(int x, int y, int radius, ViNeuron *neuron, ViWidget *parent = NULL);
		
		void setNeuron(ViNeuron *neuron);
		ViNeuron* neuron();

		void setPosition(int x, int y);
		void move(int x, int y);

		void setX(int x);
		int x();

		void setY(int y);
		int y();

		QPoint center();

		void setRadius(int radius);
		int radius();

	protected:

		virtual void initailizeColors();

		void paintEvent(QPaintEvent *event);

	private:

		ViNeuron *mNeuron;

		int mX;
		int mY;
		int mRadius;

	protected:

		QColor mBorderColor;
		QColor mOuterColor;
		QColor mInnerColor;
		QColor mTextColor;

};

#endif
