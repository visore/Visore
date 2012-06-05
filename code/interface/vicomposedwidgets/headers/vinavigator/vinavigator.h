#ifndef VINAVIGATOR_H
#define VINAVIGATOR_H

#include "vitabwidget.h"
#include "vimenu.h"

namespace Ui
{
    class ViNavigator;
}

class ViNavigator : public ViWidget
{
	Q_OBJECT

	public slots:

		void changeStackIndex(int tabIndex);

	public:

		ViNavigator(QWidget *parent = 0);
		~ViNavigator();
		void setStackIndex(int index);
		int addStackWidget(ViWidget *widget);

	protected:

		void initialize();
		void addMenu(QString name, ViMenu *menu);

	private:

		Ui::ViNavigator *mUi;
		QList<ViMenu*> mMenus;

};

#endif
