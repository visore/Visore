#include "viid.h"
#include <QDateTime>
#include <QtGlobal>

#define ID_LENGTH 16

ViId::ViId()
{
	mId = generate();
}

ViId::~ViId()
{
    ViIdManager::release(mId);
}

QString ViId::id() const
{
	return mId;
}

bool ViId::setId(QString id)
{
	if(id.length() != ID_LENGTH)
	{
		return false;
	}
	double value = id.toDouble();
	if(QString::number(value, 'f', 0) != id)
	{
		return false;
	}
	mId = id;
	return true;
}

bool ViId::operator ==(const ViId &other) const
{
	return (mId == other.mId);
}

bool ViId::operator !=(const ViId &other) const
{
	return (mId != other.mId);
}

QString ViId::generate()
{
	return ViIdManager::generate();
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
	qint64 current = QDateTime::currentDateTime().toMSecsSinceEpoch();
	qsrand(current);
	QString random = QString::number(current);
	while(random.length() < ID_LENGTH)
	{
		random += QString::number(qrand());
	}
	random.resize(ID_LENGTH);
	return random;
}
