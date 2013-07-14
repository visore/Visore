#include <viabstractcoder.h>
#include <QFile>

ViAbstractCoder::ViAbstractCoder()
{
	mName = "undefined";
	mVersion = "undefined";
	setError(ViCoder::NoError);

	addFileExtension("");
	addFileExtension(".a");
	addFileExtension(".lib");
	#ifdef WINDOWS
		addFileExtension(".dll");
	#elif defined MACOSX
		addFileExtension(".so");
		addFileExtension(".dylib");
	#elif defined LINUX
		addFileExtension(".so");
	#endif
}

ViAbstractCoder::~ViAbstractCoder()
{
	mLibrary.unload();
}

const QList<ViAudioCodec*> ViAbstractCoder::supportedCodecs() const
{
	return mSupportedCodecs;
}

ViCoder::Error ViAbstractCoder::load()
{
	if(!mLibrary.isLoaded())
	{
		QFile file(filePath());
		if(!file.exists())
		{
			setError(ViCoder::LibraryFileError);
			return error();
		}
		if(mLibrary.load())
		{
			setError(initializeLibrary());
			if(error() != ViCoder::NoError)
			{
				unload();
			}
			return error();
		}
		else
		{
			setError(ViCoder::LibraryFileError);
			return error();
		}
	}
	setError(ViCoder::NoError);
	return error();
}

ViCoder::Error ViAbstractCoder::load(QString filePath)
{
	setFilePath(filePath);
	return load();
}

bool ViAbstractCoder::unload()
{
	return mLibrary.unload();
}

QString ViAbstractCoder::filePath() const
{
	return mLibrary.fileName();
}

void ViAbstractCoder::setFilePath(QString filePath)
{
	mLibrary.setFileName(filePath);
}

ViAudioFormat ViAbstractCoder::format(ViAudio::Mode mode) const
{
	if(mode == ViAudio::AudioInput)
	{
		return mInputFormat;
	}
	else if(mode == ViAudio::AudioOutput)
	{
		return mOutputFormat;
	}
	return ViAudioFormat();
}

void ViAbstractCoder::setFormat(ViAudio::Mode mode, ViAudioFormat format)
{
	if(mode == ViAudio::AudioInput)
	{
		mInputFormat = format;
	}
	else if(mode == ViAudio::AudioOutput)
	{
		mOutputFormat = format;
	}
}

ViMetadata ViAbstractCoder::metadata() const
{
	return mMetadata;
}

void ViAbstractCoder::setMetadata(const ViMetadata &metadata)
{
	mMetadata = metadata;
}

QString ViAbstractCoder::name() const
{
	return mName;
}

QString ViAbstractCoder::version() const
{
	return mVersion;
}

QStringList ViAbstractCoder::fileNames() const
{
	return mFileNames;
}

QStringList ViAbstractCoder::fileExtensions() const
{
	return mFileExtensions;
}

void ViAbstractCoder::addFileName(QString name)
{
	mFileNames.append(name);
}

void ViAbstractCoder::addFileExtension(QString extension)
{
	mFileExtensions.append(extension);
}

ViCoder::Error ViAbstractCoder::error() const
{
	return mError;
}

void ViAbstractCoder::setError(ViCoder::Error error)
{
	mError = error;
	if(mError != ViCoder::NoError)
	{
		emit failed(mError);
	}
}

bool ViAbstractCoder::operator == (const ViAbstractCoder &other) const
{
	return mName != "" && mName.trimmed().toLower() == other.mName.trimmed().toLower();
}

bool ViAbstractCoder::operator != (const ViAbstractCoder &other) const
{
	return mName == "" || mName.trimmed().toLower() != other.mName.trimmed().toLower();
}
