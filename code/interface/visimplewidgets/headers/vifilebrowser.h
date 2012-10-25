#ifndef VIFILEBROWSER_H
#define VIFILEBROWSER_H

#include <QStringList>
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
			OpenFile,
			SaveFile,
			OpenFiles,
			OpenDirectory
		};

		ViFileBrowser(QWidget *parent = 0);
		~ViFileBrowser();
		void setMode(ViFileBrowser::Mode mode);
		void addFilter(QString filter);
		void setDirectory(QString directory);
		QString fileName();
		QStringList fileNames();
		void setFileName(QString fileName);
	
	protected:

		void resizeEvent(QResizeEvent *event);
		QString listToString(QStringList fileNames);
		QStringList stringToList(QString fileNames);

	private:

		QFileDialog *mDialog;

		QGridLayout mLayout;
		ViLineEdit *mLineEdit;
		ViToolButton *mButton;

};

#endif
