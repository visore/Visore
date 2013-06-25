#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <vinotifier.h>
#include <vichunk.h>
#include <viaudioobject.h>
#include <vipcmconverter.h>

class ViProcessor;

class ViProcessorThread : public QThread
{

	public:

		ViProcessorThread();
		void setProcessor(ViProcessor *processor);
		void run();

	private:

		ViProcessor *mProcessor;
};

class ViProcessor : public ViNotifier, public ViSerializer
{

	Q_OBJECT

	friend class ViProcessorThread;

	protected slots:

		void startThread();
		virtual void startProgress();
		virtual void startProgressless();

	public:

        ViProcessor();

		void enableProgress(bool enable = true);
		void disableProgress(bool disable = true);
		bool isProgressEnabled();

		void setMultiShot(bool multishot = true); //If called, stop() has to be called manually in order to finish of the process. Has to be called for audio objects where the data is continuesly added to the buffer
		bool isMultiShot();

        void stop();

        virtual ~ViProcessor();
        virtual void process(ViAudioObjectPointer audioObject, ViAudio::Type type);

	protected:

		virtual void initialize();
		virtual void execute() = 0;
		virtual void finalize();

		bool initializeProcess(ViAudioObjectPointer audioObject, ViAudio::Type type);

		ViProcessorThread& thread();
		ViAudioObjectPointer object();
		ViBufferStreamPointer readStream();

		ViAudio::Type type();
		ViAudio::Type type1();
		ViSampleChunk& read();
		ViSampleChunk& read1();
		ViSampleChunk& samples();
		ViSampleChunk& samples1();
		bool hasData();
		bool hasData1();
		ViAudioFormat format();
		ViAudioFormat format1();

		void exit(bool exit = true);
		bool willExit();

		void setChunkSize(int size);
		int chunkSize();
		int sampleCount();

	private:

		int mChunkSize;
		ViAudioObjectPointer mObject;
		ViAudio::Type mType;
		ViBufferStreamPointer mReadStream;
		ViRawChunk mChunk;
		ViSampleChunk mSamples;
		ViPcmConverter<qreal> mConverter;
		ViProcessorThread mThread;
		bool mProgressEnabled;
		bool mExit;
		bool mMultiShot;

};

class ViDualProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		void startProgress();
		void startProgressless();

	public:

		ViDualProcessor();
		virtual ~ViDualProcessor();
        void process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2);

	protected:

		ViAudio::Type type2();
		ViSampleChunk& read2();
		ViSampleChunk& samples2();
		bool hasData2();
		ViAudioFormat format2();

	private:

		ViAudio::Type mType2;
		ViBufferStreamPointer mReadStream2;
		ViRawChunk mChunk2;
		ViSampleChunk mSamples2;
		ViPcmConverter<qreal> mConverter2;

};

class ViModifyProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		void startProgress();
		void startProgressless();

	public:

		ViModifyProcessor(bool autoWrite = true);
		virtual ~ViModifyProcessor();
        void process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2);

	protected:

		ViAudio::Type type2();
		ViAudioFormat format2();
		void write();
		void write(ViSampleChunk& samples);

	private:

		bool mAutoWrite;
		ViAudio::Type mType2;
		ViBufferStreamPointer mWriteStream;
		ViRawChunk mChunk2;
		ViPcmConverter<qreal> mConverter2;

};

#endif
