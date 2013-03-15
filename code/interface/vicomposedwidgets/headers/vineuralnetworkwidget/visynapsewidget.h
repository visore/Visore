#ifndef VISYNAPSEWIDGET_H
#define VISYNAPSEWIDGET_H

#include <vineuronwidget.h>
#include <visynapse.h>

class ViSynapseWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void repaint();

	public:

		ViSynapseWidget(ViSynapse *synapse, ViWidget *parent = NULL);
		ViSynapseWidget(ViNeuronWidget *inputWidget, ViNeuronWidget *outputWidget, ViSynapse *synapse, ViWidget *parent = NULL);
		
		void setSynapse(ViSynapse *synapse);
		ViSynapse* synapse();

		void setWidgets(ViNeuronWidget *inputWidget, ViNeuronWidget *outputWidget);
		void setInputWidget(ViNeuronWidget *widget);
		void setOutputWidget(ViNeuronWidget *widget);

		ViNeuronWidget* inputWidget();
		ViNeuronWidget* outputWidget();

		void setThickness(int thickness);
		int thickness();

	protected:

		void initailizeColors();

		virtual void paintEvent(QPaintEvent *event);

	private:

		int mThickness;
		QColor mColor;
		QColor mTextColor;

		int mFromX;
		int mFromY;
		int mToX;
		int mToY;

		ViSynapse *mSynapse;

		ViNeuronWidget *mInputWidget;
		ViNeuronWidget *mOutputWidget;

};

#endif
