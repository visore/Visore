#include <viaudioengine.h>
#include <vicorrelatormanager.h>

ViAudioEngine::ViAudioEngine()
{
	QObject::connect(&mPlayer, SIGNAL(positionChanged(ViAudioPosition)), this, SIGNAL(positionChanged(ViAudioPosition)));
	QObject::connect(&mPlayer, SIGNAL(durationChanged(ViAudioPosition)), this, SIGNAL(durationChanged(ViAudioPosition)));

	QObject::connect(&mRecorder, SIGNAL(statused(QString)), this, SIGNAL(statusChanged(QString)));
	QObject::connect(&mRecorder, SIGNAL(progressed(qreal)), this, SIGNAL(progressed(qreal)));
	QObject::connect(&mRecorder, SIGNAL(finished()), this, SIGNAL(progressFinished()));

	QObject::connect(&mObjectChain, SIGNAL(progressed(qreal)), this, SIGNAL(progressed(qreal)));
	QObject::connect(&mObjectChain, SIGNAL(finished()), this, SIGNAL(progressFinished()));
	QObject::connect(&mObjectChain, SIGNAL(statused(QString)), this, SIGNAL(statusChanged(QString)));
}

ViAudioEngine::~ViAudioEngine()
{
}

void ViAudioEngine::playback(ViAudioObjectPointer object, ViAudio::Type type)
{
	mPlayer.setData(object, type);
}

void ViAudioEngine::clearPlayback()
{
	mPlayer.stop();
	mPlayer.clear();
}

void ViAudioEngine::startPlayback()
{
	mPlayer.play();
}

void ViAudioEngine::stopPlayback()
{
	mPlayer.stop();
}

void ViAudioEngine::pausePlayback()
{
	mPlayer.pause();
}

void ViAudioEngine::setPlaybackPosition(ViAudioPosition position)
{
	mPlayer.changePosition(position);
}

void ViAudioEngine::setPlaybackVolume(int volume)
{
	mPlayer.changeVolume(volume);
}

void ViAudioEngine::correlate(ViAudioObjectPointer object)
{
	correlate({object});
}

void ViAudioEngine::correlate(ViAudioObjectQueue objects)
{
	mObjectChain.clear();
	//Can't pass it as parameter for correlate(), because the object takes ownership
	for(int i = 0; i < objects.size(); ++i)
	{
		objects[i]->addCorrelators(ViCorrelatorManager::createAll());
	}
	mObjectChain.add(objects);

	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(ViAudio::Target | ViAudio::Corrupted | ViAudio::Corrected)), 0.1);
	mObjectChain.addFunction(ViFunctionCall("align"), 0.04);
	mObjectChain.addFunction(ViFunctionCall("correlate"), 0.85);
	mObjectChain.addFunction(ViFunctionCall("clearBuffers"), 0.01, false);

	QString tracks = "Track";
	if(objects.size() > 1) tracks += "s";
	mObjectChain.execute("Correlating " + tracks);
}

void ViAudioEngine::correct(ViAudioObjectQueue objects, ViModifyProcessor *corrector, const bool &correlate)
{
	mObjectChain.clear();
	mObjectChain.add(objects);

	qreal percentage = 0.97;
	if(correlate) percentage = 0.91;

	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(ViAudio::Corrupted)), 0.01);
	mObjectChain.addFunction(ViFunctionCall("correct", QVariant::fromValue(corrector)), percentage); // THIS ONE CAUSE ERROR IN CORRELATION
	mObjectChain.addFunction(ViFunctionCall("encode", QVariant(ViAudio::Corrected)), 0.01);
	if(correlate)
	{
		mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(ViAudio::Target)), 0.01);
		mObjectChain.addFunction(ViFunctionCall("align"), 0.01);
		mObjectChain.addFunction(ViFunctionCall("correlate", QVariant::fromValue(ViCorrelatorManager::createAll())), 0.04);
	}
	mObjectChain.addFunction(ViFunctionCall("clearBuffers"), 0.01, false);

	QString tracks = "Track";
	if(objects.size() > 1) tracks += "s";
	mObjectChain.execute("Correcting " + tracks);
}

void ViAudioEngine::recordProject(ViProject *project, ViAudio::Type type, ViAudioFormat format, int sides, bool detectInfo)
{
	mRecorder.record(project, type, format, sides, detectInfo);
}

void ViAudioEngine::updateMetadata(ViProject *project)
{
	mObjectChain.clear();
	mObjectChain.add(*project);

	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(ViAudio::Target | ViAudio::Corrupted | ViAudio::Corrected)), 0.40);
	mObjectChain.addFunction(ViFunctionCall("encode", QVariant::fromValue(ViAudio::Target | ViAudio::Corrupted | ViAudio::Corrected)), 0.59);
	mObjectChain.addFunction(ViFunctionCall("clearBuffers"), 0.01, false);

	mObjectChain.execute("Updating Metadata");
}

void ViAudioEngine::generateWave(ViAudioObjectPointer object, ViAudio::Type type, const bool &align)
{
	mObjectChain.clear();
	mObjectChain.add(object);

	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(type)), 0.19);
	qfloat percentage = 0.8;
	if(align)
	{
		percentage = 0.75;
		mObjectChain.addFunction(ViFunctionCall("align"), 0.05);
	}
	mObjectChain.addFunction(ViFunctionCall("generateWave", {QVariant::fromValue(type), true}), percentage);
	mObjectChain.addFunction(ViFunctionCall("clearBuffers"), 0.01, false);

	mObjectChain.execute("Generating Waves");
}

void ViAudioEngine::generateSpectrum(ViAudioObjectPointer object, ViAudio::Type type, qint32 windowSizeSamples, QString windowFunction)
{
	mObjectChain.clear();
	mObjectChain.add(object);
	mObjectChain.addFunction(ViFunctionCall("decode", QVariant::fromValue(type)), 0.09);
	mObjectChain.addFunction(ViFunctionCall("generateSpectrum", {QVariant::fromValue(type), windowSizeSamples, windowFunction, true}), 0.9);
	mObjectChain.addFunction(ViFunctionCall("clearBuffers"), 0.01, false);
	mObjectChain.execute("Generating Frequency Spectrum");
}

void ViAudioEngine::align(ViProject &project)
{
	mObjectChain.clear();
	mObjectChain.add(project);
	mObjectChain.addFunction("align");
	mObjectChain.execute();
}

void ViAudioEngine::align(ViAudioObjectPointer object)
{
	mObjectChain.clear();
	mObjectChain.add(object);
	mObjectChain.addFunction("align");
	mObjectChain.execute();
}
