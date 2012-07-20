#ifndef VICODINGCHAINCOMPONENT_H
#define VICODINGCHAINCOMPONENT_H

#include <QMutex>
#include <QThread>
#include <QFile>
#include <viabstractcoder.h>

#include<iostream>
using namespace std;

/**********************************************************
ViCodingChainComponent
**********************************************************/

class ViCodingChainFileInput;
class ViCodingChainDataInput;
class ViCodingChainDecoder;
class ViCodingChainEncoder;
class ViCodingChainFileOutput;
class ViCodingChainDataOutput;

class ViCodingChainComponent : public QObject
{

	Q_OBJECT

	friend class ViCodingChainFileInput;
	friend class ViCodingChainDataInput;
	friend class ViCodingChainDecoder;
	friend class ViCodingChainEncoder;
	friend class ViCodingChainFileOutput;
	friend class ViCodingChainDataOutput;

	signals:

		void failed(ViCoder::Error error);

	protected slots:

		virtual void changeFormat(ViAudioFormat format);
		void addData(ViSampleArray *data);
		void addData(ViSampleArray *data, qint64 position);

	public:

		ViCodingChainComponent();

		ViCoder::Error error();

		void setNext(ViCodingChainComponent *next);
		int chunkSize();

		virtual void initialize() = 0;
		virtual void execute() = 0;
		virtual void finalize() = 0;

	protected:

		virtual void seek(qint64 position);
		void setError(ViCoder::Error error);

	protected:

		ViCoder::Error mError;
		QQueue<ViSampleArray*> mData;
		ViCodingChainComponent *mNext;

};

/**********************************************************
ViCodingChainInput
**********************************************************/

class ViCodingChainInput : public ViCodingChainComponent
{

	Q_OBJECT

	public:

		ViCodingChainInput();
		virtual bool hasData() = 0;
		virtual int size() = 0;
		void setSampleSize(int size);

	protected:

		int mSampleSize;

};

/**********************************************************
ViCodingChainFileInput
**********************************************************/

class ViCodingChainFileInput : public ViCodingChainInput
{

	Q_OBJECT

	public:

		ViCodingChainFileInput();
		void setFilePath(QString filePath);
		bool hasData();
		int size();
		void initialize();
		void execute();
		void finalize();

	protected:

		QString mFilePath;
		QFile mFile;

};

/**********************************************************
ViCodingChainDataInput
**********************************************************/

class ViCodingChainDataInput : public ViCodingChainInput
{

	Q_OBJECT

	public:

		ViCodingChainDataInput();
		~ViCodingChainDataInput();
		void setData(QByteArray &data);
		bool hasData();
		int size();
		void initialize();
		void execute();
		void finalize();

	protected:

		QByteArray *mByteArray;
		QDataStream *mStream;

};

/**********************************************************
ViCodingChainCoder
**********************************************************/

class ViCodingChainCoder : public ViCodingChainComponent
{

	Q_OBJECT

	public:

		ViCodingChainCoder();
		virtual void setCoder(ViAbstractCoder *coder);

	protected:

		ViAbstractCoder *mCoder;

};

/**********************************************************
ViCodingChainDecoder
**********************************************************/

class ViCodingChainDecoder : public ViCodingChainCoder
{

	Q_OBJECT

	public:

		ViCodingChainDecoder();
		void setCoder(ViAbstractCoder *coder);
		void initialize();
		void execute();
		void finalize();

};

/**********************************************************
ViCodingChainEncoder
**********************************************************/

class ViCodingChainEncoder : public ViCodingChainCoder
{

	Q_OBJECT

	public slots:

		void changeFormat(ViAudioFormat format);

	public:

		ViCodingChainEncoder();
		void initialize();
		void execute();
		void finalize();

};

/**********************************************************
ViCodingChainOutput
**********************************************************/

class ViCodingChainOutput : public ViCodingChainComponent
{

	Q_OBJECT

	public:

		ViCodingChainOutput();

};

/**********************************************************
ViCodingChainFileOutput
**********************************************************/

class ViCodingChainFileOutput : public ViCodingChainOutput
{

	Q_OBJECT

	public:

		ViCodingChainFileOutput();
		void setFilePath(QString filePath);
		void seek(qint64 position);
		void initialize();
		void execute();
		void finalize();

	protected:

		QString mFilePath;
		QFile mFile;

};

/**********************************************************
ViCodingChainDataOutput
**********************************************************/

class ViCodingChainDataOutput : public ViCodingChainOutput
{

	Q_OBJECT

	public:

		ViCodingChainDataOutput();
		~ViCodingChainDataOutput();
		void setData(QByteArray &data);
		void seek(qint64 position);
		void initialize();
		void execute();
		void finalize();

	protected:

		QByteArray *mByteArray;
		QDataStream *mStream;

};

#endif
