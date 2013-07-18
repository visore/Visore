#ifndef VIWAVEWIDGET_H
#define VIWAVEWIDGET_H

#include <QLabel>
#include <QToolButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QResizeEvent>
#include <QTimer>
#include <viwavecontroltoolbar.h>
#include <viwaveinfotoolbar.h>
#include <viwavebasewidget.h>
#include <viwaveoverlaywidget.h>
#include <viwaveform.h>

class ViWaveWidgetGroup;

class ViWaveWidget : public ViWidget
{

	Q_OBJECT

	signals:

		void pointerChanged(qint32 position);
		void zoomChanged(qint16 level);
		void positionChanged(ViAudioPosition position);

		void toolbarsShown();
		void toolbarsHidden();

	public slots:

		void zoom(qint16 levels); //Negative values are zoomOut, otherwise zoomIn
		void zoomIn();
		void zoomOut();
		void setZoom(qint16 level);

		void setPointer(qint32 position);

		void setPosition(ViAudioPosition position);
		void setPosition(qint64 sample);

		void toggleGroup();
		void connectGroup();
		void disconnectGroup();

		void showToolbars();
		void hideToolbars();

	public:

		ViWaveWidget(QWidget *parent = 0);
		~ViWaveWidget();

		void setWaveForm(ViWaveForm *form, ViAudioFormat format);
		void clear();

		void setGroup(ViWaveWidgetGroup *group);
		ViWaveWidgetGroup* group();
		bool hasGroup();

	protected:

		void resizeEvent(QResizeEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);

	private:

		ViWaveWidgetGroup *mGroup;

		ViWaveBaseWidget *mBaseWidget;
		ViWaveOverlayWidget *mOverlayWidget;

		ViWaveForm *mForm;
		ViAudioFormat mFormat;
		
		qint16 mZoomLevel;

		//Do not delete the subwidget, QLayout will do that

		ViWaveControlToolbar *mControlToolbar;
		ViWaveInfoToolbar *mInfoToolbar;
		QTimer mToolbarTimer;
		
};

#endif
