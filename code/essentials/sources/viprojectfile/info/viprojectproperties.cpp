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

ViInfoElement ViProjectProperties::toXml()
{
	mEditedVersion = ViManager::version();
	ViInfoElement root(name());

	//Project name
	root.addChild(ViInfoElement("ProjectName", mProjectName));

	//Created
	ViInfoElement createdVersion("Version");
	createdVersion.addChild(ViInfoElement("Major", QString::number(mCreatedVersion.major())));
	createdVersion.addChild(ViInfoElement("Minor", QString::number(mCreatedVersion.minor())));
	createdVersion.addChild(ViInfoElement("Patch", QString::number(mCreatedVersion.patch())));
	ViInfoElement created("Created");
	created.addChild(createdVersion);
	root.addChild(created);

	//Edited
	ViInfoElement editedVersion("Version");
	editedVersion.addChild(ViInfoElement("Major", QString::number(mEditedVersion.major())));
	editedVersion.addChild(ViInfoElement("Minor", QString::number(mEditedVersion.minor())));
	editedVersion.addChild(ViInfoElement("Patch", QString::number(mEditedVersion.patch())));
	ViInfoElement edited("Edited");
	edited.addChild(editedVersion);
	root.addChild(edited);

	return root;
}

bool ViProjectProperties::fromXml(ViInfoElement &document)
{
	if(document.name() == name())
	{
		mProjectName = document.child("ProjectName").valueString();
		ViInfoElement created = document.child("Created").child("Version");
		mCreatedVersion = ViVersion(created.child("Major").valueInt(), created.child("Minor").valueInt(), created.child("Patch").valueInt());
		ViInfoElement edited = document.child("Edited").child("Version");
		mEditedVersion = ViVersion(edited.child("Major").valueInt(), edited.child("Minor").valueInt(), edited.child("Patch").valueInt());
		return true;
	}
	return false;
}
