#ifndef VIRATINGWIDGET_H
#define VIRATINGWIDGET_H

#include "viwidget.h"
#include <QList>

namespace Ui
{
    class ViRatingWidget;
}

class ViRatingWidget : public ViWidget
{

	public:
		ViRatingWidget(QWidget *parent = 0);
		~ViRatingWidget();

		void setRating(qint16 percentage);

	private:
		Ui::ViRatingWidget *mUi;
		QList<QWidget*> mStars;

};

#endif
