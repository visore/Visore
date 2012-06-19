#include "vifiler.h"
#include "vimanager.h"

ViFiler::ViFiler(QString filePath)
{
	mProperties = NULL;
	setFilePath(filePath);
}

ViFiler::~ViFiler()
{
	close();
	delete mProperties;
}

void ViFiler::setFilePath(QString filePath)
{
	QString extension = "." + ViManager::projectExtension();
	if(!filePath.endsWith(extension))
	{
		filePath += extension;
	}
	mFile.setFileName(filePath);
	open();
}

QString ViFiler::filePath()
{
	return mFile.fileName();
}

ViPropertiesElement* ViFiler::properties()
{
	return mProperties;
}

bool ViFiler::open()
{
	if(filePath() == "")
	{
		return false;
	}
	if(mFile.isOpen())
	{
		delete mProperties;
		close();
	}
	if(mFile.open(QIODevice::ReadWrite))
	{
		mDocument.setContent(mFile.readAll());
		mProperties = new ViPropertiesElement(&mDocument);
		return true;
	}
	return false;
}

bool ViFiler::save()
{
	QDomElement root = mDocument.createElement("VisoreProject");
	root.appendChild(mProperties->toDom());
	mDocument.appendChild(root);

	mFile.resize(0);
	mFile.write(mDocument.toByteArray());
	return mFile.flush();
}

void ViFiler::close()
{
	if(mFile.isOpen())
	{
		save();
	}
	mFile.close();
}
