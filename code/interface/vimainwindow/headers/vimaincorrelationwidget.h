#ifndef VIMAINCORRELATIONWIDGET_H
#define VIMAINCORRELATIONWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMainCorrelationWidget;
}

class ViMainCorrelationWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void checkCorrelate();
		void correlate();
		void changeCorrelation();
		void updateCorrelators();

	public:

		ViMainCorrelationWidget(QWidget *parent = 0);
		~ViMainCorrelationWidget();

		void clear();

		void setProject(ViProject *project); // Takes ownership
		void setObjects(ViAudioObjectQueue objects); // Takes ownership

	private:

		Ui::ViMainCorrelationWidget *mUi;
		ViProject *mProject;
		ViAudioObjectQueue mObjects;

};

#endif
