#ifndef VIWAVEFORMWIDGET_H
#define VIWAVEFORMWIDGET_H

#include <QLabel>
#include <QToolButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QResizeEvent>
#include "viwidgettoolbar.h"
#include "viwavebasewidget.h"
#include "viwaveoverlaywidget.h"

class ViWaveFormWidget : public ViWidget
{
	Q_OBJECT

	signals:
		void pointerMoved(qint32 position);

	public slots:
		void zoom(qint16 levels); //Negative values are zoomOut, otherwise zoomIn
		void zoomIn();
		void zoomOut();
		void setPointer(qint32 position);
		void updateSampleValues(qreal maximum, qreal minimum, qreal maximumAverage, qreal minimumAverage);

	public:
		ViWaveFormWidget(QWidget *parent = 0);
		~ViWaveFormWidget();
		void setDirection(ViAudioConnection::Direction direction);
		void setZoomLevel(qint16 level);

	protected:

		void resizeEvent(QResizeEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:

		ViWaveBaseWidget *mBaseWidget;
		ViWaveOverlayWidget *mOverlayWidget;
		
		qint16 mZoomLevel;

		//Do not delete the subwidget, QLayout will do that

		ViWidgetToolbar *mControlToolbar;
		QToolButton *mZoomInButton;
		QToolButton *mZoomOutButton;

		ViWidgetToolbar *mInfoToolbar;
		QWidget *mInfoWidget;
		QGridLayout *mInfoLayout;
		QLabel *mMaxLabel1;
		QLabel *mMinLabel1;
		QLabel *mMaxAvgLabel1;
		QLabel *mMinAvgLabel1;
		QLabel *mMaxLabel2;
		QLabel *mMinLabel2;
		QLabel *mMaxAvgLabel2;
		QLabel *mMinAvgLabel2;
		
};

#endif
