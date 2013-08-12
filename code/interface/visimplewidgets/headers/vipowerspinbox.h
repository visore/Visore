#ifndef VIPOWERSPINBOX_H
#define VIPOWERSPINBOX_H

#include <QSpinBox>

class ViPowerSpinBox : public QSpinBox
{

	Q_OBJECT

	signals:

		void powerValueChanged();

	private slots:

		void changePowerValue();
		void setPowerValue(int value);

	public:

		ViPowerSpinBox(QWidget *parent = 0);
		virtual ~ViPowerSpinBox();

		void setMinimum(int minimum);
		void setRange(int minimum, int maximum);

	private:

		int mPreviousValue;

};

#endif
