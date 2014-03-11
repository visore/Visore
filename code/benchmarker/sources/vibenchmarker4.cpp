#include <vibenchmarker4.h>
#include <viaudiodata.h>
#include <vilogger.h>
#include <vinearestneighbournoisedetector.h>
#include <vimahalanobisnoisedetector.h>
#include <vizscorenoisedetector.h>
#include <vimadnoisedetector.h>
#include <vipredictionnoisedetector.h>
#include <vifouriernoisedetector.h>
#include <QTextStream>

#include <iomanip>

#define AI_NOISE_SEPERATION 512
#define AI_NOISE_START_LENGTH 1
#define AI_NOISE_END_LENGTH 30

ViBenchMarker4::ViBenchMarker4()
{
    TYPE = "classical";
    mCurrentObject = ViAudioObject::create();
}

ViBenchMarker4::~ViBenchMarker4()
{

}

void ViBenchMarker4::benchmark()
{
    QDir dir("/home/visore/Visore Projects/Files/");
	QStringList files = dir.entryList(QDir::Files);
	for(int i = 0; i < files.size(); ++i)
	{
		mFiles.enqueue(dir.absoluteFilePath(files[i]));
	}

	nextFile();
}

void ViBenchMarker4::nextFile()
{
	if(mFiles.isEmpty())
	{
		quit();
	}
	else
    {
		mCurrentFile = mFiles.dequeue();
		mCurrentObject->clearBuffers();
        mCurrentObject->clearSpectrums();
		mCurrentObject->setFilePath(ViAudio::Target, mCurrentFile);
		QObject::connect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
		mCurrentObject->decode();
	}
}

void ViBenchMarker4::process1()
{
	QObject::disconnect(mCurrentObject.data(), SIGNAL(decoded()), this, SLOT(process1()));
   // generateNoise();
    QObject::connect(mCurrentObject.data(), SIGNAL(spectrumed()), this, SLOT(process2()));
    mCurrentObject->generateSpectrum(ViAudio::Target, 2048, "Hamming");
}

void ViBenchMarker4::process2()
{
    QString name =  mCurrentObject->filePath(ViAudio::Target).replace("/home/visore/Visore Projects/Files/", "");
    QObject::disconnect(mCurrentObject.data(), SIGNAL(spectrumed()), this, SLOT(process2()));
    mOutputFile.setFileName("/home/visore/Visore Projects/Results/"+name+".txt");
    mOutputFile.open(QIODevice::WriteOnly);
    mOutputStream.setDevice(&mOutputFile);
    mOutputStream << name<< "\n";

    ViRealSpectrum *s = mCurrentObject->spectrum(ViAudio::Target);
    for(int i = 0; i < s->size(); ++i)
    {
        mOutputStream << s->at(i).polar().decibel().real() << "\n";
    }

    mOutputFile.close();
    nextFile();
}


void ViBenchMarker4::generateNoise()
{
    mNoiseType = 0;
    mNoiseLength = AI_NOISE_START_LENGTH;
    mNoiseCount = AI_NOISE_END_LENGTH - mNoiseLength; // Linear add less noise as the size increases

    ViBuffer *buffer = mCurrentObject->buffer(ViAudio::Target);
    ViAudioReadData data(buffer);
    mCurrentObject->buffer(ViAudio::Corrupted)->setFormat(mCurrentObject->buffer(ViAudio::Target)->format());
    ViAudioWriteData data2(mCurrentObject->buffer(ViAudio::Corrupted));

    mNoise1.clear();
    mNoise2.clear();
    mNoise1.resize(data.bufferSamples() / 2);
    mNoise2.resize(data.bufferSamples() / 2);
    qint64 offset1 = 0, offset2 = 0;
    for(qint64 i = 0; i < mNoise1.size(); ++i)
    {
        mNoise1[i] = 0;
        mNoise2[i] = 0;
    }

    while(data.hasData())
    {
        data.read();

        ViSampleChunk &c1 = data.splitSamples(0);
        int index = 0;
        while(index < c1.size())
        {
            index += AI_NOISE_SEPERATION;
            int length = addNoise(c1, index);

            offset1 += AI_NOISE_SEPERATION;
            if(offset1+length <= mNoise1.size())
            {
                for(int i = 0; i < length; ++i) mNoise1[offset1+i] = 1;
                offset1 += AI_NOISE_SEPERATION;
            }

            index += AI_NOISE_SEPERATION;
        }
        data2.enqueueSplitSamples(c1, 0);

        ViSampleChunk &c2 = data.splitSamples(1);
        index = 0;
        while(index < c2.size())
        {
            index += AI_NOISE_SEPERATION;
            int length = addNoise(c2, index);

            offset2 += AI_NOISE_SEPERATION;
            if(offset2+length <= mNoise2.size())
            {
                for(int i = 0; i < length; ++i) mNoise1[offset2+i] = 1;
                offset2 += AI_NOISE_SEPERATION;
            }

            index += AI_NOISE_SEPERATION;
        }
        data2.enqueueSplitSamples(c2, 1);
    }

    //cout << "Artifical noise generated" << endl;
}

int ViBenchMarker4::addNoise(ViSampleChunk &s, int offset)
{
    if(mNoiseLength > s.size()) return 0;

    if(mNoiseType == 0)
    {
        addNoise1(s, offset, mNoiseLength);
    }
    else if(mNoiseType == 1)
    {
        addNoise2(s, offset, mNoiseLength);
    }
    else if(mNoiseType == 2)
    {
        addNoise3(s, offset, mNoiseLength);
    }
    else
    {
        cout << "INVALID NOISE LENGTH +++++++++++++" << endl;
    }

    int oldLength = mNoiseLength;

    --mNoiseCount;
    if(mNoiseCount == 0)
    {
        ++mNoiseType;
        if(mNoiseType > 2)
        {
            mNoiseType = 0;
            ++mNoiseLength;
            if(mNoiseLength > AI_NOISE_END_LENGTH)
            {
                mNoiseLength = AI_NOISE_START_LENGTH;
            }
        }
        mNoiseCount = AI_NOISE_END_LENGTH - mNoiseLength;
    }

    return oldLength;
}

void ViBenchMarker4::addNoise1(ViSampleChunk &s, int offset, int length)
{
    qreal startval = 0.95;
    qreal val = startval;
    bool plus = true;
    for(int i = 0; i < length; ++i)
    {
        s[offset + i] = val;

        if(plus) val += 0.005;
        else val -= 0.005;

        if(val > 1)
        {
            val = 1;
            plus = false;
        }
        else if(val < startval)
        {
            val = startval;
            plus = true;
        }
    }
}

void ViBenchMarker4::addNoise2(ViSampleChunk &s, int offset, int length)
{
    qreal startval = -0.95;
    qreal val = startval;
    bool minus = true;
    for(int i = 0; i < length; ++i)
    {
        s[offset + i] = val;

        if(minus) val -= 0.005;
        else val += 0.005;

        if(val < -1)
        {
            val = -1;
            minus = false;
        }
        else if(val > startval)
        {
            val = startval;
            minus = true;
        }
    }
}

void ViBenchMarker4::addNoise3(ViSampleChunk &s, int offset, int length)
{
    qreal zag = 1;
    bool minus = true;
    for(int i = 0; i < length; ++i)
    {
        s[offset + i] = zag;

        if(zag == -1) minus = false;
        else if (zag == 1) minus = true;

        if(minus) zag -= 0.2;
        else zag += 0.2;
    }
}

void ViBenchMarker4::quit()
{
    cout << "QUIT!" << endl;
    int x = 0;
    int y = 1 / x;
}
