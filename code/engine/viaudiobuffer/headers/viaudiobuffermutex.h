#ifndef VIAUDIOBUFFERMUTEX_H
#define VIAUDIOBUFFERMUTEX_H

#include <QSharedPointer>
#include <QMutex>

class ViAudioBufferMutex : public QMutex
{
	public:
		static ViAudioBufferMutex* readInstance();
		static ViAudioBufferMutex* writeInstance();

	protected:
		ViAudioBufferMutex();

	private:
		static QSharedPointer<ViAudioBufferMutex> mReadInstance;
		static QSharedPointer<ViAudioBufferMutex> mWriteInstance;
};

#endif
