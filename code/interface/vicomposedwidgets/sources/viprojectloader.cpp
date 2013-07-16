#include <viprojectloader.h>
#include <ui_viprojectloader.h>
#include <viloadingwidget.h>

ViProjectLoader::ViProjectLoader(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectLoader();
	mUi->setupUi(this);

	mEnableBufferSelection = true;

	mUi->fileBrowser->setDirectory(ViManager::projectPath());
	mUi->fileBrowser->addFilter(ViManager::projectFilter());

	QObject::connect(mUi->fileBrowser, SIGNAL(selected()), this, SLOT(loadProjects()));
	QObject::connect(mUi->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMode(int)));
	QObject::connect(mUi->tracksComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(analyseTrack(int)));

	clear();

	mUi->modeComboBox->setCurrentIndex(2);
}

ViProjectLoader::~ViProjectLoader()
{
	delete mUi;
	clear();
}

ViProjectQueue ViProjectLoader::projects()
{
	return mProjects;
}

ViProject* ViProjectLoader::project()
{
	if(mProjects.isEmpty())
	{
		return NULL;
	}
	return mProjects[0];
}

ViAudioObjectQueue ViProjectLoader::objects()
{
	return mObjects;
}

ViAudioObjectPointer ViProjectLoader::object()
{
	if(mObjects.isEmpty())
	{
		return ViAudioObject::createNull();
	}
	else if(mSelectedObject >= 0)
	{
		return mObjects[mSelectedObject];
	}
	return mObjects[0];
}

void ViProjectLoader::clear()
{
	mSelectedObject = -1;
	mEnableBufferSelection = true;

	mUi->tracksComboBox->clear();
	mUi->fileBrowser->clear();

	viDeleteAll(mProjects);
	mObjects.clear();

	mUi->tracksLabel->hide();
	mUi->tracksComboBox->hide();
	mUi->processLabel->hide();
	mUi->targetCheckBox->hide();
	mUi->corruptedCheckBox->hide();
	mUi->correctedCheckBox->hide();
}

void ViProjectLoader::loadProjects()
{
	mObjects.clear();
	mProjectCount = 0;
	mSelectedObject = -1;

	QString message = "Loading project";
	if(mMode == ViProjectLoader::MultipleProjects)
	{
		message += "s";
	}
	ViLoadingWidget::start(true, false, message, ViProgressBar::Text, ViProgressBar::Infinite);

	QStringList projectPaths = mUi->fileBrowser->fileNames();
	QList<ViProject*> projects;
	for(int i = 0; i < projectPaths.size(); ++i)
	{
		projects.append(new ViProject(projectPaths[i]));
	}
	loadProjects(projects);
}

void ViProjectLoader::loadProjects(QList<ViProject*> projects)
{
	ViProject *project;
	for(int i = 0; i < projects.size(); ++i)
	{
		++mProjectCount;
		project = projects[i];
		QObject::connect(project, SIGNAL(loaded()), this, SLOT(loadTracks()), Qt::UniqueConnection);
		if(!project->load())
		{
			--mProjectCount;
			LOG("The project (" + projects[i]->filePath() + ") could not be loaded.");
			ViLoadingWidget::stop();
		}
	}
	if(projects.size() > 0)
	{
		emit projectChanged();
	}
}

void ViProjectLoader::loadTracks()
{
	ViProject *project = (ViProject*) sender();
	QObject::disconnect(project, SIGNAL(loaded()), this, SLOT(loadTracks()));
	mProjects.enqueue(project);

	mUi->tracksComboBox->clear();

	ViAudioObjectQueue objects = project->objects();
	ViAudio::Type resources;
	for(int i = 0; i < objects.size(); ++i)
	{
		resources = objects[i]->availableResources();
		if(resources != ViAudio::Undefined)
		{
			mObjects.enqueue(objects[i]);
			mUi->tracksComboBox->addItem(objects[i]->fileName(true, true));
		}
		else
		{
			LOG("Track (side " + QString::number(objects[i]->sideNumber()) + ", track " + QString::number(objects[i]->trackNumber()) + ") does not have any data and will be removed from the list.");
		}
	}

	if(mMode == ViProjectLoader::SingleTrack && !mObjects.isEmpty())
	{
		mUi->tracksLabel->show();
		mUi->tracksComboBox->show();
	}

	--mProjectCount;
	if(mProjectCount == 0)
	{
		ViLoadingWidget::stop();
		emit finished();
	}
}

void ViProjectLoader::analyseTrack(int index)
{
	if(index >= 0)
	{
		mSelectedObject = index;
		ViAudio::Type resources = mObjects[index]->availableResources();
		
		if(mEnableBufferSelection)
		{
			mUi->processLabel->show();
			mUi->processContainer->show();
		}
		else
		{
			mUi->processLabel->hide();
			mUi->processContainer->hide();
		}

		if(resources & ViAudio::Target)
		{
			mUi->targetCheckBox->show();
		}
		else
		{
			mUi->targetCheckBox->hide();
		}

		if(resources & ViAudio::Corrupted)
		{
			mUi->corruptedCheckBox->show();
		}
		else
		{
			mUi->corruptedCheckBox->hide();
		}


		if(resources & ViAudio::Corrected)
		{
			mUi->correctedCheckBox->show();
		}
		else
		{
			mUi->correctedCheckBox->hide();
		}

		emit trackChanged();
	}
}

void ViProjectLoader::changeMode(int mode)
{
	mMode = (ViProjectLoader::Mode) mode;

	if(mMode == ViProjectLoader::MultipleProjects)
	{
		mUi->fileLabel->setText("Files:");
		mUi->fileBrowser->setMode(ViFileBrowser::OpenFiles);
	}
	else
	{
		mUi->fileLabel->setText("File:");
		mUi->fileBrowser->setMode(ViFileBrowser::OpenFile);
	}

	clear();
}

ViAudio::Type ViProjectLoader::processTypes()
{
	int result = 0;
	if(mUi->targetCheckBox->isVisible() && mUi->targetCheckBox->isChecked())
	{
		result |= ViAudio::Target;
	}
	if(mUi->corruptedCheckBox->isVisible() && mUi->corruptedCheckBox->isChecked())
	{
		result |= ViAudio::Corrupted;
	}
	if(mUi->correctedCheckBox->isVisible() && mUi->correctedCheckBox->isChecked())
	{
		result |= ViAudio::Corrected;
	}
	if(result == 0)
	{
		return ViAudio::Undefined;
	}
	return (ViAudio::Type) result;
}

void ViProjectLoader::setMode(ViProjectLoader::Mode mode)
{
	changeMode(mode);
	mUi->modeLabel->hide();
	mUi->modeComboBox->hide();
}

void ViProjectLoader::enableBufferSelection(bool enable)
{
	mEnableBufferSelection = enable;
}

void ViProjectLoader::disableBufferSelection(bool disable)
{
	mEnableBufferSelection = !disable;
}
