#include "vipropertiesfile.h"

ViPropertiesFile::ViPropertiesFile()
	: ViProjectFile()
{
	setName("Properties");
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();
	mProjectName = "";
}

ViVersion ViPropertiesFile::createdVersion()
{
	return mCreatedVersion;
}

ViVersion ViPropertiesFile::editedVersion()
{
	return mEditedVersion;
}

QString ViPropertiesFile::projectName()
{
	return mProjectName;
}

void ViPropertiesFile::setProjectName(QString name)
{
	mProjectName = name;
}

QDomDocument ViPropertiesFile::toXml()
{
	mEditedVersion = ViManager::version();

	QDomDocument document;

	QDomElement root = document.createElement(name());
	document.appendChild(root);

	//Project name
	QDomElement project = document.createElement("ProjectName");
	root.appendChild(project);
	QDomText projectText = document.createTextNode(mProjectName);
	project.appendChild(projectText);

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
	mProjectName  = document.elementsByTagName("ProjectName").item(0).toElement().text();
	QDomElement created = document.elementsByTagName("Created").item(0).toElement();
	QDomElement createdVersion = created.elementsByTagName("Version").item(0).toElement();
	mCreatedVersion = ViVersion(createdVersion.elementsByTagName("Major").item(0).toElement().text().toInt(), createdVersion.elementsByTagName("Minor").item(0).toElement().text().toInt(), createdVersion.elementsByTagName("Patch").item(0).toElement().text().toInt());
	QDomElement edited = document.elementsByTagName("Edited").item(0).toElement();
	QDomElement editedVersion = edited.elementsByTagName("Version").item(0).toElement();
	mEditedVersion = ViVersion(editedVersion.elementsByTagName("Major").item(0).toElement().text().toInt(), editedVersion.elementsByTagName("Minor").item(0).toElement().text().toInt(), editedVersion.elementsByTagName("Patch").item(0).toElement().text().toInt());
}
