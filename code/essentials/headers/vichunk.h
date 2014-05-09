#ifndef VICHUNK_H
#define VICHUNK_H

#include <QList>

template<typename T>
class ViChunk
{

	public:

		ViChunk();
		ViChunk(T *data, qint64 size); //Takes ownership
		ViChunk(qint64 size);
		ViChunk(const ViChunk<T> &other);
		~ViChunk();

		T* data() const;
		const T* constData() const;
		qint64 size() const;

		void setData(T *data, qint64 size); //Takes ownership
		void setSize(qint64 size);
		void resize(qint64 size);

		void insert(T value, int repeatValue = 1); // Insert value repeatValue number of times at the front
		void append(T value, int repeatValue = 1); // Append value repeatValue number of times at the end
		void append(const ViChunk<T> &other);

		void clear();

		ViChunk<T> subset(const qint64 &startIndex, const qint64 &size) const;
		bool subset(ViChunk<T> &subsetData, const qint64 &startIndex, const qint64 &size) const;
		QList<ViChunk<T>> subsets(const qint64 &size) const;

		const T& first() const;
		const T& first(qint64 offset) const;
		T& first();
		T& first(qint64 offset);

		const T& last() const;
		const T& last(qint64 offset) const;
		T& last();
		T& last(qint64 offset);

		const T& at(qint64 index) const;
		T& at(qint64 index);
		const T& operator[](const qint64 index) const;
		T& operator[] (const qint64 index);

		void operator = (const ViChunk &other);

        // Copies the data from source to destination and sets the size of source to the destination size.
        // If the destination is smaller than the source, only the maximum number of values that destination can hold will be copied.
        static void copy(const ViChunk<T> &source, ViChunk<T> &destination);

        // Copies the data from source to destination, but does not change the size.
        // If the destination is smaller than the source, only the maximum number of values that destination can hold will be copied.
        static void copyData(const ViChunk<T> &source, ViChunk<T> &destination);

	private:

		T *mData;
		qint64 mSize;

};

typedef ViChunk<double> ViDoubleChunk;
typedef ViDoubleChunk ViRealChunk;
typedef ViDoubleChunk ViSampleChunk;
typedef ViDoubleChunk ViFrequencyChunk;
typedef ViChunk<float> ViFloatChunk;
typedef ViChunk<char> ViRawChunk;

typedef QList<ViSampleChunk> ViSampleChunks;
typedef QList<ViFrequencyChunk> ViFrequencyChunks;

//Template decleration and implementation should actually be in the same file
#include "../sources/vichunk.cpp"

#endif
