#ifndef VICODINGCHAINCOMPONENT_H
#define VICODINGCHAINCOMPONENT_H

#include <QThread>
#include <QFile>
#include <viabstractcoder.h>
#include <vibuffer.h>

/**********************************************************
ViCodingChainComponent
**********************************************************/

class ViCodingChainFileInput;
class ViCodingChainDataInput;
class ViCodingChainBufferInput;
class ViCodingChainDecoder;
class ViCodingChainEncoder;
class ViCodingChainFileOutput;
class ViCodingChainDataOutput;
class ViCodingChainBufferOutput;

class ViCodingChainComponent : public QObject
{

	Q_OBJECT

	friend class ViCodingChainFileInput;
	friend class ViCodingChainDataInput;
	friend class ViCodingChainBufferInput;
	friend class ViCodingChainDecoder;
	friend class ViCodingChainEncoder;
	friend class ViCodingChainFileOutput;
	friend class ViCodingChainDataOutput;
	friend class ViCodingChainBufferOutput;

	signals:

		void failed(ViCoder::Error);

	protected slots:

		virtual void changeFormat(ViAudioFormat format);
		void addData(ViSampleArray *data);

	public:

		ViCodingChainComponent();

		ViCoder::Error error();

		void setHeader(QByteArray header);
		void setNext(ViCodingChainComponent *next);
		int chunkSize();

		virtual void initialize() = 0;
		virtual void execute() = 0;
		virtual void finalize() = 0;

	protected:

		void setError(ViCoder::Error error);

	protected:

		ViCoder::Error mError;
		QQueue<ViSampleArray*> mData;
		ViCodingChainComponent *mNext;
		QByteArray mHeader;
		bool mContinue;

};

/**********************************************************
ViCodingChainInput
**********************************************************/

class ViCodingChainInput : public ViCodingChainComponent
{

	Q_OBJECT

	protected slots:

		virtual void seek(int position) = 0;

	public:

		ViCodingChainInput();
		virtual bool hasData() = 0;
		virtual int size() = 0;
		void setSampleSize(int size);
		void setOffsets(qint64 from, qint64 to);

	protected:

		void initializeOffsets(int size);
		int readSize();
		void addReadSize(const int &size);

	protected:

		int mSampleSize;
		qint64 mFromOffset;
		qint64 mToOffset;
		qint64 mTotalReadSize;

};

/**********************************************************
ViCodingChainFileInput
**********************************************************/

class ViCodingChainFileInput : public ViCodingChainInput
{

	Q_OBJECT

	protected slots:

		void seek(int position);

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

	protected slots:

		void seek(int position);

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
ViCodingChainBufferInput
**********************************************************/

class ViCodingChainBufferInput : public ViCodingChainInput
{

	Q_OBJECT

	protected slots:

		void seek(int position);

	public:

		ViCodingChainBufferInput();
		~ViCodingChainBufferInput();
		void setBuffer(ViBuffer *buffer);
		bool hasData();
		int size();
		void initialize();
		void execute();
		void finalize();

	protected:

		ViBuffer *mBuffer;
		ViBufferStreamPointer mStream;

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

	signals:

		void formatChanged(ViAudioFormat);

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
		virtual void initialize();

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
		void initialize();
		void execute();
		void finalize();

	protected:

		QString mFilePath;
		QFile mTempFile;
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
		void initialize();
		void execute();
		void finalize();

	protected:

		QByteArray *mByteArray;
		QDataStream *mStream;

};

/**********************************************************
ViCodingChainBufferOutput
**********************************************************/

class ViCodingChainBufferOutput : public ViCodingChainOutput
{

	Q_OBJECT

	public slots:

		void changeFormat(ViAudioFormat format);

	public:

		ViCodingChainBufferOutput();
		~ViCodingChainBufferOutput();
		void setBuffer(ViBuffer *buffer);
		void initialize();
		void execute();
		void finalize();

	protected:

		ViBuffer *mBuffer;
		ViBufferStreamPointer mStream;


};

#endif
