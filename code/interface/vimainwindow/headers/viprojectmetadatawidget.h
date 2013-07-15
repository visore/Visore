#ifndef VIPROJECTMETADATAWIDGET_H
#define VIPROJECTMETADATAWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViProjectMetadataWidget;
}

class ViProjectMetadataWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void changeProject();

	public:

		ViProjectMetadataWidget(QWidget *parent = 0);
		~ViProjectMetadataWidget();

		void clear();

		void setProject(ViProject *project);

	protected:

		void hideEvent(QHideEvent *event);

	private:

		Ui::ViProjectMetadataWidget *mUi;

};

#endif
