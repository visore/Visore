#ifndef VIBUTTON_H
#define VIBUTTON_H

#include <QToolButton>
#include <vithemeicon.h>
#include <vifont.h>

class ViButton : public QToolButton
{

	public:

		enum Mode
		{
			Normal,
			Hovered,
			Selected
		};

	public:

		ViButton(QWidget *parent = 0);

		void clearStyleSheet();
		void addStyleSheet(QString style);
		void addStyleSheet(QString style, ViButton::Mode mode);

		void disable();
		void enable();

		void enableBorder();
		void disableBorder();

		void enableBackground();
		void disableBackground();

		void setText(const QString &text);
		void setText(const QString &text, const QColor &color, const QFont &font);
		void setText(const QString &text, const ViFont &font);

		void setFont(const QColor &color, const QFont &font);
		void setFont(const QFont &font);
		void setFont(const ViFont &font);

		void setSize(const int &widthHeight);
		void setSize(const int &width, const int &height);
		void setWidth(const int &width);
		void setHeight(const int &height);

		void setIcon(const ViThemeIcon &icon, const int &size);
		void setIcon(const QIcon &icon, const int &size);

	protected:

		void initialize();

	private:

		QColor mTextColor;
		bool mEnableBorder;
		bool mEnableBackground;

};

#endif
