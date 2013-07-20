#ifndef VIMAINSPECTRUMWIDGET_H
#define VIMAINSPECTRUMWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMainSpectrumWidget;
}

class ViMainSpectrumWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void generate();
		void assign();

	public:

		ViMainSpectrumWidget(QWidget *parent = 0);
		~ViMainSpectrumWidget();

		void clear();

	private:

		Ui::ViMainSpectrumWidget *mUi;

};

#endif
