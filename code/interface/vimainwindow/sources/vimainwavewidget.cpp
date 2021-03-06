#include <vimainwavewidget.h>
#include <ui_vimainwavewidget.h>

ViMainWaveWidget::ViMainWaveWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainWaveWidget();
	mUi->setupUi(this);

	clear();

	mUi->projectLoader->setProjectMode(ViProjectLoader::SingleTrack);

	setStyleSheet("QLabel { min-width: 100px; }");
}

ViMainWaveWidget::~ViMainWaveWidget()
{
	delete mUi;
	clear();
}

void ViMainWaveWidget::clearProject()
{
	mAlignments.clear();
	mUi->targetWaveForm->clear();
	mUi->corruptedWaveForm->clear();
	mUi->correctedWaveForm->clear();
	mUi->targetGroupBox->hide();
	mUi->corruptedGroupBox->hide();
	mUi->correctedGroupBox->hide();
	mUi->scrollArea->hide();
	mUi->alignWidget->hide();
	mUi->alignCheckBox->setChecked(false);
}

void ViMainWaveWidget::generateWave()
{
	mUi->targetWaveForm->clear();
	mUi->corruptedWaveForm->clear();
	mUi->correctedWaveForm->clear();

	ViAudioObjectPointer object = mUi->projectLoader->object();
	QList<ViAudio::Type> instructions = ViAudio::types(mUi->projectLoader->types());
	QList<ViAudio::Type> newInstructions;

	if(!instructions.empty())
	{
		mUi->alignWidget->show();
	}
	bool regenerate = false;
	bool found = false;
	for(int i = 0; i < mAlignments.size(); ++i)
	{
		if(mAlignments[i].first == object)
		{
			found = true;
			if(mAlignments[i].second != mUi->alignCheckBox->isChecked())
			{
				mAlignments[i].second = mUi->alignCheckBox->isChecked();
				regenerate = true;
				break;
			}
		}
	}
	if(!found)
	{
		mAlignments.append(QPair<ViAudioObjectPointer, bool>(object, mUi->alignCheckBox->isChecked()));
		regenerate = true;
	}

	for(int i = 0; i < instructions.size(); ++i)
	{
		if(!object->hasWave(instructions[i]))
		{
			newInstructions.append(instructions[i]);
		}
	}

	if(newInstructions.isEmpty() && !regenerate)
	{
		drawWave();
	}
	else
	{
		ViAudio::Type type;
		if(regenerate) type = mUi->projectLoader->types();
		else type = ViAudio::types(newInstructions);

		QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(drawWave()));
		engine()->generateWave(object, type, mUi->alignCheckBox->isChecked());
	}
}

void ViMainWaveWidget::drawWave()
{
	mUi->scrollArea->show();

	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(drawWave()));
	ViAudio::Type types = mUi->projectLoader->types();
	ViWaveForm *form;

	ViAudioObjectPointer object = mUi->projectLoader->object();

	form = object->wave(ViAudio::Target);
	if(form != NULL && !form->isEmpty() && (ViAudio::Target & types))
	{
		mUi->targetGroupBox->show();
		mUi->targetWaveForm->setWaveForm(form, object->format(ViAudio::Target));
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
		mUi->corruptedWaveForm->setWaveForm(form, object->format(ViAudio::Corrupted));
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
		mUi->correctedWaveForm->setWaveForm(form, object->format(ViAudio::Corrected));
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
	mUi->alignCheckBox->disconnect();
	mUi->projectLoader->disconnect();
	mUi->projectLoader->clear();
	clearProject();
	QObject::connect(mUi->alignCheckBox, SIGNAL(stateChanged(int)), this, SLOT(generateWave()));
	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(generateWave()));
	QObject::connect(mUi->projectLoader, SIGNAL(projectChanged()), this, SLOT(clearProject()), Qt::DirectConnection);
}
