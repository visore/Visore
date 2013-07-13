#ifndef VICODINGCHAIN_H
#define VICODINGCHAIN_H

#include <vicodingchaincomponent.h>

class ViCodingChain : public QThread
{

	Q_OBJECT

	signals:

		void failed(ViCoder::Error error);
		void progressed(qreal percentage);
		void formatChanged(ViAudioFormat format);

	protected slots:

		void setError(ViCoder::Error error);

	public:

		enum Mode
		{
			Unknown = 0,
			ConvertFileToFile = 1,
			ConvertFileToData = 2,
			ConvertDataToData = 3,
			ConvertDataToFile = 4,
			DecodeFile = 5,
			DecodeData = 6,
			EncodeFile = 7,
			EncodeData = 8
		};

		ViCodingChain();

		void setMode(ViCodingChain::Mode mode);

		void setSongInfo(ViSongInfo info);

		void setInputPath(QString filePath);
		void setOutputPath(QString filePath);

		void setInputData(QByteArray &array);
		void setOutputData(QByteArray &array);

		void setInputBuffer(ViBuffer *buffer);
		void setOutputBuffer(ViBuffer *buffer);

		void setInputFormat(ViAudioFormat format);
		void setOutputFormat(ViAudioFormat format);

		void setOffsets(qint64 fromOffset, qint64 toOffset);

		void run();

	protected:

		void reset();

		void detectCoderData();
		void detectCoderFile();

	private:

		void (ViCodingChain::*detectCoder)();

		ViCoder::Error mError;
		ViCodingChain::Mode mMode;
		ViSongInfo mSongInfo;

		QString mInputFilePath;
		QByteArray *mInputData;

		ViAudioFormat mInputFormat;
		ViAudioFormat mOutputFormat;
		bool mReferenceInputFormat;

		ViAbstractCoder *mInputCoder;
		ViAbstractCoder *mOutputCoder;

		ViCodingChainInput *mInput;
		ViCodingChainOutput *mOutput;

		ViCodingChainFileInput mFileInput;
		ViCodingChainFileOutput mFileOutput;

		ViCodingChainDataInput mDataInput;
		ViCodingChainDataOutput mDataOutput;

		ViCodingChainBufferInput mBufferInput;
		ViCodingChainBufferOutput mBufferOutput;

		ViCodingChainDecoder mDecoder;
		ViCodingChainEncoder mEncoder;

		qint64 mFromOffset;
		qint64 mToOffset;

};

#endif
