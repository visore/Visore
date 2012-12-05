#ifndef VIARCHIVE_H
#define VIARCHIVE_H

#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QThread>

class ViArchiveThread;

class ViArchive : public QObject
{
	Q_OBJECT

	signals:

		void finished();
		void compressFinished();
		void decompressFinished();

	private slots:

		void finish();

	public:
		
		enum Error
		{
			NoError,
			ReadError,
			WriteError,
			SeekError,
			InternalError,
			HeaderError,
			OpenError,
			ArchiveError,
			CorruptedError,
			DeviceError,
			FileError,
			UnknownError
		};

		enum Action
		{
			Unknown,
			Compress,
			Decompress 
		};

		ViArchive(QString filePath = "", int compression = 10, QString comment = "");
		~ViArchive();
		void setFilePath(QString filePath);
		void setCompression(int compression);
		void setComment(QString comment);

		QString filePath();
		ViArchive::Error error();
		QString errorString();
		QStringList fileList(QString extension = "");
		bool isValid();

		void compressData(QFileInfoList filesAndDirs);
		void compressData(QString directory);
		bool decompressData(QString location);
		bool decompressData(QString location, QString file);
		bool decompressData(QString location, QStringList files);

	private:

		ViArchiveThread *mThread;
		ViArchive::Action mAction;

};

class ViArchiveThread : public QThread
{
	public:

		ViArchiveThread();
		void setAction(ViArchive::Action action);
		void setFilePath(QString filePath);
		void setCompression(int compression);
		void setComment(QString comment);

		QString filePath();
		ViArchive::Error error();

		void setInput(QFileInfoList files);
		void setOutput(QString location);
		void setOutput(QString location, QString file);
		void setOutput(QString location, QStringList files);

		void run();
		void compressData();
		void decompressData();

	protected:
		
		bool convertZipError(int code);
		bool convertUnzipError(int code);

	private:

		void (ViArchiveThread::*doAction)();

		bool mDecompressAll;
		QString mFilePath;
		QFileInfoList mInput;
		QString mOutputLocation;
		QStringList mOutputFiles;
		int mCompression;
		QString mComment;
		ViArchive::Error mError;
};

#endif
