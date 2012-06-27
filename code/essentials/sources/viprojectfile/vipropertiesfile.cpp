#include "vipropertiesfile.h"

ViPropertiesFile::ViPropertiesFile()
	: ViProjectFile()
{
	setName("Properties");
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();
}

ViVersion ViPropertiesFile::createdVersion()
{
	return mCreatedVersion;
}

ViVersion ViPropertiesFile::editedVersion()
{
	return mEditedVersion;
}

QDomDocument ViPropertiesFile::toXml()
{
	mEditedVersion = ViManager::version();

	QDomDocument document;

	QDomElement root = document.createElement(name());
	document.appendChild(root);

	//Created
	QDomElement created = document.createElement("Created");
	root.appendChild(created);
	QDomElement createdVersion = document.createElement("Version");
	created.appendChild(createdVersion);

	QDomElement createdMajor = document.createElement("Major");
	createdVersion.appendChild(createdMajor);
	QDomText createdMajorText = document.createTextNode(QString::number(mCreatedVersion.major()));
	createdMajor.appendChild(createdMajorText);

	QDomElement createdMinor = document.createElement("Minor");
	createdVersion.appendChild(createdMinor);
	QDomText createdMinorText = document.createTextNode(QString::number(mCreatedVersion.minor()));
	createdMinor.appendChild(createdMinorText);

	QDomElement createdPatch = document.createElement("Patch");
	createdVersion.appendChild(createdPatch);
	QDomText createdPatchText = document.createTextNode(QString::number(mCreatedVersion.patch()));
	createdPatch.appendChild(createdPatchText);

	//Edited
	QDomElement edited = document.createElement("Edited");
	root.appendChild(edited);
	QDomElement editedVersion = document.createElement("Version");
	edited.appendChild(editedVersion);

	QDomElement editedMajor = document.createElement("Major");
	editedVersion.appendChild(editedMajor);
	QDomText editedMajorText = document.createTextNode(QString::number(mEditedVersion.major()));
	editedMajor.appendChild(editedMajorText);

	QDomElement editedMinor = document.createElement("Minor");
	editedVersion.appendChild(editedMinor);
	QDomText editedMinorText = document.createTextNode(QString::number(mEditedVersion.minor()));
	editedMinor.appendChild(editedMinorText);

	QDomElement editedPatch = document.createElement("Patch");
	editedVersion.appendChild(editedPatch);
	QDomText editedPatchText = document.createTextNode(QString::number(mEditedVersion.patch()));
	editedPatch.appendChild(editedPatchText);

	return document;
}

void ViPropertiesFile::fromXml(QDomDocument document)
{
	QDomElement created = document.firstChildElement("Created");
	QDomElement createdVersion = created.firstChildElement("Version");
	mCreatedVersion = ViVersion(createdVersion.firstChildElement("Major").text().toInt(), createdVersion.firstChildElement("Minor").text().toInt(), createdVersion.firstChildElement("Patch").text().toInt());
	QDomElement edited = document.firstChildElement("Edited");
	QDomElement editedVersion = edited.firstChildElement("Version");
	mEditedVersion = ViVersion(editedVersion.firstChildElement("Major").text().toInt(), editedVersion.firstChildElement("Minor").text().toInt(), editedVersion.firstChildElement("Patch").text().toInt());
}
