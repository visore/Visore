#ifndef VINEURONLINEWIDGET_H
#define VINEURONLINEWIDGET_H

#include <vineuronwidget.h>

class ViNeuronLineWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void repainted();

	public slots:

		void repaint();

	public:

		ViNeuronLineWidget(ViWidget *parent = NULL);
		ViNeuronLineWidget(ViNeuronWidget *neuronWidget, ViWidget *parent = NULL);
		ViNeuronLineWidget(ViNeuronWidget *neuronWidget, int length, int thickness, ViWidget *parent = NULL);
		
		void setLength(int length);
		int length();

		void setThickness(int thickness);
		int thickness();

		void setNeuronWidget(ViNeuronWidget *neuronWidget);
		ViNeuronWidget* neuronWidget();

	protected:

		void initailizeColors();

		void paintEvent(QPaintEvent *event);

	private:

		int mLength;
		int mThickness;
		QColor mColor;
		ViNeuronWidget *mNeuronWidget;

};

#endif
