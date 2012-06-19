#include "vifilerelement.h"

ViFilerElement::ViFilerElement(QDomDocument *document)
{
	mDocument = document;
	mIsNull = true;
}

bool ViFilerElement::isNull()
{
	return mIsNull;
}

QDomElement ViFilerElement::createNode(QString name)
{
	return mDocument->createElement(name);
}

QDomElement ViFilerElement::createNode(QString name, QString text)
{
	QDomElement element = mDocument->createElement(name);
	element.appendChild(mDocument->createTextNode(text));
	return element;
}
