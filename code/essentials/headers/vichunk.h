#ifndef VICHUNK_H
#define VICHUNK_H

template<typename T>
class ViChunk
{

	public:

		ViChunk(bool autoDelete = true);
		ViChunk(T *data, int size, bool autoDelete = true);
		ViChunk(int size, bool autoDelete = true);
		ViChunk(const ViChunk &other);
		~ViChunk();

		void setAutoDelete(bool autoDelete);

		T* data() const;
		const T* constData() const;
		int size() const;

		void setData(T *data, int size);
		void setSize(int size);
		void resize(int size);

		void insert(T value, int repeatValue = 1); // Insert value repeatValue number of times at the front
		void append(T value, int repeatValue = 1); // Append value repeatValue number of times at the end

		void clear();

		const T& at(int index) const;
		T& at(int index);
		const T& operator[](const int index) const;
		T& operator[] (const int index);

		void operator= (const ViChunk &other);

		static void copy(const ViChunk<T> &source, ViChunk<T> &destination);

	private:

		bool mAutoDelete;
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
