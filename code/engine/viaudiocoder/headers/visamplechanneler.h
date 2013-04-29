#ifndef VISAMPLECHANNELER_H
#define VISAMPLECHANNELER_H

#include <QList>
#include <viaudioformat.h>
#include <vichunk.h>

template <typename T>
class ViSampleChanneler
{

	public:

		static QList<ViChunk<T> > split(const T *input, const int &samples, const int &channels);
		static QList<ViChunk<T> > split(const T *input, const int &samples, const ViAudioFormat &format);

		static ViChunk<T> merge(QList<ViChunk<T> > channels);
		static ViChunk<T> merge(QList<QList<T>  >channels);

};

#include <visamplechanneler.cpp>

#endif
