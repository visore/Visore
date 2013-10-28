#ifndef VIMAINNOISEWIDGET_H
#define VIMAINNOISEWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMainNoiseWidget;
}

class ViMainNoiseWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void showContainer();
		void toggleDetector();

		void generate();

	public:

		ViMainNoiseWidget(QWidget *parent = 0);
		~ViMainNoiseWidget();

		void clear();

	private:

		Ui::ViMainNoiseWidget *mUi;
		QString mDefaultDetector;

};

#endif
