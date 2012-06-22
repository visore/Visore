#ifndef VIARCHIVE_H
#define VIARCHIVE_H

#include <QString>
#include <QStringList>
#include <QThread>

class ViArchiveThread;

class ViArchive : public QObject
{
	Q_OBJECT

	signals:

		void finished();

	public:
		
		enum Error
		{
			None = 0,
			ReadError = 1,
			WriteError = 2,
			SeekError = 3,
			InternalError = 4,
			HeaderError = 5,
			OpenError = 6,
			ArchiveError = 7,
			CorruptedError = 8,
			DeviceError = 9,
			FileError = 10,
			Unknown = 11
		};

		ViArchive(QString filePath = "", int compression = 10, QString comment = "");
		~ViArchive();
		void setFilePath(QString filePath);
		void setCompression(int compression);
		void setComment(QString comment);
		ViArchive::Error error();

		void compress(QStringList filesAndDirs = QStringList());
		void decompress(QString location);

	private:

		ViArchiveThread *mThread;

};

class ViArchiveThread : public QThread
{
	public:

		enum Action
		{
			Compress = 0,
			Decompress = 1
		};

		ViArchiveThread();
		void setAction(ViArchiveThread::Action action);
		void setFilePath(QString filePath);
		void setCompression(int compression);
		void setComment(QString comment);
		ViArchive::Error error();

		void setInput(QStringList files);
		void setOutput(QString location);

		void run();
		void compress();
		void decompress();
	protected:


		
		bool convertZipError(int code);
		bool convertUnzipError(int code);

	private:

		void (ViArchiveThread::*doAction)();

		QString mFilePath;
		QStringList mInputFiles;
		QString mOutputLocation;
		int mCompression;
		QString mComment;
		ViArchive::Error mError;
};

#endif
