#ifndef VISAMPLEARRAY_H
#define VISAMPLEARRAY_H

#include <vicoder.h>

class ViSampleArray
{

	public:

		ViSampleArray();
		ViSampleArray(const ViSampleArray &other);
		ViSampleArray(const QByteArray &other);
		ViSampleArray(char *data, int size, int samples = 0);
		ViSampleArray(qbyte8u *data, int size, int samples = 0);
		ViSampleArray(qbyte16u *data, int size, int samples = 0);
		ViSampleArray(qbyte32u *data, int size, int samples = 0);
		ViSampleArray(qbyte8s *data, int size, int samples = 0);
		ViSampleArray(qbyte16s *data, int size, int samples = 0);
		ViSampleArray(qbyte32s *data, int size, int samples = 0);
		ViSampleArray(qfloat *data, int size, int samples = 0);
		ViSampleArray(qreal *data, int size, int samples = 0);
		~ViSampleArray();

		void setData(char *data, int size);
		void setData(qbyte8u *data, int size);
		void setData(qbyte16u *data, int size);
		void setData(qbyte32u *data, int size);
		void setData(qbyte8s *data, int size);
		void setData(qbyte16s *data, int size);
		void setData(qbyte32s *data, int size);
		void setData(qfloat *data, int size);
		void setData(qreal *data, int size);

		void setData(char *data, int size, int samples);
		void setData(qbyte8u *data, int size, int samples);
		void setData(qbyte16u *data, int size, int samples);
		void setData(qbyte32u *data, int size, int samples);
		void setData(qbyte8s *data, int size, int samples);
		void setData(qbyte16s *data, int size, int samples);
		void setData(qbyte32s *data, int size, int samples);
		void setData(qfloat *data, int size, int samples);
		void setData(qreal *data, int size, int samples);

		int samples();
		void setSamples(int samples);

		int size();
		void* data();
		char* charData();

		void clear();
		void clearData();

	private:

		int mSamples;
		int mSize;
		void *mData;

};

#endif
