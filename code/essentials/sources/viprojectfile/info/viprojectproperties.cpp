#include "viprojectproperties.h"

ViProjectProperties::ViProjectProperties()
	: ViProjectInfo()
{
	setName("Properties");
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();
	mProjectName = "";
}

ViVersion ViProjectProperties::createdVersion()
{
	return mCreatedVersion;
}

ViVersion ViProjectProperties::editedVersion()
{
	return mEditedVersion;
}

QString ViProjectProperties::projectName()
{
	return mProjectName;
}

void ViProjectProperties::setProjectName(QString name)
{
	mProjectName = name;
}

ViElement ViProjectProperties::toXml()
{
	mEditedVersion = ViManager::version();
	ViElement root(name());

	//Project name
	root.addChild(ViElement("ProjectName", mProjectName));

	//Created
	ViElement createdVersion("Version");
	createdVersion.addChild(ViElement("Major", QString::number(mCreatedVersion.major())));
	createdVersion.addChild(ViElement("Minor", QString::number(mCreatedVersion.minor())));
	createdVersion.addChild(ViElement("Patch", QString::number(mCreatedVersion.patch())));
	ViElement created("Created");
	created.addChild(createdVersion);
	root.addChild(created);

	//Edited
	ViElement editedVersion("Version");
	editedVersion.addChild(ViElement("Major", QString::number(mEditedVersion.major())));
	editedVersion.addChild(ViElement("Minor", QString::number(mEditedVersion.minor())));
	editedVersion.addChild(ViElement("Patch", QString::number(mEditedVersion.patch())));
	ViElement edited("Edited");
	edited.addChild(editedVersion);
	root.addChild(edited);

	return root;
}

bool ViProjectProperties::fromXml(ViElement &document)
{
	if(document.name() == name())
	{
		mProjectName = document.child("ProjectName").toString();
		ViElement created = document.child("Created").child("Version");
		mCreatedVersion = ViVersion(created.child("Major").toInt(), created.child("Minor").toInt(), created.child("Patch").toInt());
		ViElement edited = document.child("Edited").child("Version");
		mEditedVersion = ViVersion(edited.child("Major").toInt(), edited.child("Minor").toInt(), edited.child("Patch").toInt());
		return true;
	}
	return false;
}
