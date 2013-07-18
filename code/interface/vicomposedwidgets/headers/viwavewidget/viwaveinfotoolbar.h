#ifndef VIWAVEINFOTOOLBAR_H
#define VIWAVEINFOTOOLBAR_H

#include <viwidgettoolbar.h>
#include <QLabel>
#include <QGridLayout>

class ViWaveInfoToolbar : public ViWidgetToolbar
{

	Q_OBJECT

	public slots:

		void setValues(qreal maximum, qreal minimum, qreal average);
		void setPosition(ViAudioPosition position);

	public:

		ViWaveInfoToolbar(QWidget *parent = 0);
		~ViWaveInfoToolbar();
		void clear();

	private:

		QWidget *mWidget;
		QGridLayout *mLayout;
		QLabel *mSampleLabel;
		QLabel *mSecondLabel;
		QLabel *mMaximumLabel;
		QLabel *mMinimumLabel;
		QLabel *mAverageLabel;
		bool mValid;
		
};

#endif
