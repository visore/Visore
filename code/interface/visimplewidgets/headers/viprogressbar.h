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

		ViProgressBar(QWidget *parent = 0);
		~ViProgressBar();

		void setStyleSheet(QString styleSheet);
		void setAlignment(Qt::Alignment alignment);
		void setValue(int value);
		void setText(QString text);
		void setProgressStyle(ViProgressBar::ProgressStyle style);
		
		Qt::Alignment alignment();
		int value();

	private:
		Ui::ViProgressBar *mUi;

};

#endif
