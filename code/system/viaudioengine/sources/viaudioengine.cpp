#include "viaudioengine.h"

ViAudioEngine::ViAudioEngine(QObject *parent)
	: QObject(parent)
{
	mAudioInput = NULL;
	mAudioOutput = NULL;
	mPlayPosition = 0;
	mSpectrumBufferLength = 0;
	mDataLength = 0;
	mInputAudioType = ViAudioEngine::None;
	mOutputAudioType = ViAudioEngine::None;

	//Test
	QAudioFormat format;
	format.setFrequency(8000);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);
	mInputAudioType = ViAudioEngine::File;
	initializeInput("/tmp/test.raw", format);

	mOutputAudioType = ViAudioEngine::Stream;
	initializeOutput(QAudioDeviceInfo::defaultOutputDevice(), format);
	mAudioOutput->start();
}

ViAudioEngine::~ViAudioEngine()
{
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
		mAudioInput = NULL;
	}
	if(mAudioOutput != NULL)
	{
		delete mAudioOutput;
		ViObject::disconnect(mAudioOutput, SIGNAL(notify()), this, SLOT(receiveNotification()));
		mAudioOutput = NULL;
	}
}

void ViAudioEngine::initializeInput(QString filePath, QAudioFormat format)
{
	if(mInputAudioType == ViAudioEngine::Stream)
	{
		//mAudioInput = new ViStreamInput(device, deviceInfo, format);
	}
	else if(mInputAudioType == ViAudioEngine::File)
	{
		mAudioInput = new ViFileInput(filePath, format);
	}
}

void ViAudioEngine::initializeOutput(QAudioDeviceInfo deviceInfo, QAudioFormat format)
{
	if(mOutputAudioType == ViAudioEngine::Stream)
	{
		mAudioOutput = new ViStreamOutput(mAudioInput, deviceInfo, format);
		ViObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(receiveNotification()));
	}
	else if(mOutputAudioType == ViAudioEngine::File)
	{

	}
}

void ViAudioEngine::setPlayPosition(qint64 position, bool forceEmit)
{
	const bool changed = (mPlayPosition != position);
	mPlayPosition = position;
	if (changed || forceEmit)
	{
		emit playPositionChanged(mPlayPosition);
	}
}

void ViAudioEngine::setFormat(QAudioFormat format)
{
	const bool changed = (format != mAudioOutput->format());	
	mAudioOutput->setFormat(format);
	mSpectrumBufferLength = cSpectrumLengthSamples * (format.sampleSize() / 8) * format.channels();
	if(changed)
	{
		emit formatChanged(format);
	}
}

void ViAudioEngine::receiveNotification()
{
	const qint64 playPosition = mAudioOutput->audioLength(mAudioOutput->secondsProcessed());
	setPlayPosition(qMin(mAudioOutput->bufferLength(), playPosition));
	const qint64 levelPosition = playPosition - mAudioOutput->levelBufferLength();
	const qint64 spectrumPosition = playPosition - mSpectrumBufferLength;
	
	if(mInputAudioType == ViAudioEngine::File)
	{
		if(levelPosition > mAudioOutput->bufferPosition() || spectrumPosition > mAudioOutput->bufferPosition() || qMax(mAudioOutput->levelBufferLength(), mSpectrumBufferLength) > mDataLength)
		{
			mAudioOutput->setBufferPosition(0);
			mDataLength = 0;
			// Data needs to be read into m_buffer in order to be analysed
			const qint64 readPos = qMax(qint64(0), qMin(levelPosition, spectrumPosition));
			const qint64 readEnd = qMin(mAudioInput->size(), qMax(levelPosition + mAudioOutput->levelBufferLength(), spectrumPosition + mSpectrumBufferLength));
			const qint64 readLen = readEnd - readPos + mAudioOutput->audioLength(cWaveformWindowDuration);
		}
	}
            
           /* if (m_file) {
                if (levelPosition > m_bufferPosition ||
                    spectrumPosition > m_bufferPosition ||
                    qMax(m_levelBufferLength, m_spectrumBufferLength) > m_dataLength) {
                    m_bufferPosition = 0;
                    m_dataLength = 0;
                    // Data needs to be read into m_buffer in order to be analysed
                    const qint64 readPos = qMax(qint64(0), qMin(levelPosition, spectrumPosition));
                    const qint64 readEnd = qMin(m_analysisFile->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));
                    const qint64 readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
                    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
                        m_buffer.resize(readLen);
                        m_bufferPosition = readPos;
                        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
                    }
                    emit bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
                }
            } else {
                if (playPosition >= m_dataLength)
                    stopPlayback();
            }*/
         /*   if (levelPosition >= 0 && levelPosition + m_levelBufferLength < m_bufferPosition + m_dataLength)
                calculateLevel(levelPosition, m_levelBufferLength);
            if (spectrumPosition >= 0 && spectrumPosition + m_spectrumBufferLength < m_bufferPosition + m_dataLength)
                calculateSpectrum(spectrumPosition);
        */
}


void ViAudioEngine::calculateSpectrum(qint64 position)
{

   /* Q_ASSERT(position + m_spectrumBufferLength <= m_bufferPosition + m_dataLength);
    Q_ASSERT(0 == m_spectrumBufferLength % 2); // constraint of FFT algorithm

    // QThread::currentThread is marked 'for internal use only', but
    // we're only using it for debug output here, so it's probably OK :)
    ENGINE_DEBUG << "Engine::calculateSpectrum" << QThread::currentThread()
                 << "count" << m_count << "pos" << position << "len" << m_spectrumBufferLength
                 << "spectrumAnalyser.isReady" << m_spectrumAnalyser.isReady();

    if(m_spectrumAnalyser.isReady()) {
        m_spectrumBuffer = QByteArray::fromRawData(m_buffer.constData() + position - m_bufferPosition,
                                                   m_spectrumBufferLength);
        m_spectrumPosition = position;
        m_spectrumAnalyser.calculate(m_spectrumBuffer, m_format);*/
}
