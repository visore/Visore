#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <vilibrary.h>
#include <viprocessor.h>
#include <vinoise.h>

class ViNoiseDetector : public ViLibrary
{

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

        virtual ViNoiseDetector* clone() = 0;

	//protected:
	public:

		virtual void calculateNoise(const ViSampleChunk &samples) = 0;
		void setNoise(const qreal &value);

    private:

		ViProcessor::ChannelMode mMode;
		int mChannel;
		ViAudioReadData *mData;
		QList<ViNoise*> mNoise;
		QList<qint64> mIndexes;

};

#endif
