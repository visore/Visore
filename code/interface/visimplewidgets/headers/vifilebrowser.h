#ifndef VIFILEBROWSER_H
#define VIFILEBROWSER_H

#include <QGridLayout>
#include <QFileDialog>
#include "vitoolbutton.h"
#include "vilineedit.h"
#include "vithememanager.h"

class ViFileBrowser : public ViWidget
{
	Q_OBJECT

	private slots:

		void showDialog();

	public:

		enum Mode
		{
			OpenFile = 0,
			SaveFile = 2,
			OpenDirectory = 3
		};

		ViFileBrowser(QWidget *parent = 0);
		~ViFileBrowser();
		void setMode(ViFileBrowser::Mode mode);
		void addFilter(QString filter);
		void setDirectory(QString directory);
		QString fileName();
	
	protected:

		void resizeEvent(QResizeEvent *event);

	private:

		QFileDialog *mDialog;

		QGridLayout mLayout;
		ViLineEdit *mLineEdit;
		ViToolButton *mButton;

};

#endif
