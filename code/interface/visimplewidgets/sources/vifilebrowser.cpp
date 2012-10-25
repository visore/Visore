#include "vifilebrowser.h"
#include <QStringList>

ViFileBrowser::ViFileBrowser(QWidget *parent)
	: ViWidget(parent)
{
	mLayout.setContentsMargins(0, 0, 0, 0);
	setLayout(&mLayout);

	mDialog = new QFileDialog(this);
	setDirectory(QDir::homePath());

	mLineEdit = new ViLineEdit(this);
	mLayout.addWidget(mLineEdit);

	mButton = new ViToolButton(mLineEdit);
	int size = mLineEdit->height() * 0.8;
	mButton->setSize(size, size);
	mButton->setIcon(ViThemeManager::image("browse.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);
	mButton->setIcon(ViThemeManager::image("browse.png", ViThemeImage::Selected, ViThemeManager::Icon), ViThemeImage::Hovered);
	mButton->setCursor(Qt::ArrowCursor);
	mButton->setToolTip("Browse");

	QObject::connect(mButton, SIGNAL(clicked()), this, SLOT(showDialog()));
}

ViFileBrowser::~ViFileBrowser()
{
	delete mDialog;
	delete mButton;
	delete mLineEdit;
}

void ViFileBrowser::resizeEvent(QResizeEvent *event)
{
	mButton->move(mLineEdit->width() - mButton->width() - 3, (mLineEdit->height() / 2) - (mButton->height() / 2));
	mLineEdit->pad(ViLineEdit::Right, mButton->width());
	ViWidget::resizeEvent(event);
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
	if(mLineEdit->text() != "")
	{
		setDirectory(mLineEdit->text());
	}
	QStringList fileNames;
	if(mDialog->exec())
	{
		fileNames = mDialog->selectedFiles();
	}
	mLineEdit->setText(listToString(fileNames));
}

void ViFileBrowser::setMode(ViFileBrowser::Mode mode)
{
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
	return stringToList(mLineEdit->text());
}

void ViFileBrowser::setFileName(QString fileName)
{
	mLineEdit->setText(fileName);
}
