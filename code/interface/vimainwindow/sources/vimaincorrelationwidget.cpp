#include <vimaincorrelationwidget.h>
#include <ui_vimaincorrelationwidget.h>

ViMainCorrelationWidget::ViMainCorrelationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainCorrelationWidget();
	mUi->setupUi(this);

	clear();

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
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
}

void ViMainCorrelationWidget::checkCorrelate()
{
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
	if(mUi->projectLoader->projectMode() != ViProjectLoader::SingleTrack)
	{
		correlate();
	}
}

void ViMainCorrelationWidget::correlate()
{
	mUi->correlationWidget->clear();
	mUi->scrollArea->hide();
	if(!mUi->projectLoader->objects().isEmpty())
	{
		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(changeCorrelation()));
		engine()->correlate(mUi->projectLoader->objects());
	}
}

void ViMainCorrelationWidget::changeCorrelation()
{
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(changeCorrelation()));

	ViProjectLoader::ProjectMode mode = mUi->projectLoader->projectMode();
	if(mode == ViProjectLoader::SingleProject)
	{
		mUi->correlationWidget->setProject(mUi->projectLoader->project());
	}
	else
	{
		mUi->correlationWidget->setObjects(mUi->projectLoader->objects());
	}
	LOG("wwwwwwwww: "+QString::number(mUi->projectLoader->objects().size()));
	mUi->scrollArea->show();
}
