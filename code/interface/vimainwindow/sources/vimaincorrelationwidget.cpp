#include <vimaincorrelationwidget.h>
#include <ui_vimaincorrelationwidget.h>
#include <vicorrelatormanager.h>

ViMainCorrelationWidget::ViMainCorrelationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainCorrelationWidget();
	mUi->setupUi(this);

	mProject = NULL;
	clear();

	QString style = "QLabel { min-width: 80px; }";
	mUi->projectLoader->setStyleSheet(style);
	mUi->correlatorContainer->setStyleSheet(style);

	mUi->projectLoader->setTypeMode(ViProjectLoader::NoTypes);
	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(correlate()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), this, SLOT(checkCorrelate()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectModeChanged()), this, SLOT(checkCorrelate()));
}

ViMainCorrelationWidget::~ViMainCorrelationWidget()
{
	clear();
	delete mUi;
}

void ViMainCorrelationWidget::clear()
{
	mUi->projectLoader->clear();
	mUi->projectLoader->show();
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
	mUi->correlatorContainer->hide();

	if(mProject != NULL)
	{
		delete mProject;
		mProject = NULL;
	}
	mObjects.clear();
}

void ViMainCorrelationWidget::setProject(ViProject *project)
{
	clear();
	mProject = project;
	mUi->projectLoader->hide();
	changeCorrelation();
}

void ViMainCorrelationWidget::setObjects(ViAudioObjectQueue objects)
{
	clear();
	mObjects = objects;
	mUi->projectLoader->hide();
	changeCorrelation();
}

void ViMainCorrelationWidget::checkCorrelate()
{
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
	mUi->correlatorContainer->hide();
	if(mUi->projectLoader->projectMode() != ViProjectLoader::SingleTrack)
	{
		correlate();
	}
}

void ViMainCorrelationWidget::correlate()
{
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
	mUi->correlatorContainer->hide();
	if(!mUi->projectLoader->objects().isEmpty())
	{
		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(changeCorrelation()));
		engine()->correlate(mUi->projectLoader->objects());
	}
}

void ViMainCorrelationWidget::changeCorrelation()
{
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(changeCorrelation()));
	mUi->scrollArea->show(); // Table must be visible before setting the data, in order to resize properly
	if(mObjects.isEmpty())
	{
		ViProjectLoader::ProjectMode mode = mUi->projectLoader->projectMode();
		if(mode == ViProjectLoader::SingleProject)
		{
			mUi->correlationWidget->setProject(mUi->projectLoader->project());
		}
		else
		{
			mUi->correlationWidget->setObjects(mUi->projectLoader->objects());
		}
	}
	else
	{
		if(mProject == NULL)
		{
			mUi->correlationWidget->setObjects(mObjects);
		}
		else
		{
			mUi->correlationWidget->setProject(mProject);
		}
	}
	updateCorrelators();
}

void ViMainCorrelationWidget::updateCorrelators()
{
	QObject::disconnect(mUi->correlatorComboBox, SIGNAL(currentTextChanged(QString)), mUi->correlationWidget, SLOT(changeCorrelator(QString)));
	mUi->correlatorContainer->show();

	ViAudioObjectQueue objects = mUi->projectLoader->objects();
	QSet<QString> correlators;
	ViCorrelationGroups correlations;
	QStringList keys;
	for(int i = 0; i < objects.size(); ++i)
	{
		correlations = objects[i]->correlations();
		for(int j = 0; j < correlations.size(); ++j)
		{
			keys = correlations[j].correlators();
			for(int k = 0; k < keys.size(); ++k)
			{
				correlators.insert(keys[k]);
			}
		}
	}

	QString correlator;
	 for(int i = 0; i < mUi->correlatorComboBox->count(); ++i)
	 {
		 correlator = mUi->correlatorComboBox->itemText(i);
		 if(correlators.contains(correlator))
		 {
			 correlators.remove(correlator);
		 }
	 }

	 mUi->correlatorComboBox->clear();
	 mUi->correlatorComboBox->addItems(correlators.toList());
	 mUi->correlatorComboBox->setCurrentText(ViCorrelatorManager::defaultName());
	 QObject::connect(mUi->correlatorComboBox, SIGNAL(currentTextChanged(QString)), mUi->correlationWidget, SLOT(changeCorrelator(QString)));
	 mUi->correlationWidget->changeCorrelator(mUi->correlatorComboBox->currentText());
}
