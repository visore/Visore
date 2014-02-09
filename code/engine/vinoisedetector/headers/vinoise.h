#ifndef VINOISE_H
#define VINOISE_H

#include <QVector>
#include <vichunk.h>

class ViNoise
{

	public:

		// ftp://cs.uku.fi/franti/papers/Hautamaki/P2.pdf
		enum MaskType
		{
			Direct,
			Mean,
			Maximum
		};

	public:

		ViNoise(ViSampleChunk *data, ViSampleChunk *mask, const qreal &threshold);
		ViNoise(const int &size = 0);
		ViNoise(const ViNoise &other);
		virtual ~ViNoise();

		void setThreshold(const qreal &threshold);

		void resize(const int &size);

		void clear();
		int size() const;

		qreal snr() const; // Signal-to-noise ratio
		bool isNoisy() const; // If the SNR falls below a certain threshold

		// This function will try to minimize the noise into blocks
		// Eg: 0 1 1 1 0 1 1 0 -> 0 1 1 1 1 1 1 0
		// This depends on the minimize threshold
		void minimize();

		ViSampleChunk* data();
		ViSampleChunk* mask();
		void generateMask(const MaskType &type = Direct);

		void set(const int &index, const qreal &value);
		void set(const int &index, const ViNoise &other);

		qreal get(const int &index);

		const bool& at(const int &index) const;
		const bool& operator [] (const int &index) const;

	private:

		void generateDirectMask();
		void generateMeanMask();
		void generateMaximumMask();

	private:

		qreal mThreshold;
		ViSampleChunk *mMask;
		ViSampleChunk *mData;

};

#endif
