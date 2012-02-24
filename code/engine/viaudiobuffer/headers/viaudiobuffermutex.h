#ifndef VIAUDIOBUFFERMUTEX_H
#define VIAUDIOBUFFERMUTEX_H

#include <QSharedPointer>
#include <QMutex>

class ViAudioBufferMutex : public QMutex
{
	public:
		static ViAudioBufferMutex* instance();

	protected:
		ViAudioBufferMutex();

	private:
		static QSharedPointer<ViAudioBufferMutex> mInstance;
};

#endif
