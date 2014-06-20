#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <vinotifier.h>
#include <vibuffer.h>
#include <viclassificationerror.h>
#include <QVector>

class ViNoiseDetector : public ViNotifier
{

	public:

		ViNoiseDetector();
		ViNoiseDetector(const ViNoiseDetector &other);
		~ViNoiseDetector();

		void detect(ViBuffer *corrupted, ViBuffer *noiseMap);

		void mask(ViBuffer *noiseMap, ViBuffer *noiseMask, const qreal &threshold);

		ViClassificationErrorCollection error(ViBuffer *noiseMask, ViBuffer *sizeMask);
		ViClassificationErrorCollection error(ViBuffer *noiseMap, ViBuffer *sizeMask, const qreal &threshold);

		virtual bool validParameters() const;

		void setParameter(const QString &name, const qreal &value);
		void setParameter(const int &index, const qreal &value);
		bool hasParameter(const QString &name) const;
		int parameterCount() const;
		QStringList parameters(const bool &allCaps = true) const;
		QString parameter(const int &index, const bool &allCaps = true) const;

	protected:

		qreal parameter(const QString &name);
		void addParameter(const QString &name);
		void setOffset(const int &offset);

		virtual void initialize(); // Use this to set all the parameters
		virtual int detect(const QVector<qreal> &samples, QVector<qreal> &noise) = 0; // returns the number of samples to remove.

	private:

		int mOffset;
		QMap<QString, qreal> mParameters;

};

#endif




/*#ifndef VINOISEDETECTOR_H
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

		virtual void setParameter(const int &number, const qreal &value);
		virtual void setParameters(const qreal &param1);
		virtual void setParameters(const qreal &param1, const qreal &param2);
		virtual void setParameters(const qreal &param1, const qreal &param2, const qreal &param3);
		virtual void setParameters(const qreal &param1, const qreal &param2, const qreal &param3, const qreal &param4);

		virtual bool validParameters();
		virtual bool hasParameter(const QString &name);

		QString parameterName(const int &number, const bool &allCaps = true);
		int parameterCount();

		virtual void setAmplification(const qreal &amp);
		virtual qreal amplification();

	protected:

		void addParameterName(const QString &name);
		void setOffset(const int &offset); // The first offset samples that can't be used to detect noise

	protected:

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

		QList<QString> mParameterNames;

};

#endif
*/
