#ifndef VIMAINMETADATAWIDGET_H
#define VIMAINMETADATAWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMainMetadataWidget;
}

class ViMainMetadataWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void changeProject();

	public:

		ViMainMetadataWidget(QWidget *parent = 0);
		~ViMainMetadataWidget();

		void clear();

		void setProject(ViProject *project);

	private:

		Ui::ViMainMetadataWidget *mUi;

};

#endif
