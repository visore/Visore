#ifndef VIINFOLABEL_H
#define VIINFOLABEL_H

#include <QLabel>
#include <QHBoxLayout>
#include "viwidget.h"

class ViInfoLabel : public ViWidget
{

	Q_OBJECT

	public slots:

		void setText(QString description, QString text);
		void setText(QString text);
		void setDescriptor(QString description);

	public:

		ViInfoLabel(QWidget *parent, QString description, QString text);
		ViInfoLabel(QWidget *parent, QString text);
		ViInfoLabel(QString description, QString text);
		ViInfoLabel(QString text);
		virtual ~ViInfoLabel();
		
		int textWidth();
		int descriptorWidth();
		void setTextWidth(int width);
		void setDescriptorWidth(int width);
		bool hasDescriptor();

	protected:

		QHBoxLayout *mLayout;
		QLabel *mTextLabel;
		QLabel *mDescriptorLabel;
		
};

#endif
