#ifndef VIANIMATIONWIDGET_H
#define VIANIMATIONWIDGET_H

#include <viwidget.h>
#include <vithemeanimation.h>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>

class ViAnimationWidget : public ViWidget
{

	Q_OBJECT

	public slots:

		void start(int interval = 500);
		void stop();

	private slots:

		void next();

	public:

		ViAnimationWidget(QWidget *parent = 0);
		~ViAnimationWidget();
		void setAnimation(ViThemeAnimation animation, int size = -1);

	private:

		QGridLayout *mLayout;
		QLabel *mLabel;
		QTimer mTimer;
		ViPixmapList mPixmaps;
		int mCurrentPixmap;

};

#endif
