#include <viprojectloader.h>
#include <ui_viprojectloader.h>
#include <viloadingwidget.h>

ViProjectLoader::ViProjectLoader(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectLoader();
	mUi->setupUi(this);

	mTypeMode = ViProjectLoader::MultipleTypes;
	mProjectMode = ViProjectLoader::SingleTrack;

	mUi->fileBrowser->setDirectory(ViManager::projectPath());
	mUi->fileBrowser->addFilter(ViManager::projectFilter());

	QObject::connect(mUi->fileBrowser, SIGNAL(selected()), this, SLOT(loadProjects()));
	QObject::connect(mUi->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProjectMode(int)));
	QObject::connect(mUi->tracksComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(analyseTrack(int)));

	QObject::connect(mUi->targetCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(typesChanged()));
	QObject::connect(mUi->correctedCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(typesChanged()));
	QObject::connect(mUi->corruptedCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(typesChanged()));
	QObject::connect(mUi->targetRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeRadioTypes(bool)));
	QObject::connect(mUi->correctedRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeRadioTypes(bool)));
	QObject::connect(mUi->corruptedRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeRadioTypes(bool)));

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
	if(mSelectedObject >= 0)
	{
		ViAudioObjectQueue queue;
		queue.enqueue(mObjects[mSelectedObject]);
		return queue;
	}
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

	mUi->tracksComboBox->clear();
	mUi->fileBrowser->clear();

	viDeleteAll(mProjects);
	mObjects.clear();

	mUi->tracksLabel->hide();
	mUi->tracksComboBox->hide();

	mUi->typeLabel->hide();
	mUi->multiTypeContainer->hide();
	mUi->singleTypeContainer->hide();

	mUi->targetCheckBox->hide();
	mUi->corruptedCheckBox->hide();
	mUi->correctedCheckBox->hide();


	mUi->targetCheckBox->blockSignals(true);
	mUi->corruptedCheckBox->blockSignals(true);
	mUi->correctedCheckBox->blockSignals(true);
	mUi->targetRadioButton->blockSignals(true);
	mUi->corruptedRadioButton->blockSignals(true);
	mUi->correctedRadioButton->blockSignals(true);

	mUi->targetCheckBox->setChecked(true);
	mUi->corruptedCheckBox->setChecked(true);
	mUi->correctedCheckBox->setChecked(true);
	mUi->targetRadioButton->setChecked(false);
	mUi->corruptedRadioButton->setChecked(false);
	mUi->correctedRadioButton->setChecked(false);

	mUi->targetCheckBox->blockSignals(false);
	mUi->corruptedCheckBox->blockSignals(false);
	mUi->correctedCheckBox->blockSignals(false);
	mUi->targetRadioButton->blockSignals(false);
	mUi->corruptedRadioButton->blockSignals(false);
	mUi->correctedRadioButton->blockSignals(false);
}

void ViProjectLoader::loadProjects()
{
	mObjects.clear();
	mProjectCount = 0;
	mSelectedObject = -1;

	QString message = "Loading project";
	if(mProjectMode == ViProjectLoader::MultipleProjects)
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
		}
	}
	ViLoadingWidget::stop();
	QString message = "The project";
	if(mProjectMode == ViProjectLoader::MultipleProjects)
	{
		message += "s were";
	}
	else
	{
		message += "was";
	}
	LOG(message + " loaded.");
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

	if(mProjectMode == ViProjectLoader::SingleTrack && !mObjects.isEmpty())
	{
		mUi->tracksLabel->show();
		mUi->tracksComboBox->show();
	}

	--mProjectCount;
	if(mProjectCount == 0)
	{
		emit finished();
	}
}

void ViProjectLoader::analyseTrack(int index)
{
	if(index >= 0)
	{
		mSelectedObject = index;
		ViAudio::Type resources = mObjects[index]->availableResources();
		
		mUi->typeLabel->hide();
		mUi->multiTypeContainer->hide();
		mUi->singleTypeContainer->hide();
		mUi->targetCheckBox->hide();
		mUi->corruptedCheckBox->hide();
		mUi->correctedCheckBox->hide();
		mUi->targetRadioButton->hide();
		mUi->corruptedRadioButton->hide();
		mUi->correctedRadioButton->hide();

		if(mTypeMode == ViProjectLoader::MultipleTypes)
		{
			mUi->typeLabel->show();
			mUi->multiTypeContainer->show();

			if(resources & ViAudio::Target) mUi->targetCheckBox->show();
			if(resources & ViAudio::Corrupted) mUi->corruptedCheckBox->show();
			if(resources & ViAudio::Corrected) mUi->correctedCheckBox->show();
		}
		else if(mTypeMode == ViProjectLoader::SingleType)
		{
			mUi->typeLabel->show();
			mUi->singleTypeContainer->show();
			bool selected = false;

			mUi->targetRadioButton->blockSignals(true);
			mUi->corruptedRadioButton->blockSignals(true);
			mUi->correctedRadioButton->blockSignals(true);

			if(resources & ViAudio::Corrected)
			{
				mUi->correctedRadioButton->show();
				mUi->correctedRadioButton->setChecked(true);
				selected = true;
			}
			if(resources & ViAudio::Corrupted)
			{
				mUi->corruptedRadioButton->show();
				if(!selected) mUi->corruptedRadioButton->setChecked(true);
			}
			if(resources & ViAudio::Target)
			{
				mUi->targetRadioButton->show();
				if(!selected) mUi->targetRadioButton->setChecked(true);
			}

			mUi->targetRadioButton->blockSignals(false);
			mUi->corruptedRadioButton->blockSignals(false);
			mUi->correctedRadioButton->blockSignals(false);
		}

		emit trackChanged();
		emit typesChanged();
	}
}

void ViProjectLoader::changeProjectMode(int mode)
{
	mProjectMode = (ViProjectLoader::ProjectMode) mode;
	if(mProjectMode == ViProjectLoader::MultipleProjects)
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
	emit projectModeChanged(); // Must come after clear
}

void ViProjectLoader::changeRadioTypes(bool checked)
{
	if(checked)
	{
		emit typesChanged();
	}
}

ViAudio::Type ViProjectLoader::type()
{
	QQueue<ViAudio::Type> theTypes = ViAudioObject::decomposeTypes(types());
	if(theTypes.isEmpty()) return ViAudio::Undefined;
	return theTypes[0];
}

ViAudio::Type ViProjectLoader::types()
{
	int result = 0;
	if(	(mUi->targetCheckBox->isVisible() && mUi->targetCheckBox->isChecked()) ||
		(mUi->targetRadioButton->isVisible() && mUi->targetRadioButton->isChecked()))
	{
		result |= ViAudio::Target;
	}
	if(	(mUi->corruptedCheckBox->isVisible() && mUi->corruptedCheckBox->isChecked()) ||
		(mUi->corruptedRadioButton->isVisible() && mUi->corruptedRadioButton->isChecked()))
	{
		result |= ViAudio::Corrupted;
	}
	if(	(mUi->correctedCheckBox->isVisible() && mUi->correctedCheckBox->isChecked()) ||
		(mUi->correctedRadioButton->isVisible() && mUi->correctedRadioButton->isChecked()))
	{
		result |= ViAudio::Corrected;
	}
	if(result == 0)
	{
		return ViAudio::Undefined;
	}
	return (ViAudio::Type) result;
}

void ViProjectLoader::setProjectMode(ViProjectLoader::ProjectMode mode)
{
	changeProjectMode(mode);
	mUi->modeLabel->hide();
	mUi->modeComboBox->hide();
}
ViProjectLoader::ProjectMode ViProjectLoader::projectMode()
{
	return mProjectMode;
}

void ViProjectLoader::setTypeMode(ViProjectLoader::TypeMode mode)
{
	mTypeMode = mode;
}

ViProjectLoader::TypeMode ViProjectLoader::typeMode()
{
	return mTypeMode;
}
