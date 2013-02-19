#ifndef VIBUTTON_H
#define VIBUTTON_H

#include <QPainter>
#include <QLinearGradient>
#include <QPushButton>
#include <vigradientcreator.h>
#include <vithemeicon.h>
#include <viwidget.h>

class ViButton : public ViWidget
{
	Q_OBJECT

	Q_PROPERTY(bool pressed READ pressed)

	signals:

		void clicked(bool checked = false);

	public:

		ViButton(QWidget *parent = 0);

		void setIcon(QString icon, int size = -1);
		void setIcon(ViThemeIcon icon, int size = -1);
		void setText(QString text);
		void setText(QString text, ViFont font);
		void setText(QString text, QColor color, QFont font);
		void setCornerRounding(qreal value);

		void enableBackground(bool enable = true);
		void disableBackground();

		void setGlow(QColor color, ViGradientCreator::ViGradientType type = ViGradientCreator::Rectangle);
		void setGlow(ViGradientCreator::ViGradientType type);
		void enableGlow(bool enable = true);
		void disableGlow();

		void setEnabled(bool enable = true);
		void enable();
		void disable();

		void setCheckable(bool checkable = true);
		bool isCheckable();
		void setChecked(bool checked = true);
		void check();
		void uncheck();
		bool isChecked();

		bool pressed();

	protected:

		void paintEvent(QPaintEvent *event);
		void enterEvent(QEvent *event);
		void leaveEvent(QEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event);

		void paintGlow(QPainter &painter);
		void paintBackground(QPainter &painter);
		void paintIcon(QPainter &painter);
		void paintText(QPainter &painter);

	private:

		ViThemeIcon mIcon;
		int mIconSize;
		int mIconWidth;
		int mIconHeight;

		QString mText;
		QColor mTextColor;
		QFont mTextFont;

		bool mBackgroundEnabled;

		int mHalfWidth;
		int mHalfHeight;

		bool mIsEnabled;
		bool mIsCheckable;
		bool mIsChecked;

		bool mIsHover;
		bool mIsPressed;

		bool mGlowEnabled;
		int mGlowOffset;
		QColor mGlowColor;
		ViGradientCreator::ViGradientType mGlowType;

		QLinearGradient mNormalGradient;
		QLinearGradient mSelectedGradient;
		QLinearGradient mDisabledGradient;
		
		qreal mCornerRounding;
};

#endif
