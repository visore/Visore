#ifndef VIWAVECONTROLTOOLBAR_H
#define VIWAVECONTROLTOOLBAR_H

#include <viwidgettoolbar.h>

class ViWaveControlToolbar : public ViWidgetToolbar
{

	Q_OBJECT

	signals:

		void zoomedIn();
		void zoomedOut();
		void toggledGroup();

	public:

		ViWaveControlToolbar(QWidget *parent = 0);
	
};

#endif
