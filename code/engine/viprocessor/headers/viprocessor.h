#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <vinotifier.h>
#include <viaudioobject.h>
#include <viaudiodata.h>
#include <viprocessor.h>
#include <QThreadPool>

class ViNoiseDetector;

class ViProcessor : public ViNotifier, public ViSerializer, public QRunnable
{

	Q_OBJECT

	friend class ViProcessorThread;

	protected slots:

		void startThread();
		void executeNext();
		virtual bool readNext();
		virtual void handleExit();

    public:

		enum ChannelMode
		{
			Combined,
			Separated
		};

        enum ProcessMode
        {
            All,    // All data is processed
            Noise   // Only noisy windows are processed
        };

	public:

		ViProcessor(ViProcessor::ChannelMode mode = ViProcessor::Separated);
		virtual ~ViProcessor();

		void setMultiShot(bool multishot = true); //If called, stop() has to be called manually in order to finish of the process. Has to be called for audio objects where the data is continuesly added to the buffer
		bool isMultiShot();

        void stop();

        virtual void process(ViAudioObjectPointer audioObject, ViAudio::Type type);

        void setNoiseDetector(ViNoiseDetector *detector); //Takes ownership
        bool isNoisy();

		void setChannelMode(ViProcessor::ChannelMode mode);
        void setProcessMode(ViProcessor::ProcessMode mode);

        ViAudio::Type type();
		inline ViAudio::Type type1(){ return type(); }

        ViAudioFormat format();
		inline ViAudioFormat format1(){ return format(); }

		ViAudioReadData& data();
		inline ViAudioReadData& data1(){ return data(); }

		ViSampleChunk& currentSamples();
		inline ViSampleChunk& currentSamples1(){ return currentSamples(); }

		ViFrequencyChunk& currentFrequencies();
		inline ViFrequencyChunk& currentFrequencies1(){ return currentFrequencies(); }

	protected:

		void run();

		virtual void initialize();
		virtual void execute(int channel) = 0;
		virtual void finalize();

		bool initializeProcess(ViAudioObjectPointer audioObject, ViAudio::Type type);

		ViAudioObjectPointer object();
		void exit(bool exit = true);

	protected:

		ViAudioObjectPointer mObject;
		ViAudioReadData mData;

		QThreadPool *mThreadPool;
		QMutex mThreadMutex;

		QMutex mMutex;

		ViAudio::Type mType;
		bool mExit;
		bool mMultiShot;
        ViNoiseDetector *mNoiseDetector;

		ViProcessor::ChannelMode mChannelMode;
        ViProcessor::ProcessMode mProcessMode;

		int mTotalChannels;
		int mCurrentChannel;

};

class ViDualProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		bool readNext();
		void handleExit();

	public:

		ViDualProcessor();
		virtual ~ViDualProcessor();
        void process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2);

	protected:

		ViAudio::Type type2();
		ViAudioFormat format2();
		ViAudioReadData& data2();

	private:

		ViAudio::Type mType2;
		ViAudioReadData mData2;

};

class ViModifyProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		bool readNext();

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

	protected:

		ViAudio::Type type2();
		ViAudioFormat format2();
		ViAudioWriteData& data2();

	private:

		ViAudioWriteData mData2;
		bool mAutoWrite;
		ViAudio::Type mType2;
        ViModifyProcessor::ModifyMode mModifyMode;
        QQueue<QPair<bool, ViSampleChunk>> mOriginalSamples;

};

#endif
