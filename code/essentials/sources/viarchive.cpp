#include "viarchive.h"
#include "zip.h"
#include "unzip.h"
#include <QDir>
#include <QFileInfo>

ViArchiveThread::ViArchiveThread()
	: QThread()
{
	mFilePath = "";
	mCompression = 10;
	mComment = "";
	mDecompressAll = true;
	mError = ViArchive::None;
}

void ViArchiveThread::setAction(ViArchiveThread::Action action)
{
	if(action == ViArchiveThread::Compress)
	{
		doAction = &ViArchiveThread::compressData;
	}
	else if(action == ViArchiveThread::Decompress)
	{
		doAction = &ViArchiveThread::decompressData;
	}
}

void ViArchiveThread::setFilePath(QString filePath)
{
	mFilePath = filePath;
}

void ViArchiveThread::setCompression(int compression)
{
	mCompression = compression;
}

void ViArchiveThread::setComment(QString comment)
{
	mComment = comment;
}

QString ViArchiveThread::filePath()
{
	return mFilePath;
}

ViArchive::Error ViArchiveThread::error()
{
	return mError;
}

void ViArchiveThread::setInput(QFileInfoList files)
{
	mInput = files;
}

void ViArchiveThread::setOutput(QString location)
{
	mDecompressAll = true;
	mOutputLocation = location;
}

void ViArchiveThread::setOutput(QString location, QString file)
{
	mDecompressAll = false;
	mOutputLocation = location;
	mOutputFiles.clear();
	mOutputFiles.append(file);
}

void ViArchiveThread::setOutput(QString location, QStringList files)
{
	mDecompressAll = false;
	mOutputLocation = location;
	mOutputFiles.clear();
	mOutputFiles = files;
}

void ViArchiveThread::compressData()
{
	QDir dir(mFilePath.mid(0, mFilePath.lastIndexOf(QDir::separator())));
	if(!dir.exists())
	{
		dir.mkpath(dir.absolutePath());
	}

	Zip::CompressionLevel compression;
	if(mCompression == 1)
	{
		compression = Zip::Deflate1;
	}
	else if(mCompression == 2)
	{
		compression = Zip::Deflate2;
	}
	else if(mCompression == 3)
	{
		compression = Zip::Deflate3;
	}
	else if(mCompression == 4)
	{
		compression = Zip::Deflate4;
	}
	else if(mCompression == 5)
	{
		compression = Zip::Deflate5;
	}
	else if(mCompression == 6)
	{
		compression = Zip::Deflate6;
	}
	else if(mCompression == 7)
	{
		compression = Zip::Deflate7;
	}
	else if(mCompression == 8)
	{
		compression = Zip::Deflate8;
	}
	else if(mCompression == 9)
	{
		compression = Zip::Deflate9;
	}
	else
	{
		compression = Zip::AutoFull;
	}

	Zip zip;
	zip.setArchiveComment(mComment);

	if(!convertZipError(zip.createArchive(mFilePath)))
	{
		return;
	}

	for(int i = 0; i < mInput.size(); ++i)
	{
		if(mInput[i].isDir())
		{
			convertZipError(zip.addDirectory(mInput[i].absoluteFilePath(), compression));
		}
		else if(mInput[i].isFile())
		{
			convertZipError(zip.addFile(mInput[i].absoluteFilePath(), compression));
		}
	}

	convertZipError(zip.closeArchive());
}

void ViArchiveThread::decompressData()
{
	QDir dir("");
	dir.mkpath(dir.absolutePath());

	UnZip unzip;
	if(!convertUnzipError(unzip.openArchive(mFilePath)))
	{
		return;
	}

	if(mDecompressAll)
	{
		convertUnzipError(unzip.extractAll(mOutputLocation));
	}
	else
	{
		convertUnzipError(unzip.extractFiles(mOutputFiles, mOutputLocation));
	}
	unzip.closeArchive();
}

bool ViArchiveThread::convertZipError(int code)
{
	if(code == Zip::Ok)
	{
		mError = ViArchive::None;
		return true;
	}
	else if(code == Zip::ZlibInit || code == Zip::ZlibError || code == Zip::InternalError)
	{
		mError = ViArchive::InternalError;
	}
	else if(code == Zip::FileExists)
	{
		mError = ViArchive::FileError;
	}
	else if(code == Zip::OpenFailed || code == Zip::FileNotFound)
	{
		mError = ViArchive::OpenError;
	}
	else if(code == Zip::NoOpenArchive)
	{
		mError = ViArchive::ArchiveError;
	}
	else if(code == Zip::ReadFailed)
	{
		mError = ViArchive::ReadError;
	}
	else if(code == Zip::WriteFailed)
	{
		mError = ViArchive::WriteError;
	}
	else if(code == Zip::SeekFailed)
	{
		mError = ViArchive::SeekError;
	}
	else
	{
		mError = ViArchive::None;
	}
	return false;
}

bool ViArchiveThread::convertUnzipError(int code)
{
	if(code == UnZip::Ok)
	{
		mError = ViArchive::None;
		return true;
	}
	else if(code == UnZip::ZlibInit || code == UnZip::ZlibError)
	{
		mError = ViArchive::InternalError;
	}
	else if(code == UnZip::OpenFailed || code == UnZip::FileNotFound)
	{
		mError = ViArchive::OpenError;
	}
	else if(code == UnZip::ReadFailed)
	{
		mError = ViArchive::ReadError;
	}
	else if(code == UnZip::WriteFailed)
	{
		mError = ViArchive::WriteError;
	}
	else if(code == UnZip::SeekFailed)
	{
		mError = ViArchive::SeekError;
	}
	else if(code == UnZip::Corrupted || code == UnZip::PartiallyCorrupted)
	{
		mError = ViArchive::CorruptedError;
	}
	else if(code == UnZip::NoOpenArchive || code == UnZip::InvalidArchive)
	{
		mError = ViArchive::ArchiveError;
	}
	else if(code == UnZip::InvalidDevice)
	{
		mError = ViArchive::DeviceError;
	}
	else if(code == UnZip::HeaderConsistencyError)
	{
		mError = ViArchive::HeaderError;
	}
	return false;
}

void ViArchiveThread::run()
{
	(this->*doAction)();
}

ViArchive::ViArchive(QString filePath, int compression, QString comment)
	: QObject()
{
	mThread = new ViArchiveThread();
	QObject::connect(mThread, SIGNAL(finished()), this, SIGNAL(finished()));
	setFilePath(filePath);
	setCompression(compression);
	setComment(comment);
}

ViArchive::~ViArchive()
{
	delete mThread;
}

void ViArchive::setFilePath(QString filePath)
{
	mThread->setFilePath(filePath);
}

void ViArchive::setCompression(int compression)
{
	mThread->setCompression(compression);
}

void ViArchive::setComment(QString comment)
{
	mThread->setComment(comment);
}

QString ViArchive::filePath()
{
	return mThread->filePath();
}

ViArchive::Error ViArchive::error()
{
	return mThread->error();
}

QStringList ViArchive::fileList(QString extension)
{
	UnZip unzip;
	QStringList result;
	if(unzip.openArchive(mThread->filePath()) == UnZip::Ok)
	{
		QStringList temp = unzip.fileList();
		unzip.closeArchive();
		if(extension != "")
		{
			for(int i = 0; i < temp.size(); ++i)
			{
				if(temp[i].endsWith(extension))
				{
					result.append(temp[i]);
				}
			}
		}
		else
		{
			return temp;
		}
	}
	return result;
}

bool ViArchive::isValid()
{
	UnZip unzip;
	if(unzip.openArchive(mThread->filePath()) == UnZip::Ok)
	{
		unzip.closeArchive();
		return true;
	}
	return false;
}

void ViArchive::compressData(QFileInfoList filesAndDirs)
{
	mThread->setInput(filesAndDirs);
	mThread->setAction(ViArchiveThread::Compress);
	mThread->start();
}

void ViArchive::compressData(QString directory)
{
	QDir dir(directory);
	QFileInfoList filesAndDirs = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	compressData(filesAndDirs);
}

bool ViArchive::decompressData(QString location)
{
	if(isValid())
	{
		mThread->setOutput(location);
		mThread->setAction(ViArchiveThread::Decompress);
		mThread->start();
		return true;
	}
	return false;
}

bool ViArchive::decompressData(QString location, QString file)
{
	if(isValid())
	{
		mThread->setOutput(location, file);
		mThread->setAction(ViArchiveThread::Decompress);
		mThread->start();
		return true;
	}
	return false;
}

bool ViArchive::decompressData(QString location, QStringList files)
{
	if(isValid())
	{
		mThread->setOutput(location, files);
		mThread->setAction(ViArchiveThread::Decompress);
		mThread->start();
		return true;
	}
	return false;
}
