#include "vifilebrowser.h"
#include <QStringList>

ViFileBrowser::ViFileBrowser(QWidget *parent)
	: ViLineEdit(parent)
{
	mDialog = new QFileDialog(this);
	setDirectory(QDir::homePath());
	mDialog->setStyleSheet("");

	mButton = new ViButton(this);
	int size = height() * 0.8;
	mButton->setSize(size, size);
	mButton->setIcon(ViThemeManager::icon("browse"), 20);
	mButton->setHeight(25);
	mButton->setToolTip("Browse");
	mButton->disableBackground();
	mButton->disbaleBorder();
	mButton->setCursor(Qt::ArrowCursor);

	setStyleSheet(styleSheet() + "ViLineEdit { padding-right: " + QString::number(mButton->width()) + "px; }");

	QObject::connect(mButton, SIGNAL(clicked()), this, SLOT(showDialog()));
	QObject::connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(checkPath()));
	QObject::connect(this, SIGNAL(doubleClicked()), this, SLOT(showDialog()));

	setMode(ViFileBrowser::OpenFile);
}

ViFileBrowser::~ViFileBrowser()
{
	delete mDialog;
	delete mButton;
}

void ViFileBrowser::resizeEvent(QResizeEvent *event)
{
	mButton->move(width() - mButton->width() - 3, (height() / 2) - (mButton->height() / 2));
	ViLineEdit::resizeEvent(event);
}

QString ViFileBrowser::listToString(QStringList fileNames)
{
	QString result = "";
	if(!fileNames.isEmpty())
	{
		result += fileNames[0];
		for(int i = 1; i < fileNames.size(); ++i)
		{
			result += ";" + fileNames[i];
		}
	}
	return result;
}

QStringList ViFileBrowser::stringToList(QString fileNames)
{
	QStringList files = fileNames.split(";");
	files.removeAll("");
	return files;
}

void ViFileBrowser::showDialog()
{
	if(text() != "")
	{
		setDirectory(text());
	}
	QStringList fileNames;
	if(mDialog->exec())
	{
		fileNames = mDialog->selectedFiles();
	}
	setText(listToString(fileNames));
}

void ViFileBrowser::checkPath()
{
	if(mMode == ViFileBrowser::OpenFile)
	{
		QFile file(fileName());
		if(file.exists())
		{
			emit selected();
		}
	}
	else if(mMode == ViFileBrowser::SaveFile)
	{
		QDir dir(fileName());
		if(dir.exists())
		{
			emit selected();
		}
	}
	else if(mMode == ViFileBrowser::OpenFiles)
	{
		QStringList paths = fileNames();
		QStringList newPaths;
		for(int i = 0; i < paths.size(); ++i)
		{
			QFile file(paths[i]);
			if(file.exists())
			{
				newPaths.append(paths[i]);
			}
		}
		setFileNames(newPaths);
		if(!newPaths.isEmpty())
		{
			emit selected();
		}
	}
	else if(mMode == ViFileBrowser::OpenDirectory)
	{
		QDir dir(fileName());
		if(dir.exists())
		{
			emit selected();
		}
	}
}

void ViFileBrowser::setMode(ViFileBrowser::Mode mode)
{
	mMode = mode;
	if(mode == ViFileBrowser::OpenFile)
	{
		mDialog->setAcceptMode(QFileDialog::AcceptOpen);
		mDialog->setFileMode(QFileDialog::ExistingFile);
	}
	else if(mode == ViFileBrowser::SaveFile)
	{
		mDialog->setAcceptMode(QFileDialog::AcceptSave);
		mDialog->setFileMode(QFileDialog::AnyFile);
	}
	else if(mode == ViFileBrowser::OpenFiles)
	{
		mDialog->setAcceptMode(QFileDialog::AcceptOpen);
		mDialog->setFileMode(QFileDialog::ExistingFiles);
	}
	else if(mode == ViFileBrowser::OpenDirectory)
	{
		mDialog->setAcceptMode(QFileDialog::AcceptOpen);
		mDialog->setFileMode(QFileDialog::Directory);
	}
}

void ViFileBrowser::addFilter(QString filter)
{
	QStringList filters = mDialog->nameFilters();
	filters.insert(0, filter);
	mDialog->setNameFilters(filters);
}

void ViFileBrowser::setDirectory(QString directory)
{
	mDialog->setDirectory(directory);
}

QString ViFileBrowser::fileName()
{
	QStringList result = fileNames();
	if(result.isEmpty())
	{
		return "";
	}
	return result[0];
}

QStringList ViFileBrowser::fileNames()
{
	return stringToList(text());
}

void ViFileBrowser::setFileName(QString fileName)
{
	setText(fileName);
}

void ViFileBrowser::setFileNames(QStringList fileNames)
{
	setText(listToString(fileNames));
}
