#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include <QSize>
#include <viaudioengine.h>
#include <viid.h>
#include <vifunctioncall.h>
#include <vithememanager.h>

class ViWidget : public QWidget, public ViId
{

	Q_OBJECT

	private slots:

		bool executeFunction();

	public:

		ViWidget(QWidget *parent = 0);
		virtual ~ViWidget(){}
		ViAudioEnginePointer engine();
		virtual void refresh();
		virtual void setStyleSheet(QString styleSheet);
		virtual void addStyleSheet(QString styleSheet);
		virtual QString styleSheet();

		virtual void setBorder(QColor color = Qt::gray, int thickness = 1, int roundness = 0);

		virtual bool addFunctionCall(QString signal, ViFunctionCall functionCall);

		virtual void setSize(int width, int height);
		virtual void setSize(QSize size);
		virtual void setWidth(int width);
		virtual void setHeight(int height);

	protected:

		virtual void paintEvent(QPaintEvent *event);

	protected:

		QWidget *mParent;
		ViAudioEnginePointer mEngine;
		QString mStyleSheet;

		ViFunctionCallSignalMap mFunctionCalls;

};

#endif
