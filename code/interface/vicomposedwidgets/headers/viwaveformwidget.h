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
		void zoomIn();
		void zoomOut();
		void setPointer(qint32 position);

	public:
		ViWaveFormWidget(ViAudioEngine *engine, ViAudioBuffer::ViAudioBufferType type, QWidget *parent = 0);
		~ViWaveFormWidget();
		void setZoomLevel(qint16 level);

	protected:
		void resizeEvent(QResizeEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:
		ViWaveBaseWidget *mBaseWidget;
		ViWaveOverlayWidget *mOverlayWidget;
		
		qint16 mZoomLevel;

		ViWidgetToolbar *mControlToolbar;
		QToolButton mZoomInButton;
		QToolButton mZoomOutButton;

		ViWidgetToolbar *mInfoToolbar;
		QWidget mInfoWidget;
		QGridLayout mInfoLayout;
		QTextEdit mMaxLabel1;
		QLabel mMinLabel1;
		QLabel mMaxAvgLabel1;
		QLabel mMinAvgLabel1;
		QLabel mMaxLabel2;
		QLabel mMinLabel2;
		QLabel mMaxAvgLabel2;
		QLabel mMinAvgLabel2;
		
};

#endif
