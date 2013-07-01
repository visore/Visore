#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <vinotifier.h>
#include <vichunk.h>
#include <viaudioobject.h>
#include <vipcmconverter.h>
#include <vifouriertransformer.h>
#include <viprocessor.h>

class ViProcessor;
class ViNoiseDetector;

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

        enum ProcessMode
        {
            All,    // All data is processed
            Noise   // Only noisy windows are processed
        };

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

        void setNoiseDetector(ViNoiseDetector *detector); //Takes ownership
        bool isNoisy();

        void setProcessMode(ViProcessor::ProcessMode mode);
        ViProcessor::ProcessMode processMode();

        ViAudio::Type type();
        ViAudio::Type type1();
        ViSampleChunk& samples();
        ViSampleChunk& samples1();
        ViAudioFormat format();
        ViAudioFormat format1();
        ViFrequencyChunk& frequencies();
        ViFrequencyChunk& frequencies1();

        void setChunkSize(int size);
        int chunkSize();
        int sampleCount();
        int sampleCount1();

	protected:

		virtual void initialize();
		virtual void execute() = 0;
		virtual void finalize();

		bool initializeProcess(ViAudioObjectPointer audioObject, ViAudio::Type type);

		ViProcessorThread& thread();
		ViAudioObjectPointer object();
		ViBufferStreamPointer readStream();

		ViSampleChunk& read();
		ViSampleChunk& read1();
		bool hasData();
		bool hasData1();

		void exit(bool exit = true);
		bool willExit();

        void samplesToFrequencies(ViSampleChunk &inputSamples, ViFrequencyChunk &outputFrequencies, ViFrequencyChunk &intermidiate);
        void frequenciesToSamples(ViFrequencyChunk &inputFrequencies, ViSampleChunk &outputSamples);

	private:

		int mChunkSize;
		ViAudioObjectPointer mObject;
		ViAudio::Type mType;
		ViBufferStreamPointer mReadStream;
		ViRawChunk mChunk;
		ViSampleChunk mSamples;
        int mSampleCount;
		ViPcmConverter<qreal> mConverter;
		ViProcessorThread mThread;
		bool mProgressEnabled;
		bool mExit;
		bool mMultiShot;
        ViNoiseDetector *mNoiseDetector;
        ViProcessor::ProcessMode mProcessMode;

        ViFourierTransformer mTransformer;
        bool mRecalculateFrequencies;
        ViFrequencyChunk mFrequencies;
        ViFrequencyChunk mTempFourierData;

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

        int sampleCount2();

        ViFrequencyChunk& frequencies2();

	private:

		ViAudio::Type mType2;
		ViBufferStreamPointer mReadStream2;
		ViRawChunk mChunk2;
		ViSampleChunk mSamples2;
        int mSampleCount2;
		ViPcmConverter<qreal> mConverter2;

        bool mRecalculateFrequencies2;
        ViFrequencyChunk mFrequencies2;
        ViFrequencyChunk mTempFourierData2;

};

class ViModifyProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		void startProgress();
		void startProgressless();

    public:

        enum ModifyMode
        {
            All,    // All data is modified
            Noise   // Only noisy windows are modified
        };

	public:

		ViModifyProcessor(bool autoWrite = true);
		virtual ~ViModifyProcessor();
        void process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2);

        void setModifyMode(ViModifyProcessor::ModifyMode mode);
        ViModifyProcessor::ModifyMode modifyMode();

	protected:

		ViAudio::Type type2();
		ViAudioFormat format2();
        bool write();
        bool write(ViSampleChunk& samples);
        bool writeFrequencies(ViFrequencyChunk &frequencies);

	private:

		bool mAutoWrite;
		ViAudio::Type mType2;
		ViBufferStreamPointer mWriteStream;
		ViRawChunk mChunk2;
		ViPcmConverter<qreal> mConverter2;
        ViModifyProcessor::ModifyMode mModifyMode;
        QQueue<QPair<bool, ViSampleChunk>> mOriginalSamples;

        int tt;

};

#endif
