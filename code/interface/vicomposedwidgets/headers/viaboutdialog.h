#ifndef VIABOUTDIALOG_H
#define VIABOUTDIALOG_H

#include <viwidget.h>
#include <QDialog>

namespace Ui
{
	class ViAboutDialog;
}

class ViAboutDialog : public QDialog
{

	Q_OBJECT

	public:

		ViAboutDialog(QWidget *parent = 0);
		~ViAboutDialog();

	private:

		Ui::ViAboutDialog *mUi;

};

#endif
