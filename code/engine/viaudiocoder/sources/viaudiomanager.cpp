#include <viaudiomanager.h>
#include <viaudiocodec.h>
#include <viwavecoder.h>
#include <vilamecoder.h>
#include <viflaccoder.h>
#include <QSharedPointer>
#include <QDir>


#include<iostream>
using namespace std;
#define DEFAUL_HEADER_SIZE 8192
#define MAXIMUM_HEADER_TESTS 50

ViAudioManager::ViAudioManager()
{
	setError(ViCoder::NoError);

	add(ViAudioManager::Supported, new ViWaveCoder);
	add(ViAudioManager::Supported, new ViLameCoder);
	add(ViAudioManager::Supported, new ViFlacCoder);

	initializeSearchPaths();
	testLibraries();
}

ViAudioManager::~ViAudioManager()
{
	qDeleteAll(mSupportedCoders);
	mSupportedCoders.clear();
}

ViAudioManager& ViAudioManager::instance()
{
	static ViAudioManager instance;
	return instance;
}

ViCoder::Error ViAudioManager::error()
{
	return instance().mError;
}

void ViAudioManager::setError(ViCoder::Error error)
{
	mError = error;
}

void ViAudioManager::addFileName(const QString coderName, const QString name)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	ViAbstractCoder *theCoder = coder(coderName, ViAudioManager::Available);
	if(theCoder != NULL)
	{
		theCoder->addFileName(name);
		manager.testLibrary(theCoder);
	}
}

void ViAudioManager::addFileExtension(const QString coderName, const QString extension)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	ViAbstractCoder *theCoder = coder(coderName, ViAudioManager::Available);
	if(theCoder != NULL)
	{
		theCoder->addFileExtension(extension);
		manager.testLibrary(theCoder);
	}
}

void ViAudioManager::addSearchPath(const QString searchPath)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	manager.mSearchPaths.append(searchPath);
	manager.testLibraries();
}

ViAudioCodec* ViAudioManager::codec(const QString name, const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	QString newName = name.trimmed().toLower();
	if(mode == ViAudioManager::Available)
	{
		for(int i = 0; i < manager.mAvailableCodecs.size(); ++i)
		{
			if(manager.mAvailableCodecs[i]->longName().toLower() == newName || manager.mAvailableCodecs[i]->shortName().toLower() == newName)
			{
				manager.setError(ViCoder::NoError);
				return manager.mAvailableCodecs[i];
			}
			QStringList abbreviations = manager.mAvailableCodecs[i]->abbreviations();
			for(int k = 0; k < abbreviations.size(); ++k)
			{
				if(abbreviations[k].toLower() == newName)
				{
					manager.setError(ViCoder::NoError);
					return manager.mAvailableCodecs[i];
				}
			}
		}
		if(manager.codec(name, ViAudioManager::Supported) != NULL)
		{
			manager.setError(ViCoder::UnavailableCodecError);
		}
	}
	else
	{
		for(int i = 0; i < manager.mSupportedCodecs.size(); ++i)
		{
			if(manager.mSupportedCodecs[i]->longName().toLower() == newName || manager.mSupportedCodecs[i]->shortName().toLower() == newName)
			{
				manager.setError(ViCoder::NoError);
				return manager.mSupportedCodecs[i];
			}
			QStringList abbreviations = manager.mSupportedCodecs[i]->abbreviations();
			for(int k = 0; k < abbreviations.size(); ++k)
			{
				if(abbreviations[k].toLower() == newName)
				{
					manager.setError(ViCoder::NoError);
					return manager.mSupportedCodecs[i];
				}
			}
		}
		manager.setError(ViCoder::UnsupportedCodecError);
	}
	return NULL;
}

ViAbstractCoder* ViAudioManager::coder(const QString name, const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	QString newName = name.trimmed().toLower(); 
	if(mode == ViAudioManager::Available)
	{
		for(int i = 0; i < manager.mAvailableCoders.size(); ++i)
		{
			if(manager.mAvailableCoders[i]->name().toLower() == newName)
			{
				return manager.mAvailableCoders[i];
			}
			QList<ViAudioCodec*> codecs = manager.mAvailableCoders[i]->supportedCodecs();
			for(int j = 0; j < codecs.size(); ++j)
			{
				if(codecs[j]->longName().toLower() == newName || codecs[j]->shortName().toLower() == newName)
				{
					return manager.mAvailableCoders[i];
				}
				QStringList abbreviations = codecs[j]->abbreviations();
				for(int k = 0; k < abbreviations.size(); ++k)
				{
					if(abbreviations[k].toLower() == newName)
					{
						return manager.mAvailableCoders[i];
					}
				}
			}
		}
		if(manager.coder(name, ViAudioManager::Supported) != NULL)
		{
			manager.setError(ViCoder::UnavailableCodecError);
		}
	}
	else
	{
		for(int i = 0; i < manager.mSupportedCoders.size(); ++i)
		{
			if(manager.mSupportedCoders[i]->name().toLower() == newName)
			{
				return manager.mSupportedCoders[i];
			}
			QList<ViAudioCodec*> codecs = manager.mSupportedCoders[i]->supportedCodecs();
			for(int j = 0; j < codecs.size(); ++j)
			{
				if(codecs[j]->longName().toLower() == newName || codecs[j]->shortName().toLower() == newName)
				{
					return manager.mSupportedCoders[i];
				}
				QStringList abbreviations = codecs[j]->abbreviations();
				for(int k = 0; k < abbreviations.size(); ++k)
				{
					if(abbreviations[k].toLower() == newName)
					{
						return manager.mSupportedCoders[i];
					}
				}
			}
		}
		manager.setError(ViCoder::UnsupportedCodecError);
	}
	return NULL;
}

ViAbstractCoder* ViAudioManager::coder(const ViAudioCodec *codec, const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	if(codec != NULL)
	{
		if(mode == ViAudioManager::Available)
		{
			for(int i = 0; i < manager.mAvailableCoders.size(); ++i)
			{
				QList<ViAudioCodec*> codecs = manager.mAvailableCoders[i]->supportedCodecs();
				for(int j = 0; j < codecs.size(); ++j)
				{
					if((*codecs[j]) == (*codec))
					{
						return manager.mAvailableCoders[i];
					}
				}
			}
			if(manager.coder(codec, ViAudioManager::Supported) != NULL)
			{
				manager.setError(ViCoder::UnavailableCodecError);
			}
		}
		else
		{
			for(int i = 0; i < manager.mSupportedCoders.size(); ++i)
			{
				QList<ViAudioCodec*> codecs = manager.mSupportedCoders[i]->supportedCodecs();
				for(int j = 0; j < codecs.size(); ++j)
				{
					if((*codecs[j]) == (*codec))
					{
						return manager.mSupportedCoders[i];
					}
				}
			}
			manager.setError(ViCoder::UnsupportedCodecError);
		}
	}
	else
	{
		manager.setError(ViCoder::UnsupportedCodecError);
	}
	return NULL;
}

ViAbstractCoder* ViAudioManager::coder(const ViAudioFormat &format, const ViAudioManager::Mode mode)
{
	return ViAudioManager::coder(format.codec(), mode);
}

ViAbstractCoder* ViAudioManager::detect(const QString filePath, const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	QFile file(filePath);
	if(!file.open(QIODevice::ReadOnly))
	{
		manager.setError(ViCoder::InputFileError);
		return NULL;
	}

	ViCoderList *coders;
	if(mode == ViAudioManager::Available)
	{
		coders = &manager.mAvailableCoders;
	}
	else
	{
		coders = &manager.mSupportedCoders;
	} 

	QByteArray data;
	for(int i = 0; i < MAXIMUM_HEADER_TESTS; ++i)
	{
		data.append(file.read(DEFAUL_HEADER_SIZE));
		for(int j = 0; j < coders->size(); ++j)
		{
			if(coders->at(j)->detectCodec(data) != NULL)
			{
				file.close();
				return coders->at(j);
			}
		}
	}

	if(mode == ViAudioManager::Available)
	{
		if(manager.detect(filePath, ViAudioManager::Supported) != NULL)
		{
			manager.setError(ViCoder::UnavailableCodecError);
		}
	}
	else
	{
		manager.setError(ViCoder::UnsupportedCodecError);
	}

	file.close();
	return NULL;
}

ViAbstractCoder* ViAudioManager::detect(const QByteArray &data, const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	ViCoderList *coders;
	if(mode == ViAudioManager::Available)
	{
		coders = &manager.mAvailableCoders;
	}
	else
	{
		coders = &manager.mSupportedCoders;
	}
	for(int i = 0; i < coders->size(); ++i)
	{
		if(coders->at(i)->detectCodec(data) != NULL)
		{
			return coders->at(i);
		}
	}
	if(mode == ViAudioManager::Available)
	{
		if(manager.detect(data, ViAudioManager::Supported) != NULL)
		{
			manager.setError(ViCoder::UnavailableCodecError);
		}
	}
	else
	{
		manager.setError(ViCoder::UnsupportedCodecError);
	}
	return NULL;
}

bool ViAudioManager::isAvailable(const ViAbstractCoder *coder)
{
	setError(ViCoder::NoError);
	if(coder != NULL)
	{
		for(int i = 0; i < mAvailableCoders.size(); ++i)
		{
			if((*mAvailableCoders[i]) == (*coder))
			{
				return true;
			}
		}
	}
	return false;
}

bool ViAudioManager::isAvailable(const ViAudioCodec *codec)
{
	setError(ViCoder::NoError);
	if(codec != NULL)
	{
		for(int i = 0; i < mAvailableCodecs.size(); ++i)
		{
			if((*mAvailableCodecs[i]) == (*codec))
			{
				return true;
			}
		}
	}
	return false;
}

bool ViAudioManager::isSupported(const ViAbstractCoder *coder)
{
	setError(ViCoder::NoError);
	if(coder != NULL)
	{
		for(int i = 0; i < mSupportedCoders.size(); ++i)
		{
			if((*mSupportedCoders[i]) == (*coder))
			{
				return true;
			}
		}
	}
	return false;
}

bool ViAudioManager::isSupported(const ViAudioCodec *codec)
{
	setError(ViCoder::NoError);
	if(codec != NULL)
	{
		for(int i = 0; i < mSupportedCodecs.size(); ++i)
		{
			if((*mSupportedCodecs[i]) == (*codec))
			{
				return true;
			}
		}
	}
	return false;
}

ViCoderList ViAudioManager::coders(const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	if(mode == ViAudioManager::Available)
	{
		return manager.mAvailableCoders;
	}
	else
	{
		return manager.mSupportedCoders;
	}
}

ViCodecList ViAudioManager::codecs(const ViAudioManager::Mode mode)
{
	ViAudioManager &manager = ViAudioManager::instance();
	manager.setError(ViCoder::NoError);
	if(mode == ViAudioManager::Available)
	{
		return manager.mAvailableCodecs;
	}
	else
	{
		return manager.mSupportedCodecs;
	}
}

void ViAudioManager::initializeSearchPaths()
{
	setError(ViCoder::NoError);
	QStringList paths;

	paths.append(QDir::currentPath() + QDir::separator());
	paths.append(QDir::currentPath() + QDir::separator() + "libraries");
	paths.append(QDir::currentPath() + QDir::separator() + "library");
	paths.append(QDir::currentPath() + QDir::separator() + "Libraries");
	paths.append(QDir::currentPath() + QDir::separator() + "Library");
	paths.append(QDir::currentPath() + QDir::separator() + "lib");
	paths.append(QDir::currentPath() + QDir::separator() + "Lib");
	paths.append(QDir::currentPath() + QDir::separator() + "libs");
	paths.append(QDir::currentPath() + QDir::separator() + "Libs");

	#ifdef WINDOWS
		paths.append("c:" + QDir::separator() + "program files (x86)");
		paths.append("c:" + QDir::separator() + "program files");
	#elif defined MACOSX
		paths.append(QString("") + QDir::separator() + "lib");
		paths.append(QString("") + QDir::separator() + "lib64");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "lib");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "lib64");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "local" + QDir::separator() + "lib");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "local" + QDir::separator() + "lib64");
	#elif defined LINUX
		paths.append(QString("") + QDir::separator() + "lib");
		paths.append(QString("") + QDir::separator() + "lib64");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "lib");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "lib64");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "lib" + QDir::separator() + "i386-linux-gnu");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "lib" + QDir::separator() + "x86_64-linux-gnu");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "local" + QDir::separator() + "lib");
		paths.append(QString("") + QDir::separator() + "usr" + QDir::separator() + "local" + QDir::separator() + "lib64");
	#endif

	for(int i = 0; i < paths.size(); ++i)
	{
		QDir dir(paths[i]);
		if(dir.exists())
		{
			mSearchPaths.append(dir.absolutePath() + QDir::separator());
		}
	}
}

void ViAudioManager::testLibraries()
{
	setError(ViCoder::NoError);
	for(int i = 0; i < mSupportedCoders.size(); ++i)
	{
		testLibrary(mSupportedCoders[i]);
	}
}

bool ViAudioManager::testLibrary(ViAbstractCoder *coder)
{
	setError(ViCoder::NoError);
	QStringList nameFilters;
	QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot;
	QDir dir;
	QFileInfoList possibles;

	QStringList fileNames = coder->fileNames();
	QStringList fileExtensions = coder->fileExtensions();
	for(int j = 0; j < mSearchPaths.size(); ++j)
	{
		QString path = mSearchPaths[j];
		for(int k = 0; k < fileNames.size(); ++k)
		{
			QString fileName = fileNames[k];
			for(int h = 0; h < fileNames.size(); ++h)
			{
				QString directoryName = fileNames[h];
				for(int i = 0; i < fileExtensions.size(); ++i)
				{
					if(	coder->load(path + fileName + fileExtensions[i]) == ViCoder::NoError ||
						coder->load(path + "lib" + fileName + fileExtensions[i]) == ViCoder::NoError ||
						coder->load(path + directoryName + QDir::separator() + fileName + fileExtensions[i]) == ViCoder::NoError ||
						coder->load(path + directoryName + QDir::separator() + "lib" + fileName + fileExtensions[i]) == ViCoder::NoError ||
						coder->load(path + "lib" + directoryName + QDir::separator() + fileName + fileExtensions[i]) == ViCoder::NoError ||
						coder->load(path + "lib" + directoryName + QDir::separator() + "lib" + fileName + fileExtensions[i]) == ViCoder::NoError)
					{
						coder->unload();
						add(ViAudioManager::Available, coder);
						return true;
					}

					//Check for libraries that end in version number. Eg: libflac.so.8
//cout<<path.toAscii().data()<<" "<<fileName.toAscii().data()<<endl;
					nameFilters.clear();
					possibles.clear();

					nameFilters << fileName + fileExtensions[i] + "*";
					nameFilters << "lib" + fileName + fileExtensions[i] + "*";

					dir.setPath(path);
					possibles.append(dir.entryInfoList(nameFilters, filters, QDir::Name));
					dir.setPath(path + directoryName);
					possibles.append(dir.entryInfoList(nameFilters, filters, QDir::Name));
					dir.setPath(path + "lib" + directoryName);
					possibles.append(dir.entryInfoList(nameFilters, filters, QDir::Name));

					for(int t = 0; t < possibles.size(); ++t)
					{
						if(coder->load(possibles[t].absoluteFilePath()) == ViCoder::NoError)
						{
							coder->unload();
							add(ViAudioManager::Available, coder);
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void ViAudioManager::add(const ViAudioManager::Mode mode,  ViAbstractCoder *coder)
{
	setError(ViCoder::NoError);
	if(mode == ViAudioManager::Available)
	{
		if(!isAvailable(coder))
		{
			mAvailableCoders.append(coder);
			QList<ViAudioCodec*> codecs = coder->supportedCodecs();
			for(int i = 0; i < codecs.size(); ++i)
			{
				add(mode, codecs[i]);
			}
		}
	}
	else
	{
		if(!isSupported(coder))
		{
			mSupportedCoders.append(coder);
			QList<ViAudioCodec*> codecs = coder->supportedCodecs();
			for(int i = 0; i < codecs.size(); ++i)
			{
				add(mode, codecs[i]);
			}
		}
	}
}

void ViAudioManager::add(const ViAudioManager::Mode mode, ViAudioCodec *codec)
{
	setError(ViCoder::NoError);
	if(mode == ViAudioManager::Available)
	{
		if(!isAvailable(codec))
		{
			mAvailableCodecs.append(codec);
		}
	}
	else
	{
		if(!isSupported(codec))
		{
			mSupportedCodecs.append(codec);
		}
	}
}
