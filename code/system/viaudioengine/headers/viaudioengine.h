#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include <QObject>

class ViAudioEngine : public QObject
{
    Q_OBJECT

	public:
		ViAudioEngine(QObject *parent = NULL);
};

#endif
