#ifndef VICODINGCHAINCOMPONENT_H
#define VICODINGCHAINCOMPONENT_H

#include <QMutex>
#include <QThread>
#include <QFile>
#include <viabstractcoder.h>
#include <viaudiobuffer.h>
#include <QMutexLocker>

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

		void failed(ViCoder::Error error);

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
ViCodingChainBufferInput
**********************************************************/

class ViCodingChainBufferInput : public ViCodingChainInput
{

	Q_OBJECT

	public:

		ViCodingChainBufferInput();
		~ViCodingChainBufferInput();
		void setBuffer(ViAudioBuffer *buffer);
		bool hasData();
		int size();
		void initialize();
		void execute();
		void finalize();

	protected:

		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mStream;
		QMutex mMutex;

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

	public:

		ViCodingChainBufferOutput();
		~ViCodingChainBufferOutput();
		void setBuffer(ViAudioBuffer *buffer);
		void initialize();
		void execute();
		void finalize();

	protected:

		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mStream;


};

#endif
