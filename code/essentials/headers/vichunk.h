#ifndef VICHUNK_H
#define VICHUNK_H

template<typename T>
class ViChunk
{

	public:

		ViChunk();
		ViChunk(T *data, int size);
		ViChunk(int size);
		~ViChunk();

		T* data() const;
		int size() const;

		void setData(T *data, int size);
		void setSize(int size);
		void resize(int size);

		const T& at(int index) const;
		T& at(int index);
		const T& operator[](const int index) const;
		T& operator[] (const int index);

		static void copy(const ViChunk<T> *source, ViChunk<T> *destination);

	private:

		T *mData;
		int mSize;

};

typedef ViChunk<double> ViDoubleChunk;
typedef ViDoubleChunk ViRealChunk;
typedef ViDoubleChunk ViSampleChunk;
typedef ViChunk<float> ViFloatChunk;
typedef ViChunk<char> ViRawChunk;

//Template decleration and implementation should actually be in the same file
#include "../sources/vichunk.cpp"

#endif
