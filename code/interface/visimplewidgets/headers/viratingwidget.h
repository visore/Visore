#ifndef VIRATINGWIDGET_H
#define VIRATINGWIDGET_H

#include "viwidget.h"
#include <QList>
#include <QHBoxLayout>

class ViRatingItem : public ViWidget
{

	public:

		ViRatingItem(QWidget *parent = NULL, bool grayScale = false);
		int imageWidth();
		void setOffset(int offset);
		
	protected:

		void paintEvent(QPaintEvent *event);

	private:

		QImage mImage;
		int mOffset;

};

class ViRatingWidget : public ViWidget
{

	public:

		ViRatingWidget(QWidget *parent = 0);
		~ViRatingWidget();
		void setRating(qint16 percentage);

	private:

		QHBoxLayout *mLayout;
		QSpacerItem *mSpacer;

		QList<ViRatingItem*> mNormalStars;
		QWidget *mNormalWidget;
		QHBoxLayout *mNormalLayout;

		QList<ViRatingItem*> mGrayStars;
		QWidget *mGrayWidget;
		QHBoxLayout *mGrayLayout;

};

#endif
