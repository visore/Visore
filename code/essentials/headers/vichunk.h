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

		T* data();
		int size();

		void setData(T *data, int size);
		void resize(int size);

	private:
		T *mData;
		int mSize;
};

//Template decleration and implementation should actually be in the same file
#include "../sources/vichunk.cpp"

#endif
