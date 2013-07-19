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

	signals:

		void positionMoved(ViAudioPosition position); // If the slider was manually moved
		void positionChanged(ViAudioPosition position); // If the position was changed programtically
		void durationChanged(ViAudioPosition position); // If the duration was changed programtically

	public slots:

		void changePosition(ViAudioPosition position);
		void changeDuration(ViAudioPosition duration);

	private slots:

		void movePosition();
		void setPosition(int seconds);
		void setDuration(int seconds);

	public:

		ViPositionSlider(QWidget *parent = 0);
		~ViPositionSlider();

		void clear();

	private:

		QString minutesString(int seconds);
		QString secondsString(int seconds);

	private:

		Ui::ViPositionSlider *mUi;

		QMutex mMutex;

		bool mHasPosition;
		bool mHasDuration;
		ViAudioFormat mFormat;

};

#endif
