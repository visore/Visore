#include <vimainwavewidget.h>
#include <ui_vimainwavewidget.h>

ViMainWaveWidget::ViMainWaveWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainWaveWidget();
	mUi->setupUi(this);

	mUi->projectLoader->setMode(ViProjectLoader::SingleTrack);

	QObject::connect(mUi->projectLoader, SIGNAL(trackChanged()), this, SLOT(generateWave()));
	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(generateWave()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), this, SLOT(clearProject()), Qt::DirectConnection);

	mUi->targetGroupBox->hide();
	mUi->corruptedGroupBox->hide();
	mUi->correctedGroupBox->hide();
	mUi->scrollArea->hide();
}

ViMainWaveWidget::~ViMainWaveWidget()
{
	delete mUi;
	clear();
	mUi->projectLoader->clear();
}

void ViMainWaveWidget::clearProject()
{
	clear();
}

void ViMainWaveWidget::generateWave()
{
	mUi->targetWaveForm->clear();
	mUi->corruptedWaveForm->clear();
	mUi->correctedWaveForm->clear();

	ViAudioObjectPointer object = mUi->projectLoader->object();
	QQueue<ViAudio::Type> instructions = ViAudioObject::decomposeTypes(mUi->projectLoader->processTypes());
	QQueue<ViAudio::Type> newInstructions;
	for(int i = 0; i < instructions.size(); ++i)
	{
		if(!object->hasWave(instructions[i]))
		{
			newInstructions.append(instructions[i]);
		}
	}
	if(newInstructions.isEmpty())
	{
		drawWave();
	}
	else
	{
		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(drawWave()));
		engine()->generateWave(object, ViAudioObject::composeTypes(newInstructions));
	}
}

void ViMainWaveWidget::drawWave()
{
	mUi->scrollArea->show();

	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(drawWave()));
	ViAudio::Type types = mUi->projectLoader->processTypes();
	ViWaveForm *form;

	ViAudioObjectPointer object = mUi->projectLoader->object();

	form = object->wave(ViAudio::Target);
	if(form != NULL && !form->isEmpty() && (ViAudio::Target & types))
	{
		mUi->targetGroupBox->show();
		mUi->targetWaveForm->setWaveForm(form, object->targetFormat());
		mWaveGroup.addWidget(mUi->targetWaveForm);
	}
	else
	{
		mUi->targetGroupBox->hide();
		mWaveGroup.removeWidget(mUi->targetWaveForm);
	}

	form = object->wave(ViAudio::Corrupted);
	if(form != NULL && !form->isEmpty() && (ViAudio::Corrupted & types))
	{
		mUi->corruptedGroupBox->show();
		mUi->corruptedWaveForm->setWaveForm(form, object->corruptedFormat());
		mWaveGroup.addWidget(mUi->corruptedWaveForm);
	}
	else
	{
		mUi->corruptedGroupBox->hide();
		mWaveGroup.removeWidget(mUi->corruptedWaveForm);
	}

	form = object->wave(ViAudio::Corrected);
	if(form != NULL && !form->isEmpty() && (ViAudio::Corrected & types))
	{
		mUi->correctedGroupBox->show();
		mUi->correctedWaveForm->setWaveForm(form, object->correctedFormat());
		mWaveGroup.addWidget(mUi->correctedWaveForm);
	}
	else
	{
		mUi->correctedGroupBox->hide();
		mWaveGroup.removeWidget(mUi->correctedWaveForm);
	}
}

void ViMainWaveWidget::clear()
{
	mUi->targetWaveForm->clear();
	mUi->corruptedWaveForm->clear();
	mUi->correctedWaveForm->clear();
	mUi->targetGroupBox->hide();
	mUi->corruptedGroupBox->hide();
	mUi->correctedGroupBox->hide();
	mUi->scrollArea->hide();
}
