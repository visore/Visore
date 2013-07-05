#ifndef VISCALER_H
#define VISCALER_H

#include <vichunk.h>

template <typename T>
class ViScaler
{

	public:

		static T scale(const T &value, const T &fromMinimum, const T &fromMaximum, const T &toMinimum, const T &toMaximum);
		static void scale(const ViChunk<T> &input, ViChunk<T> &output, const T &fromMinimum, const T &fromMaximum, const T &toMinimum, const T &toMaximum);
		static void scale(ViChunk<T> &values, const T &fromMinimum, const T &fromMaximum, const T &toMinimum, const T &toMaximum);

};

#include <viscaler.cpp>

#endif
