#ifndef VIWAVEINFOTOOLBAR_H
#define VIWAVEINFOTOOLBAR_H

#include <viwidgettoolbar.h>
#include <QLabel>
#include <QGridLayout>

class ViWaveInfoToolbar : public ViWidgetToolbar
{

	Q_OBJECT

	public slots:

		void setValues(qreal maximum, qreal minimum, qreal maximumAverage, qreal minimumAverage);

	public:

		ViWaveInfoToolbar(QWidget *parent = 0);
		~ViWaveInfoToolbar();
		void clear();

	private:

		QWidget *mWidget;
		QGridLayout *mLayout;
		QLabel *mMaxLabel;
		QLabel *mMinLabel;
		QLabel *mMaxAvgLabel;
		QLabel *mMinAvgLabel;
		
};

#endif
