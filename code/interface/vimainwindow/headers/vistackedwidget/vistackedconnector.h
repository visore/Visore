#ifndef VISTACKEDCONNECTOR_H
#define VISTACKEDCONNECTOR_H

#include <viwidget.h>

class ViStackedConnector : public QObject
{

	Q_OBJECT

	private slots:

		void access();

	public:

		ViStackedConnector();
		void setConnection(ViWidget *widget, char* signal);		

	private:

		int mIndex;

};

#endif
