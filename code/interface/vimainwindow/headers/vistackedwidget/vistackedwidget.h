#ifndef VISTACKEDWIDGET_H
#define VISTACKEDWIDGET_H

#include <viwidget.h>
#include <visingleton.h>
#include <QStackedWidget>

class ViStackedWidget : public QObject, public ViSingleton<ViStackedWidget>
{

	Q_OBJECT

	friend class ViSingleton<ViStackedWidget>;

    private slots:

        void deleteTemporaryWidget();

	public:

		~ViStackedWidget();

        static void showTemporaryWidget(ViWidget *widget, bool scroll = true);
        static int addWidget(ViWidget *widget, bool scroll = true);
		static QStackedWidget* widget();
		static void setCurrentWidget(ViWidget *widget);
		Q_INVOKABLE static void setCurrentIndex(int index);
		static ViFunctionCall currentIndexCall(int index);

	protected:

		ViStackedWidget();

	private:

		QStackedWidget *mWidget;
        int mTemporaryIndex;

};

#endif
