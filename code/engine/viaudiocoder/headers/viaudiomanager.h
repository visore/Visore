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
		static ViCoder::Error error();

		static void addSearchPath(const QString searchPath);
		static void addFileName(const QString coderName, const QString name);
		static void addFileExtension(const QString coderName, const QString extension);

		static ViAudioCodec* codec(const QString name, const ViAudioManager::Mode mode = ViAudioManager::Available);

		static ViAbstractCoder* coder(const QString name, const ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViAbstractCoder* coder(const ViAudioCodec *codec, const ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViAbstractCoder* coder(const ViAudioFormat &format, const ViAudioManager::Mode mode = ViAudioManager::Available);

		static ViAbstractCoder* detect(const QString filePath, const ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViAbstractCoder* detect(const QByteArray &data, const ViAudioManager::Mode mode = ViAudioManager::Available);

		static ViCoderList coders(const ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViCodecList codecs(const ViAudioManager::Mode mode = ViAudioManager::Available);

	protected:

		ViAudioManager();
		static ViAudioManager& instance();
		void setError(ViCoder::Error error);

		bool isAvailable(const ViAbstractCoder *coder);
		bool isAvailable(const ViAudioCodec *codec);
		bool isSupported(const ViAbstractCoder *coder);
		bool isSupported(const ViAudioCodec *codec);

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
