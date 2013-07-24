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
	QObject::connect(mUi->projectLoader, SIGNAL(finished()), this, SLOT(correlate()));
	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(hideCorrelation()));
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

void ViMainCorrelationWidget::setData(ViProject *project)
{
	clear();
	mProject = project;
	mUi->projectLoader->hide();
	if(!mProject->isFinished())
	{
		QObject::connect(mProject, SIGNAL(finished()), this, SLOT(changeCorrelation()));
	}
	else
	{
		changeCorrelation();
	}
}

void ViMainCorrelationWidget::setData(ViProject *project, ViAudioObjectQueue objects)
{
	clear();
	mProject = project;
	mObjects = objects;
	mUi->projectLoader->hide();
	if(!mProject->isFinished())
	{
		QObject::connect(mProject, SIGNAL(finished()), this, SLOT(changeCorrelation()));
	}
	else
	{
		changeCorrelation();
	}
}

void ViMainCorrelationWidget::setData(ViAudioObjectQueue objects)
{
	clear();
	mObjects = objects;
	mUi->projectLoader->hide();
	changeCorrelation();
}

void ViMainCorrelationWidget::hideCorrelation()
{
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
	mUi->correlatorContainer->hide();
}

void ViMainCorrelationWidget::correlate()
{
	hideCorrelation();
	if(!mUi->projectLoader->objects().isEmpty())
	{
		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(changeCorrelation()));
		engine()->correlate(mUi->projectLoader->objects());
	}
}

void ViMainCorrelationWidget::changeCorrelation()
{
	if(mProject != NULL) QObject::disconnect(mProject, SIGNAL(finished()), this, SLOT(changeCorrelation()));
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(changeCorrelation()));
	if(mObjects.isEmpty())
	{
		mUi->correlationWidget->setData(mUi->projectLoader->project(), mUi->projectLoader->objects());
	}
	else if(mProject == NULL)
	{
		mUi->correlationWidget->setData(mObjects);
	}
	else
	{
		mUi->correlationWidget->setData(mProject, mObjects);
	}
	updateCorrelators();
}

void ViMainCorrelationWidget::updateCorrelators()
{
	QObject::disconnect(mUi->correlatorComboBox, SIGNAL(currentTextChanged(QString)), mUi->correlationWidget, SLOT(changeCorrelator(QString)));

	mUi->scrollArea->show(); // Table must be visible before setting the data, in order to resize properly
	mUi->correlatorContainer->show();

	ViAudioObjectQueue objects = mObjects;
	if(objects.isEmpty()) objects = mUi->projectLoader->objects();

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
				correlators.insert(ViName::formatName(keys[k], "", true));
			}
		}
	}

	mUi->correlatorComboBox->clear();
	mUi->correlatorComboBox->addItems(correlators.toList());
	mUi->correlatorComboBox->setCurrentText(ViCorrelatorManager::defaultName());
	QObject::connect(mUi->correlatorComboBox, SIGNAL(currentTextChanged(QString)), mUi->correlationWidget, SLOT(changeCorrelator(QString)));
	mUi->correlationWidget->changeCorrelator(mUi->correlatorComboBox->currentText());

}
