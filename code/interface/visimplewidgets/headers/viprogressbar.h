#ifndef VIPROGRESSBAR_H
#define VIPROGRESSBAR_H

#include <QProgressBar>
#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViProgressBar;
}

class ViProgressBar : public ViWidget
{

	public:

		enum ProgressStyle
		{
			Finite = 0,
			Infinite = 1
		};

		enum TextStyle
		{
			None = 0,
			Percentage = 1,
			Text = 2
		};

		ViProgressBar(QWidget *parent = 0);
		~ViProgressBar();

		void setStyleSheet(QString styleSheet);
		void setAlignment(Qt::Alignment alignment);
		void setValue(int value);
		void setText(QString text);
		void setProgressStyle(ViProgressBar::ProgressStyle style);
		void setTextStyle(ViProgressBar::TextStyle style);
		
		Qt::Alignment alignment();

		QString text();
		int value();

	private:

		Ui::ViProgressBar *mUi;
		QString mText;

};

#endif
