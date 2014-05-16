#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <vilibrary.h>
#include <viprocessor.h>
#include <vinoise.h>

class ViNoiseDetector;

class ViNoiseDetectorThread : public QThread
{

	public:

		void setDetector(ViNoiseDetector *detector);
		void run();

	private:

		ViNoiseDetector *mDetector;

};

class ViNoiseDetector : public ViNotifier, public ViLibrary
{

	Q_OBJECT

	friend class ViNoiseDetectorThread;

	public:

		enum Direction
		{
			Forward = 1 << 0,
			Reversed = 1 << 1,
			Bidirectional = Forward | Reversed
		};

	public:
		
		ViNoiseDetector();
		ViNoiseDetector(const int &channels, const qint64 samples);
		ViNoiseDetector(const int &channels, const qint64 samples, ViProcessor::ChannelMode mode);
        ViNoiseDetector(const ViNoiseDetector &other);
		virtual ~ViNoiseDetector();

		void setDirection(Direction direction);
		Direction direction();

		void initialize(const int &channels, const qint64 samples);

		void setThreshold(const qreal &threshold);

		void setMode(ViProcessor::ChannelMode mode);
		ViProcessor::ChannelMode mode();

		void setChannel(int channel);
		int channel();

		void setData(ViAudioReadData &data);
		bool hasData();

		bool isNoisy(int channel);
		bool isNoisy(ViAudioReadData &data);
		bool isNoisy(ViAudioReadData &data, int channel);
		bool isNoisy();

		ViNoise& noise(const int &channel = 0);

		virtual void clear();

		void setBuffers(ViBuffer *read, ViBuffer *write1, ViBuffer *write2);

		void generate();

		virtual ViNoiseDetector* clone() = 0;

		virtual void setWindowSize(const int &size){ LOG("Window size not implemented."); exit(-1); }

		virtual void setParameters(const qreal &param1){ LOG("Invalid number of parameters given: 1"); exit(-1); }
		virtual void setParameters(const qreal &param1, const qreal &param2){ LOG("Invalid number of parameters given: 2"); exit(-1); }
		virtual void setParameters(const qreal &param1, const qreal &param2, const qreal &param3){ LOG("Invalid number of parameters given: 3"); exit(-1); }
		virtual void setParameters(const qreal &param1, const qreal &param2, const qreal &param3, const qreal &param4){ LOG("Invalid number of parameters given: 4"); exit(-1); }

		virtual void setAmplification(const qreal &amp);
		virtual qreal amplification();

	protected:

		void setOffset(const int &offset); // The first offset samples that can't be used to detect noise

	//protected:
	public:

		virtual void calculateNoise(QQueue<qreal> &samples) = 0;

		void setNoise(const qreal &value);

	private:

		void create();

    private:

		Direction mDirection;
		ViProcessor::ChannelMode mMode;
		int mChannel;
		ViAudioReadData *mData;
		QList<ViNoise*> mNoise;
		QList<ViNoise*> mReverseNoise;
		bool mReverse;
		QList<qint64> mIndexes;
		QList<qint64> mReverseIndexes;
		qreal mThreshold;

		QList<QQueue<qreal>> mCache;
		int mOffset;

		ViNoiseDetectorThread mThread;
		ViAudioReadData mRead;
		ViAudioWriteData mWrite1;
		ViAudioWriteData mWrite2;

		qreal mAmplification;

};

#endif
