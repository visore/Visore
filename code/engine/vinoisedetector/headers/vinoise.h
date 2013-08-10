#ifndef VINOISE_H
#define VINOISE_H

#include <QVector>
#include <vichunk.h>

class ViNoise
{

	public:

		ViNoise(const int &size = 0);
		ViNoise(const ViNoise &other);
		virtual ~ViNoise();

		void resize(const int &size);

		void clear();
		int size() const;

		qreal snr() const; // Signal-to-noise ratio
		bool isNoisy() const; // If the SNR falls below a certain threshold

		// This function will try to minimize the noise into blocks
		// Eg: 0 1 1 1 0 1 1 0 -> 0 1 1 1 1 1 1 0
		// This depends on the minimize threshold
		void minimize();

		ViSampleChunk& data();
		ViSampleChunk& mask();

		void set(const int &index, const qreal &value);
		void set(const int &index, const ViNoise &other);

		bool at(const int &index) const;
		bool operator [] (const int &index) const;

	private:

		ViSampleChunk mMask;
		ViSampleChunk mData;

};

#endif
