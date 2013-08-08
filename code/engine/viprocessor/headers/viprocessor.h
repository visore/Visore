#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <vinotifier.h>
#include <viaudioobject.h>
#include <viaudiodata.h>
#include <viprocessor.h>
#include <vilibrary.h>
#include <QThreadPool>

class ViProcessor;
class ViNoiseDetector;

class ViProcessorThread : public QThread
{

	Q_OBJECT

	public:

		void setProcessor(ViProcessor *processor);
		void run();

	private:

		ViProcessor *mProcessor;

};

class ViProcessor : public ViNotifier, public ViSerializer, public QRunnable, public ViLibrary
{

	Q_OBJECT

	friend class ViProcessorThread;

	protected slots:

		void exit();
		void startThread();
		void executeThread();
		virtual int readNext();
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

		virtual void clear();

		void setMultiShot(bool multishot = true); //If called, stop() has to be called manually in order to finish of the process. Has to be called for audio objects where the data is continuesly added to the buffer
		bool isMultiShot();

        void stop();

        virtual void process(ViAudioObjectPointer audioObject, ViAudio::Type type);

        void setNoiseDetector(ViNoiseDetector *detector); //Takes ownership
		bool isNoisy();
		bool isNoisy(int channel);

		void setChannelMode(ViProcessor::ChannelMode mode);
		ViProcessor::ChannelMode channelMode();
        void setProcessMode(ViProcessor::ProcessMode mode);
		ViProcessor::ProcessMode processMode();

		virtual void setWindowSize(int samples);
		virtual bool setWindowFunction(QString function); // Function for the frequency spectrum

		int sampleCount();
		int channelCount();
		int usedChannelCount(); // Returns how many channels are actually used. 1 if ChannelMode == Combined, channelCount() if ChannelMode == Separated
		int currentChannel();

        ViAudio::Type type();
		inline ViAudio::Type type1(){ return type(); }

        ViAudioFormat format();
		inline ViAudioFormat format1(){ return format(); }

		ViAudioReadData& data();
		inline ViAudioReadData& data1(){ return data(); }

		ViSampleChunk& currentSamples();
		inline ViSampleChunk& currentSamples1(){ return currentSamples(); }

		ViSampleChunk& currentSamples(int channel);
		inline ViSampleChunk& currentSamples1(int channel){ return currentSamples(channel); }

		ViFrequencyChunk& currentFrequencies();
		inline ViFrequencyChunk& currentFrequencies1(){ return currentFrequencies(); }

		ViFrequencyChunk& currentFrequencies(int channel);
		inline ViFrequencyChunk& currentFrequencies1(int channel){ return currentFrequencies(channel); }

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		void run();

		virtual void initialize();
		virtual void execute(const int &channel) = 0;
		virtual void finalize();

		bool initializeProcess(ViAudioObjectPointer audioObject, ViAudio::Type type);

		void scaleSamples(qreal from, qreal to);

		ViAudioObjectPointer object();

	protected:

		ViAudioObjectPointer mObject;
		ViAudioReadData mData;

		ViProcessorThread mThread;
		QThreadPool *mThreadPool;
		QMutex mThreadMutex;

		QMutex mMutex;

		ViAudio::Type mType;
		bool mStopped;
		bool mMultiShot;
        ViNoiseDetector *mNoiseDetector;

		ViProcessor::ChannelMode mChannelMode;
        ViProcessor::ProcessMode mProcessMode;

		int mTotalChannels;
		int mCurrentChannel;

		qint64 mTotalSize;
		qint64 mProcessedSize;

		bool mHasScale;
		qreal mScaleFrom;
		qreal mScaleTo;

};

class ViDualProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		int readNext();
		void handleExit();

	public:

		ViDualProcessor();
		virtual ~ViDualProcessor();
        void process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2);

		void setWindowSize(int samples);
		bool setWindowFunction(QString function);

		ViAudio::Type type2();
		ViAudioFormat format2();
		ViAudioReadData& data2();
		ViSampleChunk& currentSamples2();
		ViSampleChunk& currentSamples2(int channel);
		ViFrequencyChunk& currentFrequencies2();
		ViFrequencyChunk& currentFrequencies2(int channel);

	private:

		ViAudio::Type mType2;
		ViAudioReadData mData2;

};

class ViModifyData
{

	private:

		enum Usage
		{
			NotAdded,
			Added,
			Written
		};

	public:

		ViModifyData();

		void setChannels(const int &channels);

		virtual void clear();

		void enqueue(const bool &noisy, const ViSampleChunk &data, const int &channel = 1);

		bool dequeueNoisy(const int &channel = 1); // If noisy, dequeues and return true. If not noisy, just returns false (manually call dequeue() to dequeue the values)
		ViSampleChunk& dequeue(const int &channel = 1);

	private:

		int mChannels;
		QQueue<QVector<bool>> mNoise;
		QQueue<QVector<ViModifyData::Usage>> mUsed;
		QQueue<QVector<ViSampleChunk>> mData;

};

class ViModifyProcessor : public ViProcessor
{

	Q_OBJECT

	protected slots:

		int readNext();

    public:

        enum ModifyMode
        {
            All,    // All data is modified
            Noise   // Only noisy windows are modified
        };

	public:

		ViModifyProcessor();
		virtual ~ViModifyProcessor();
        void process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2);

		void setWindowSize(int samples);
		bool setWindowFunction(QString function);

        void setModifyMode(ViModifyProcessor::ModifyMode mode);

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

	protected:

		ViAudio::Type type2();
		ViAudioFormat format2();
		ViAudioWriteData& data2();

		void write(ViSampleChunk &chunk);
		void write(ViSampleChunk &chunk, int channel);
		void writeFrequencies(ViFrequencyChunk &chunk);
		void writeFrequencies(ViFrequencyChunk &chunk, int channel);

	private:

		ViAudioWriteData mData2;
		ViAudioWriteData mNoiseData;
		ViAudio::Type mType2;
        ViModifyProcessor::ModifyMode mModifyMode;
		ViModifyData mOriginalData;

};

#endif
