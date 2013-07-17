#include <vitrackmetadataeditor.h>
#include <ui_vitrackmetadataeditor.h>
#include <QFileDialog>
#include <QImageReader>

#define COVER_WIDTH 200
#define COVER_HEIGHT 200

ViTrackMetadataEditor::ViTrackMetadataEditor(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViTrackMetadataEditor();
	mUi->setupUi(this);

	QObject::connect(mUi->titleEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setTitle(const QString&)));
	QObject::connect(mUi->artistEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setArtist(const QString&)));
	QObject::connect(mUi->albumEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setAlbum(const QString&)));

	QObject::connect(mUi->previousButton, SIGNAL(clicked()), this, SLOT(previousImage()));
	mUi->previousButton->disableBackground();
	mUi->previousButton->disableBorder();
	mUi->previousButton->setIcon(ViThemeManager::icon("left"), 20);

	QObject::connect(mUi->nextButton, SIGNAL(clicked()), this, SLOT(nextImage()));
	mUi->nextButton->disableBackground();
	mUi->nextButton->disableBorder();
	mUi->nextButton->setIcon(ViThemeManager::icon("right"), 20);

	QObject::connect(mUi->browseButton, SIGNAL(clicked()), this, SLOT(browseImage()));
	mUi->browseButton->disableBackground();
	mUi->browseButton->disableBorder();
	mUi->browseButton->setIcon(ViThemeManager::icon("search"), 20);

	mUi->image->setSize(COVER_WIDTH, COVER_HEIGHT);
	QColor color = ViThemeManager::color(ViThemeColors::BorderColor3);
	mUi->imageBar->setStyleSheet(".ViWidget { border: 2px solid " + color.name() + "; border-radius: 4px; background: rgba(" + QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()) + ", 100); }");

	clear();
}

ViTrackMetadataEditor::~ViTrackMetadataEditor()
{
	clear();
	delete mUi;
}

void ViTrackMetadataEditor::clear()
{
	for(int i = 0; i < mImages.size(); ++i)
	{
		QFile::remove(mImages[i]);
	}
	mImages.clear();

	mMetadata = NULL;
	mCurrentImage = "";
	mCurrentIndex = 0;

	changeImage();

	mUi->titleEdit->clear();
	mUi->artistEdit->clear();
	mUi->albumEdit->clear();
}

void ViTrackMetadataEditor::setMetadata(ViMetadata *metadata)
{
	mMetadata = metadata;

	if(mMetadata->hasTitle()) setTitle(mMetadata->title());
	else setTitle("");

	if(mMetadata->hasArtist()) setArtist(mMetadata->artist());
	else setArtist("");

	if(mMetadata->hasAlbum()) setAlbum(mMetadata->album());
	else setAlbum("");

	mCurrentImage = addPossibleImage(mMetadata->cover());
	for(int i = 0; i < mImages.size(); ++i)
	{
		if(mCurrentImage == mImages[i]) mCurrentIndex = i;
	}
	changeImage();
}

bool ViTrackMetadataEditor::hasMetadata()
{
	return mMetadata != NULL;
}

void ViTrackMetadataEditor::setTitle(const QString &title)
{
	mUi->titleEdit->setText(title);
	if(hasMetadata())
	{
		mMetadata->setTitle(title);
		emit changed();
	}
}

void ViTrackMetadataEditor::setArtist(const QString &artist)
{
	mUi->artistEdit->setText(artist);
	if(hasMetadata())
	{
		mMetadata->setArtist(artist);
		emit changed();
	}
}

void ViTrackMetadataEditor::setAlbum(const QString &album)
{
	mUi->albumEdit->setText(album);
	if(hasMetadata())
	{
		mMetadata->setAlbum(album);
		emit changed();
	}
}

QString ViTrackMetadataEditor::addPossibleImage(QString path)
{
	QString exisitingPath = "";
	if(path != "" && !hasImage(path, exisitingPath))
	{
		exisitingPath = ViManager::tempCoverPath() + ViId::generate() + "." + QString(QImageReader::imageFormat(path));
		QFile::copy(path, exisitingPath);
		mImages.append(exisitingPath);
	}
	return exisitingPath;
}

void ViTrackMetadataEditor::setPossibleImages(QList<QString> images)
{
	for(int i = 0; i < images.size(); ++i)
	{
		addPossibleImage(images[i]);
	}
}

void ViTrackMetadataEditor::setPossibleImages(QList<ViMetadata> metadatas)
{
	for(int i = 0; i < metadatas.size(); ++i)
	{
		if(metadatas[i].hasCover())
		{
			addPossibleImage(metadatas[i].cover());
		}
	}
}

bool ViTrackMetadataEditor::hasImage(QString path)
{
	QString temp;
	return ViTrackMetadataEditor::hasImage(path, temp);
}

bool ViTrackMetadataEditor::hasImage(QString path, QString &existingPath)
{
	for(int i = 0; i < mImages.size(); ++i)
	{
		if(QImage(path) == QImage(mImages[i]))
		{
			existingPath = mImages[i];
			return true;
		}
	}
	existingPath = "";
	return false;
}

bool ViTrackMetadataEditor::previousImage()
{
	int newIndex = mCurrentIndex - 1;
	if(newIndex >= 0)
	{
		mCurrentImage = mImages[newIndex];
		mCurrentIndex = newIndex;
		changeImage();
		return true;
	}
	return false;
}

bool ViTrackMetadataEditor::nextImage()
{
	int newIndex = mCurrentIndex + 1;
	if(newIndex < mImages.size())
	{
		mCurrentImage = mImages[newIndex];
		mCurrentIndex = newIndex;
		changeImage();
		return true;
	}
	return false;
}

void ViTrackMetadataEditor::browseImage()
{
	QString image = QFileDialog::getOpenFileName(this, "Cover Image", QDir::homePath(), "Images (*bmp *.png *.gif *.jpg)");
	if(image != "")
	{
		mCurrentImage = addPossibleImage(image);
		for(int i = 0; i < mImages.size(); ++i)
		{
			if(mCurrentImage == mImages[i]) mCurrentIndex = i;
		}
		changeImage();
	}
}

void ViTrackMetadataEditor::changeImage()
{
	mUi->image->setImage(mCurrentImage);
	if(hasMetadata())
	{
		QString path = ViManager::tempCoverPath() + ViId::generate() + "." + QString(QImageReader::imageFormat(mCurrentImage));
		QFile::copy(mCurrentImage, path);
		mMetadata->setCover(path);
		emit changed();
	}
}
