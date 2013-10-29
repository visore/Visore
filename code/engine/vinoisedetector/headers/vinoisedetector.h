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
		
		ViNoiseDetector();
		ViNoiseDetector(const int &channels, const qint64 samples);
		ViNoiseDetector(const int &channels, const qint64 samples, ViProcessor::ChannelMode mode);
        ViNoiseDetector(const ViNoiseDetector &other);
		virtual ~ViNoiseDetector();

		void initialize(const int &channels, const qint64 samples);

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

        void clear();

		void setBuffers(ViBuffer *read, ViBuffer *write1, ViBuffer *write2);

		void generate();

		virtual ViNoiseDetector* clone() = 0;

	protected:

		void setOffset(const int &offset); // The first offset samples that can't be used to detect noise

	//protected:
	public:

		virtual void calculateNoise(QQueue<qreal> &samples) = 0;

		void setNoise(const qreal &value);

	private:

		void create();

    private:

		ViProcessor::ChannelMode mMode;
		int mChannel;
		ViAudioReadData *mData;
		QList<ViNoise*> mNoise;
		QList<qint64> mIndexes;

		QList<QQueue<qreal>> mCache;
		int mOffset;

		ViNoiseDetectorThread mThread;
		ViAudioReadData mRead;
		ViAudioWriteData mWrite1;
		ViAudioWriteData mWrite2;

};

#endif
