#include "viid.h"
#include <QTime>
#include <QtGlobal>

#define RANDOM_LENGTH 16

ViId::ViId()
{
	mId = ViIdManager::generate();
}

ViId::~ViId()
{
	ViIdManager::release(mId);
}

QString ViId::id() const
{
	return mId;
}

bool ViId::operator ==(const ViId &other) const
{
	return (mId == other.mId);
}

bool ViId::operator !=(const ViId &other) const
{
	return (mId != other.mId);
}

QSharedPointer<ViIdManager> ViIdManager::mInstance;

ViIdManager* ViIdManager::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViIdManager>(new ViIdManager());
	}
	return mInstance.data();
}

QString ViIdManager::generate()
{
	ViIdManager *manager = ViIdManager::instance();
	QString id;
	do
	{
		id = random();
	}
	while(manager->mIds.contains(id));
	manager->mIds.append(id);
	return id;
}

void ViIdManager::release(QString id)
{
	ViIdManager *manager = ViIdManager::instance();
	manager->mIds.removeOne(id);
}

QString ViIdManager::random()
{
	qsrand(QTime::currentTime().msec());
	QString random = "";
	while(random.length() < RANDOM_LENGTH)
	{
		random += QString::number(qrand());
	}
	random.resize(RANDOM_LENGTH);
	return random;
}
