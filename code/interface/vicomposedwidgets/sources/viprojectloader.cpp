#include <viprojectloader.h>
#include <ui_viprojectloader.h>
#include <viloadingwidget.h>

ViProjectLoader::ViProjectLoader(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectLoader();
	mUi->setupUi(this);
	
	setContentsMargins(0, 0, 0, 0);
	mProject = NULL;

	mUi->fileBrowser->setMode(ViFileBrowser::OpenFile);
	mUi->fileBrowser->setDirectory(ViManager::projectPath());
	mUi->fileBrowser->addFilter(ViManager::projectFilter());

	mUi->button->setSize(150, 64);
	mUi->button->setIcon(ViThemeManager::icon("startprocess"), 36);
	mUi->button->setText("Process");

	QObject::connect(mUi->fileBrowser, SIGNAL(selected()), this, SLOT(loadProject()));
	QObject::connect(mUi->tracksComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(analyseTrack(int)));
	QObject::connect(mUi->button, SIGNAL(clicked()), this, SLOT(checkStart()));

	clear();
}

ViProjectLoader::~ViProjectLoader()
{
	delete mUi;
	clear();
}

ViProject* ViProjectLoader::project()
{
	return mProject;
}

ViAudioObjectPointer ViProjectLoader::currentObject()
{
	return mObject;
}

void ViProjectLoader::clear()
{
	mUi->tracksComboBox->clear();
	mUi->fileBrowser->clear();
	mObject.setNull();
	mObjects.clear();

	if(mProject != NULL)
	{
		emit projectChanged();
		delete mProject;
		mProject = NULL;
	}

	mUi->tracksLabel->hide();
	mUi->tracksComboBox->hide();
	mUi->processLabel->hide();
	mUi->targetCheckBox->hide();
	mUi->corruptedCheckBox->hide();
	mUi->correctedCheckBox->hide();
	mUi->button->hide();
}

void ViProjectLoader::loadProject()
{
	ViLoadingWidget::start(true, false, "Loading project", ViProgressBar::Text, ViProgressBar::Infinite);
	if(mProject != NULL)
	{
		emit projectChanged();
		delete mProject;
	}
	mUi->tracksComboBox->clear();
	mProject = new ViProject(mUi->fileBrowser->fileName());
	QObject::connect(mProject, SIGNAL(loaded()), this, SLOT(loadTracks()), Qt::UniqueConnection);
	if(!mProject->load())
	{
		LOG("Project (" + mUi->fileBrowser->fileName() + ") could not be loaded.");
	}
}

void ViProjectLoader::loadTracks()
{
	emit opened();
	mObjects.clear();
	mObject.setNull();
	ViAudioObjectMatrix matrix = mProject->objectMatrix();
	ViAudioObject::Type resources;
	for(int i = 0; i < matrix.size(); ++i)
	{
		for(int j = 0; j < matrix[i].size(); ++j)
		{
			resources = matrix[i][j]->availableResources();
			if(resources != ViAudioObject::Undefined)
			{
				mObjects.append(matrix[i][j]);
				mUi->tracksComboBox->addItem(ViProject::generateTrackName(matrix[i][j]->songInfo(), j+1, i+1));
			}
			else
			{
				LOG("Track (side " + QString::number(i+1) + ", track " + QString::number(j+1) + ") does not have any data and will be removed from the list.");
			}
		}
	}
	if(!mObjects.isEmpty())
	{
		mUi->tracksLabel->show();
		mUi->tracksComboBox->show();
	}
	ViLoadingWidget::stop();
}

void ViProjectLoader::analyseTrack(int index)
{
	if(index >= 0)
	{
		mObject = mObjects[index];
		ViAudioObject::Type resources = mObject->availableResources();
		mUi->processLabel->show();

		if(resources & ViAudioObject::Target)
		{
			mUi->targetCheckBox->show();
		}
		else
		{
			mUi->targetCheckBox->hide();
		}

		if(resources & ViAudioObject::Corrupted)
		{
			mUi->corruptedCheckBox->show();
		}
		else
		{
			mUi->corruptedCheckBox->hide();
		}


		if(resources & ViAudioObject::Corrected)
		{
			mUi->correctedCheckBox->show();
		}
		else
		{
			mUi->correctedCheckBox->hide();
		}

		mUi->button->show();
		emit trackChanged(mObject);
	}
}

void ViProjectLoader::checkStart()
{
	if(processTypes() != ViAudioObject::Undefined)
	{
		emit started();
	}
}

ViAudioObject::Type ViProjectLoader::processTypes()
{
	int result = 0;
	if(mUi->targetCheckBox->isVisible() && mUi->targetCheckBox->isChecked())
	{
		result |= ViAudioObject::Target;
	}
	if(mUi->corruptedCheckBox->isVisible() && mUi->corruptedCheckBox->isChecked())
	{
		result |= ViAudioObject::Corrupted;
	}
	if(mUi->correctedCheckBox->isVisible() && mUi->correctedCheckBox->isChecked())
	{
		result |= ViAudioObject::Corrected;
	}
	if(result == 0)
	{
		return ViAudioObject::Undefined;
	}
	return (ViAudioObject::Type) result;
}
