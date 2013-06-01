#include <viwaveanalysiswidget.h>
#include <ui_viwaveanalysiswidget.h>

ViWaveAnalysisWidget::ViWaveAnalysisWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViWaveAnalysisWidget();
	mUi->setupUi(this);

	mUi->projectLoader->setMode(ViProjectLoader::SingleTrack);

	QObject::connect(mUi->projectLoader, SIGNAL(trackChanged()), this, SLOT(generateWave()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), this, SLOT(clear()), Qt::DirectConnection);

	mUi->targetContainer->hide();
	mUi->corruptedContainer->hide();
	mUi->correctedContainer->hide();
}

ViWaveAnalysisWidget::~ViWaveAnalysisWidget()
{
	delete mUi;
	clear();
	mUi->projectLoader->clear();
}

void ViWaveAnalysisWidget::generateWave()
{
	mUi->targetWaveForm->clear();
	mUi->corruptedWaveForm->clear();
	mUi->correctedWaveForm->clear();
	QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(drawWave()));
	engine()->generateWaveForm(mUi->projectLoader->object(), mUi->projectLoader->processTypes());
}

void ViWaveAnalysisWidget::drawWave()
{
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(drawWave()));
	mUi->expander->show();
	ViAudioObject::Type types = mUi->projectLoader->processTypes();
	ViWaveForm *form;

	ViAudioObjectPointer object = mUi->projectLoader->object();

	form = object->waveForm(ViAudioObject::Target);
	if(form != NULL && !form->isEmpty() && (ViAudioObject::Target & types))
	{
		mUi->targetContainer->show();
		mUi->targetWaveForm->setWaveForm(form, object->targetFormat());
		mUi->expander->hide();
		mWaveGroup.addWidget(mUi->targetWaveForm);
	}
	else
	{
		mUi->targetContainer->hide();
		mWaveGroup.removeWidget(mUi->targetWaveForm);
	}

	form = object->waveForm(ViAudioObject::Corrupted);
	if(form != NULL && !form->isEmpty() && (ViAudioObject::Corrupted & types))
	{
		mUi->corruptedContainer->show();
		mUi->corruptedWaveForm->setWaveForm(form, object->corruptedFormat());
		mUi->expander->hide();
		mWaveGroup.addWidget(mUi->corruptedWaveForm);
	}
	else
	{
		mUi->corruptedContainer->hide();
		mWaveGroup.removeWidget(mUi->corruptedWaveForm);
	}

	form = object->waveForm(ViAudioObject::Corrected);
	if(form != NULL && !form->isEmpty() && (ViAudioObject::Corrected & types))
	{
		mUi->correctedContainer->show();
		mUi->correctedWaveForm->setWaveForm(form, object->correctedFormat());
		mUi->expander->hide();
		mWaveGroup.addWidget(mUi->correctedWaveForm);
	}
	else
	{
		mUi->correctedContainer->hide();
		mWaveGroup.removeWidget(mUi->correctedWaveForm);
	}
}

void ViWaveAnalysisWidget::clear()
{
	mUi->targetWaveForm->clear();
	mUi->corruptedWaveForm->clear();
	mUi->correctedWaveForm->clear();
	mUi->targetContainer->hide();
	mUi->corruptedContainer->hide();
	mUi->correctedContainer->hide();

	mUi->expander->show();
}

void ViWaveAnalysisWidget::hideEvent(QHideEvent *event)
{
	clear();
	mUi->projectLoader->clear();
}
