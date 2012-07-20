#ifndef VIAUDIOMANAGER_H
#define VIAUDIOMANAGER_H

#include <QStringList>
#include <viabstractcoder.h>
#include <vicoder.h>

typedef QList<ViAbstractCoder*> ViCoderList;
typedef QList<ViAudioCodec*> ViCodecList;

class ViAudioManager
{

	public:

		enum Mode
		{
			Supported = 0,
			Available = 1	
		};

		~ViAudioManager();
		static ViAudioManager& instance();
		ViCoder::Error error();

		void addSearchPath(const QString searchPath);
		void addFileName(const QString coderName, const QString name);
		void addFileExtension(const QString coderName, const QString extension);

		ViAudioCodec* codec(const QString name, const ViAudioManager::Mode mode = ViAudioManager::Available);

		ViAbstractCoder* coder(const QString name, const ViAudioManager::Mode mode = ViAudioManager::Available);
		ViAbstractCoder* coder(const ViAudioCodec *codec, const ViAudioManager::Mode mode = ViAudioManager::Available);
		ViAbstractCoder* coder(const ViAudioFormat &format, const ViAudioManager::Mode mode = ViAudioManager::Available);

		ViAbstractCoder* detect(const QString filePath, const ViAudioManager::Mode mode = ViAudioManager::Available);
		ViAbstractCoder* detect(const QByteArray &data, const ViAudioManager::Mode mode = ViAudioManager::Available);

		bool isAvailable(const ViAbstractCoder *coder);
		bool isAvailable(const ViAudioCodec *codec);
		bool isSupported(const ViAbstractCoder *coder);
		bool isSupported(const ViAudioCodec *codec);

		ViCoderList coders(const ViAudioManager::Mode mode = ViAudioManager::Available);
		ViCodecList codecs(const ViAudioManager::Mode mode = ViAudioManager::Available);

	protected:

		ViAudioManager();
		void setError(ViCoder::Error error);

		void initializeSearchPaths();
		void testLibraries();
		bool testLibrary(ViAbstractCoder *coder);

		void add(const ViAudioManager::Mode mode, ViAbstractCoder *coder);
		void add(const ViAudioManager::Mode mode, ViAudioCodec *codec);

	private:

		ViCoder::Error mError;
		ViCoderList mSupportedCoders;
		ViCoderList mAvailableCoders;
		ViCodecList mSupportedCodecs;
		ViCodecList mAvailableCodecs;
		QStringList mSearchPaths;

};

#endif
