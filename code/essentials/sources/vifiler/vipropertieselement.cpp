#include "vipropertieselement.h"

ViPropertiesElement::ViPropertiesElement(QDomDocument *document)
	: ViFilerElement(document)
{
	mDescription = "Visore Project File";
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();
	mCreatedDateTime = QDateTime::currentDateTime();
	mEditedDateTime  = QDateTime::currentDateTime();
	mIsNull = false;
	fromDom();
}

ViVersion ViPropertiesElement::createdVersion()
{
	return mCreatedVersion;
}

ViVersion ViPropertiesElement::editedVersion()
{
	return mEditedVersion;
}

QDateTime ViPropertiesElement::createdDateTime()
{
	return mCreatedDateTime;
}

QDateTime ViPropertiesElement::editedDateTime()
{
	return mEditedDateTime;
}

void ViPropertiesElement::setCreatedVersion(ViVersion created)
{
	mCreatedVersion = created;
}

void ViPropertiesElement::setEditedVersion(ViVersion edited)
{
	mEditedVersion  = edited;
}

void ViPropertiesElement::setCreatedDateTime(QDateTime created)
{
	mCreatedDateTime = created;
}

void ViPropertiesElement::setEditedDateTime(QDateTime edited)
{
	mEditedDateTime  = edited;
}

bool ViPropertiesElement::fromDom()
{
	QDomElement properties = mDocument->firstChildElement("Properties");
	if(properties.isNull())
	{
		mIsNull = true;
	}
	else
	{
		mDescription = properties.firstChildElement("Description").text();
		QDomElement created = properties.firstChildElement("Created");
		if(created.isNull())
		{
			mIsNull = true;
		}
		else
		{
			QDomElement element = created.firstChildElement("Date");
			if(element.isNull())
			{
				mIsNull = true;
			}
			else
			{
				mCreatedDateTime.setDate(QDate(element.firstChildElement("Year").text().toInt(), element.firstChildElement("Month").text().toInt(), element.firstChildElement("Day").text().toInt()));
			}
			element = created.firstChildElement("Time");
			if(element.isNull())
			{
				mIsNull = true;
			}
			else
			{
				mCreatedDateTime.setTime(QTime(element.firstChildElement("Hour").text().toInt(), element.firstChildElement("Minute").text().toInt(), element.firstChildElement("Second").text().toInt()));
			}
			element = created.firstChildElement("Version");
			if(element.isNull())
			{
				mIsNull = true;
			}
			else
			{
				mCreatedVersion = ViVersion(element.firstChildElement("Major").text().toInt(), element.firstChildElement("Minor").text().toInt(), element.firstChildElement("Patch").text().toInt());
			}
		}

		QDomElement edited = properties.firstChildElement("Edited");
		if(edited.isNull())
		{
			mIsNull = true;
		}
		else
		{
			QDomElement element = edited.firstChildElement("Date");
			if(element.isNull())
			{
				mIsNull = true;
			}
			else
			{
				mEditedDateTime.setDate(QDate(element.firstChildElement("Year").text().toInt(), element.firstChildElement("Month").text().toInt(), element.firstChildElement("Day").text().toInt()));
			}
			element = edited.firstChildElement("Time");
			if(element.isNull())
			{
				mIsNull = true;
			}
			else
			{
				mEditedDateTime.setTime(QTime(element.firstChildElement("Hour").text().toInt(), element.firstChildElement("Minute").text().toInt(), element.firstChildElement("Second").text().toInt()));
			}
			element = edited.firstChildElement("Version");
			if(element.isNull())
			{
				mIsNull = true;
			}
			else
			{
				mEditedVersion = ViVersion(element.firstChildElement("Major").text().toInt(), element.firstChildElement("Minor").text().toInt(), element.firstChildElement("Patch").text().toInt());
			}
		}
	}
	return isNull();
}

QDomElement ViPropertiesElement::toDom()
{
	QDomElement createdDate = createNode("Date");
	createdDate.appendChild(createNode("Year", QString::number(mCreatedDateTime.date().year())));
	createdDate.appendChild(createNode("Month", QString::number(mCreatedDateTime.date().month())));
	createdDate.appendChild(createNode("Day", QString::number(mCreatedDateTime.date().day())));

	QDomElement createdTime = createNode("Time");
	createdTime.appendChild(createNode("Hour", QString::number(mCreatedDateTime.time().hour())));
	createdTime.appendChild(createNode("Minute", QString::number(mCreatedDateTime.time().minute())));
	createdTime.appendChild(createNode("Second", QString::number(mCreatedDateTime.time().second())));

	QDomElement createdVersion = createNode("Version");
	createdVersion.appendChild(createNode("Major", QString::number(mCreatedVersion.major())));
	createdVersion.appendChild(createNode("Minor", QString::number(mCreatedVersion.minor())));
	createdVersion.appendChild(createNode("Patch", QString::number(mCreatedVersion.patch())));

	QDomElement created = createNode("Created");
	created.appendChild(createdDate);
	created.appendChild(createdTime);
	created.appendChild(createdVersion);

	QDomElement editedDate = createNode("Date");
	editedDate.appendChild(createNode("Year", QString::number(mEditedDateTime.date().year())));
	editedDate.appendChild(createNode("Month", QString::number(mEditedDateTime.date().month())));
	editedDate.appendChild(createNode("Day", QString::number(mEditedDateTime.date().day())));

	QDomElement editedTime = createNode("Time");
	editedTime.appendChild(createNode("Hour", QString::number(mEditedDateTime.time().hour())));
	editedTime.appendChild(createNode("Minute", QString::number(mEditedDateTime.time().minute())));
	editedTime.appendChild(createNode("Second", QString::number(mEditedDateTime.time().second())));

	QDomElement editedVersion = createNode("Version");
	editedVersion.appendChild(createNode("Major", QString::number(mEditedVersion.major())));
	editedVersion.appendChild(createNode("Minor", QString::number(mEditedVersion.minor())));
	editedVersion.appendChild(createNode("Patch", QString::number(mEditedVersion.patch())));

	QDomElement edited = createNode("Edited");
	edited.appendChild(editedDate);
	edited.appendChild(editedTime);
	edited.appendChild(editedVersion);

	QDomElement properties = createNode("Properties");
	properties.appendChild(createNode("Description", mDescription));
	properties.appendChild(created);
	properties.appendChild(edited);

	return properties;
}
