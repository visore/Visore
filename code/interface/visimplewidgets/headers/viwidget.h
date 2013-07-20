#ifndef VIWIDGET_H
#define VIWIDGET_H

#include <QWidget>
#include <QSize>
#include <viaudioengine.h>
#include <viid.h>
#include <vifunctioncall.h>
#include <vithememanager.h>
#include <vicommon.h>
#include <viname.h>

class ViWidget : public QWidget, public ViId, public ViName
{

	Q_OBJECT

    signals:

        void shown();
        void hidden();

	private slots:

		bool executeFunction();

	public:

		ViWidget(QWidget *parent = 0);
        virtual ~ViWidget();

		virtual void clear();

		// Will call clear once the widgetis hidden
		void enableClearOnHide(bool enable = true);
		void disableClearOnHide(bool disable = true);

		ViAudioEnginePointer engine();

		virtual void addStyleSheet(QString style);

		virtual bool addFunctionCall(QString signal, ViFunctionCall functionCall);

		virtual void setSize(int widthHeight);
		virtual void setSize(int width, int height);
		virtual void setSize(QSize size);
		virtual void setWidth(int width);
		virtual void setHeight(int height);

	protected:

		virtual void paintEvent(QPaintEvent*);
		virtual void hideEvent(QHideEvent*);

	protected:

		bool mClearOnHide;

		QWidget *mParent;
        ViAudioEnginePointer mEngine;
		QString mStyleSheet;

        ViFunctionCallSignalMap mFunctionCalls;

};

#endif
