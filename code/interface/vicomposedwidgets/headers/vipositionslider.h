#ifndef VIPOSITIONSLIDER_H
#define VIPOSITIONSLIDER_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViPositionSlider;
}

class ViPositionSlider : public ViWidget
{

	Q_OBJECT

	public slots:

		void clear();

	private slots:

		void changedPosition(ViAudioPosition position);
		void changedLength(ViAudioPosition length);

	public:

		ViPositionSlider(QWidget *parent = 0);
		~ViPositionSlider();

	private:

		QString minutesString(int seconds);
		QString secondsString(int seconds);
		void setPosition(int seconds);
		void setLength(int seconds);

	private:

		Ui::ViPositionSlider *mUi;
		bool mHasPosition;
		bool mHasLength;

};

#endif
