#include <vimainplaybackwidget.h>
#include <ui_vimainplaybackwidget.h>

ViMainPlaybackWidget::ViMainPlaybackWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainPlaybackWidget();
	mUi->setupUi(this);

	mUi->projectLoader->setProjectMode(ViProjectLoader::SingleTrack);
	mUi->projectLoader->setTypeMode(ViProjectLoader::SingleType);

	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(loadTrack()));

	QObject::connect(mUi->playbackWidget, SIGNAL(played()), engine().data(), SLOT(startPlayback()));
	QObject::connect(mUi->playbackWidget, SIGNAL(paused()), engine().data(), SLOT(pausePlayback()));
	QObject::connect(mUi->playbackWidget, SIGNAL(stopped()), engine().data(), SLOT(stopPlayback()));
	QObject::connect(mUi->playbackWidget, SIGNAL(positionMoved(ViAudioPosition)), engine().data(), SLOT(setPlaybackPosition(ViAudioPosition)));
	QObject::connect(mUi->playbackWidget, SIGNAL(volumeChanged(int)), engine().data(), SLOT(setPlaybackVolume(int)));

	QObject::connect(engine().data(), SIGNAL(positionChanged(ViAudioPosition)), mUi->playbackWidget, SLOT(changePosition(ViAudioPosition)));
	QObject::connect(engine().data(), SIGNAL(durationChanged(ViAudioPosition)), mUi->playbackWidget, SLOT(changeDuration(ViAudioPosition)));

	clear();
}

ViMainPlaybackWidget::~ViMainPlaybackWidget()
{
	clear();
	delete mUi;
}

void ViMainPlaybackWidget::loadTrack()
{
	mUi->playbackWidget->stop();
	mUi->playbackWidget->clear();
	engine()->clearPlayback();

	mUi->playbackWidget->show();
	mUi->metadataWidget->show();

	ViAudioObjectQueue objects = mUi->projectLoader->objects();
	for(int i = 0; i < objects.size(); ++i)
	{
		objects[i]->clearBuffers();
	}

	ViAudioObjectPointer object = mUi->projectLoader->object();
	ViAudio::Type type = mUi->projectLoader->type();

	mUi->metadataWidget->clear();
	mUi->metadataWidget->setMetadata(object->metadata());
	QObject::connect(engine().data(), SIGNAL(durationChanged(ViAudioPosition)), this, SLOT(changeMetadata()));

	engine()->playback(object, type);
}

void ViMainPlaybackWidget::changeMetadata()
{
	QObject::disconnect(engine().data(), SIGNAL(durationChanged(ViAudioPosition)), this, SLOT(changeMetadata()));
	ViAudioObjectPointer object = mUi->projectLoader->object();
	ViAudio::Type type = mUi->projectLoader->type();
	mUi->metadataWidget->setFileInfo(ViAudioPosition::convertPosition(object->bufferSize(type), ViAudioPosition::Bytes, ViAudioPosition::Milliseconds, object->format(type)), object->fileSize(type));
	mUi->metadataWidget->setFormat(object->format(type));
}

void ViMainPlaybackWidget::clear()
{
	mUi->projectLoader->clear();
	mUi->playbackWidget->clear();
	mUi->playbackWidget->hide();
	mUi->metadataWidget->clear();
	mUi->metadataWidget->hide();
	engine()->clearPlayback();
}
