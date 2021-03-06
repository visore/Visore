#include "vielement.h"
#include <QFile>

ViElement::ViElement()
	: ViValue()
{
	mDummyAttribute = NULL;
	mDummyElement = NULL;
}

ViElement::ViElement(QString name)
	: ViValue(name)
{
	mDummyAttribute = NULL;
	mDummyElement = NULL;
}

ViElement::ViElement(QString name, QVariant value)
	: ViValue(name, value)
{
	mDummyAttribute = NULL;
	mDummyElement = NULL;
}

ViElement::ViElement(const ViElement &other)
	: ViValue(other)
{
	mDummyAttribute = NULL;
	mDummyElement = NULL;
	mAttributes = other.mAttributes;
	mChildren = other.mChildren;
}

ViElement::~ViElement()
{
	if(mDummyAttribute != NULL)
	{
		delete mDummyAttribute;
		mDummyAttribute = NULL;
	}
	if(mDummyElement != NULL)
	{
		delete mDummyElement;
		mDummyElement = NULL;
	}
}

ViAttribute& ViElement::addAttribute(ViAttribute attribute)
{
	mAttributes.append(attribute);
	mAttributes.last();
}

ViAttribute& ViElement::addAttribute(QString name)
{
	mAttributes.append(ViAttribute(name));
	return mAttributes.last();
}

ViAttribute& ViElement::addAttribute(QString name, QVariant value)
{
	mAttributes.append(ViAttribute(name, value));
	return mAttributes.last();
}

ViElement& ViElement::prependChild(ViElement child)
{
    mChildren.prepend(child);
    return mChildren.first();
}

ViElement& ViElement::prependChild(QString name)
{
    return prependChild(ViElement(name));
}

ViElement& ViElement::prependChild(QString name, QVariant value)
{
    return prependChild(ViElement(name, value));
}

ViElement& ViElement::addChild(ViElement child)
{
	mChildren.append(child);
	return mChildren.last();
}

ViElement& ViElement::addChild(QString name)
{
	mChildren.append(ViElement(name));
	return mChildren.last();
}

ViElement& ViElement::addChild(QString name, QVariant value)
{
	mChildren.append(ViElement(name, value));
	return mChildren.last();
}

ViAttributeList ViElement::attributes(QString nameFilter)
{
	if(nameFilter == "")
	{
		return mAttributes;
	}
	ViAttributeList result;
	for(int i = 0; i < mAttributes.size(); ++i)
	{
		if(mAttributes[i].name() == nameFilter)
		{
			result.append(mAttributes[i]);
		}
	}
	return result;
}

ViElementList ViElement::children(QString nameFilter)
{
	if(nameFilter == "")
	{
		return mChildren;
	}
	ViElementList result;
	for(int i = 0; i < mChildren.size(); ++i)
	{
		if(mChildren[i].name() == nameFilter)
		{
			result.append(mChildren[i]);
		}
	}
	return result;
}

int ViElement::attributeCount()
{
	return mAttributes.size();
}

int ViElement::childrenCount()
{
	return mChildren.size();
}

bool ViElement::hasAttributes()
{
	return !mAttributes.isEmpty();
}

bool ViElement::hasChildren()
{
	return !mChildren.isEmpty();
}

bool ViElement::hasChild(QString name)
{
    for(int i = 0; i < childrenCount(); ++i)
    {
        if(name == mChildren[i].name())
        {
            return true;
        }
    }
    return false;
}

ViAttribute& ViElement::attribute(int index)
{
	if(attributeCount() > index)
	{
		return mAttributes[index];
	}
	return dummyAttribute();
}

ViAttribute& ViElement::attribute(QString name)
{
	for(int i = 0; i < attributeCount(); ++i)
	{
		if(name == mAttributes[i].name())
		{
			return mAttributes[i];
		}
	}
	return dummyAttribute();
}

ViElement& ViElement::child(int index)
{
	if(childrenCount() > index)
	{
		return mChildren[index];
	}
	return dummyElement();
}

ViElement& ViElement::child(QString name)
{
	for(int i = 0; i < childrenCount(); ++i)
	{
		if(name == mChildren[i].name())
		{
			return mChildren[i];
		}
	}
	for(int i = 0; i < childrenCount(); ++i)
	{
		return mChildren[i].child(name);
	}
	return dummyElement();
}


void ViElement::fromXml(QString xml)
{
	QDomDocument document("");
	document.setContent(xml);
	QDomNode node = document.firstChild();
	fromDom(node);
}

QString ViElement::toXml()
{
	QDomNode node = toDom();
	if(node.isNull())
	{
		return "";
	}
	QDomDocument document("");
	document.appendChild(node);
	return document.toString();
}

void ViElement::fromDom(QDomNode &dom)
{
	setName(dom.toElement().tagName());
	QDomNamedNodeMap attributes = dom.attributes();
	for(int i = 0; i < attributes.size(); ++i)
	{
		addAttribute(ViAttribute(attributes.item(i).nodeName(), attributes.item(i).nodeValue()));
	}
	QDomNodeList children = dom.childNodes();
	if(children.size() > 0)
	{
		for(int i = 0; i < children.size(); ++i)
		{
			if(children.item(i).isText())
			{
				setValue(children.item(i).nodeValue());
			}
			else
			{
				ViElement child;
				QDomNode node = children.item(i);
				child.fromDom(node);
				addChild(child);
			}
		}
	}
	else
	{
		setValue(dom.nodeValue());
	}
}

QDomNode ViElement::toDom()
{
	QDomDocument document;
	QDomElement node = document.createElement(name());
	for(int i = 0; i < attributeCount(); ++i)
	{
		node.setAttribute(mAttributes[i].name(), mAttributes[i].toString());
	}
	if(childrenCount() > 0)
	{
		for(int i = 0; i < childrenCount(); ++i)
		{
			node.appendChild(mChildren[i].toDom());
		}
	}
	else
	{
		node.appendChild(document.createTextNode(toString()));
	}
	return node;
}

bool ViElement::saveToFile(QString fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly))
	{
		return false;
	}
	file.write(toXml(). toLatin1());
	file.close();
	return true;
}

bool ViElement::loadFromFile(QString fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	fromXml(file.readAll());
	file.close();
	return true;
}

ViAttribute& ViElement::dummyAttribute()
{
	if(mDummyAttribute != NULL)
	{
		delete mDummyAttribute;
	}
	mDummyAttribute = new ViAttribute();
	return *mDummyAttribute;
}

ViElement& ViElement::dummyElement()
{
	if(mDummyElement != NULL)
	{
		delete mDummyElement;
	}
	mDummyElement = new ViElement();
	return *mDummyElement;
}
