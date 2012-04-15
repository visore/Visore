#ifndef VIABSTRACTBUTTON_H
#define VIABSTRACTBUTTON_H

#include <QWidget>

class ViAbstractButton : public QWidget
{
	Q_OBJECT

	signals:
		void clicked(bool checked = false);

	public:

		ViAbstractButton(QWidget *parent = 0);

		virtual void setSize(int width, int height);
		virtual void setCheckable(bool permanentlyChecked = false);
		virtual void setChecked(bool checked);

		virtual void enable();
		virtual void disable();

	protected:

		virtual void enterEvent(QEvent *event);
		virtual void leaveEvent(QEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);

	protected:

		bool mIsHover;
		bool mIsCheckable;
		bool mIsChecked;
		bool mIsPressed;
		bool mIsEnabled;
		bool mIsPermantlyCheckable;

};

#endif
