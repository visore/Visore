#ifdef VISAMPLECHANNELER_H

#include <qmath.h>

template <typename T>
void ViSampleChanneler<T>::split(const T *input, const int &samples, const int &channels, QList<ViChunk<T>> &output)
{
    output.clear();
    QList<ViChunk<T>*> splitted = split(input, samples, channels);
    for(int i = 0; i < splitted.size(); ++i)
    {
        output.append(ViSampleChunk(splitted[i]->data(), splitted[i]->size()));
    }
}

template <typename T>
void ViSampleChanneler<T>::split(const T *input, const int &samples, const ViAudioFormat &format, QList<ViChunk<T>> &output)
{
    split(input, samples, format.channelCount(), output);
}

template <typename T>
QList<ViChunk<T>*> ViSampleChanneler<T>::split(const T *input, const int &samples, const int &channels)
{
	QList<ViChunk<T>*> result;

    int samplesPerChannel = qFloor(samples / ((qreal) channels));
	int realSamples;
	for(int i = 0; i < channels; ++i)
	{
		if(((samplesPerChannel * channels) + i - samples) < 0)
		{
			realSamples = samplesPerChannel + 1;
		}
		else
		{
			realSamples = samplesPerChannel;
		}
		result.append(new ViChunk<T>(new T[realSamples], realSamples));
	}

	int counter;
	for(int i = 0; i < result.size(); ++i)
	{
		T *data = result[i]->data();
		counter = 0;
		for(int j = i; j < samples; j += channels)
		{
			data[counter] = input[j];
			++counter;
		}
	}

	return result;
}

template <typename T>
QList<ViChunk<T>*> ViSampleChanneler<T>::split(const T *input, const int &samples, const ViAudioFormat &format)
{
	return ViSampleChanneler::split(input, samples, format.channels());
}

template <typename T>
void ViSampleChanneler<T>::merge(QList<ViChunk<T>> &input, ViChunk<T> &output)
{
    int totalSamples = 0;
    int channelCount = input.size();
    for(int i = 0; i < channelCount; ++i)
    {
        totalSamples += input[i].size();
    }
    output.resize(totalSamples);

    T *data = output.data();
    T *channelData;
    for(int i = 0; i < channelCount; ++i)
    {
        ViChunk<T> &channel = input[i];
        channelData = channel.data();
        for(int j = 0; j < channel.size(); ++j)
        {
            data[(j * channelCount) + i] = channelData[j];
        }
    }
}

template <typename T>
ViChunk<T>* ViSampleChanneler<T>::merge(QList<ViChunk<T>*> channels)
{
	int totalSamples = 0;
	int channelCount = channels.size();
	for(int i = 0; i < channelCount; ++i)
	{
		totalSamples += channels[i]->size();
	}
	T *data = new T[totalSamples];
	ViChunk<T> *channel;
	T *channelData;
	for(int i = 0; i < channelCount; ++i)
	{
		channel = channels[i];
		channelData = channel->data();
		for(int j = 0; j < channel->size(); ++j)
		{
			data[(j * channelCount) + i] = channelData[j];
		}
	}
	return new ViChunk<T>(data, totalSamples);
}

/*template <typename T>
ViChunk<T>* ViSampleChanneler<T>::merge(QList<QList<T> > &channels)
{

}*/

#endif
