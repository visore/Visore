#include "viinfoelement.h"
#include "vilogger.h"
ViInfoElement::ViInfoElement()
	: ViInfoPrimitive()
{
}

ViInfoElement::ViInfoElement(QString name)
	: ViInfoPrimitive(name)
{
}

ViInfoElement::ViInfoElement(QString name, QVariant value)
	: ViInfoPrimitive(name, value)
{
}

ViInfoElement::ViInfoElement(const ViInfoElement &other)
	: ViInfoPrimitive(other)
{
	mAttributes = other.mAttributes;
	mChildren = other.mChildren;
}

ViInfoElement& ViInfoElement::addChild(ViInfoElement child)
{
	mChildren.append(child);
	return mChildren.last();
}

ViInfoAttribute& ViInfoElement::addAttribute(ViInfoAttribute attribute)
{
	mAttributes.append(attribute);
	mAttributes.last();
}

ViInfoElement& ViInfoElement::addChild(QString name)
{
	mChildren.append(ViInfoElement(name));
	return mChildren.last();
}

ViInfoAttribute& ViInfoElement::addAttribute(QString name)
{
	mAttributes.append(ViInfoAttribute(name));
	return mAttributes.last();
}

ViInfoElement& ViInfoElement::addChild(QString name, QVariant value)
{
	mChildren.append(ViInfoElement(name, value));
	return mChildren.last();
}

ViInfoAttribute& ViInfoElement::addAttribute(QString name, QVariant value)
{
	mAttributes.append(ViInfoAttribute(name, value));
	return mAttributes.last();
}

ViInfoAttributeList ViInfoElement::attributes()
{
	return mAttributes;
}

ViInfoElementList ViInfoElement::children()
{
	return mChildren;
}

int ViInfoElement::attributeCount()
{
	return mAttributes.size();
}

int ViInfoElement::childrenCount()
{
	return mChildren.size();
}

ViInfoAttribute ViInfoElement::attribute(int index)
{
	if(attributeCount() > index)
	{
		return mAttributes[index];
	}
	return ViInfoAttribute();
}

ViInfoAttribute ViInfoElement::attribute(QString name)
{
	for(int i = 0; i < attributeCount(); ++i)
	{
		if(name == mAttributes[i].name())
		{
			return mAttributes[i];
		}
	}
	return ViInfoAttribute();
}

ViInfoElement ViInfoElement::child(int index)
{
	if(childrenCount() > index)
	{
		return mChildren[index];
	}
	return ViInfoElement();
}

ViInfoElement ViInfoElement::child(QString name)
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
		ViInfoElement child = mChildren[i].child(name);
		if(!child.isNull())
		{
			return child;
		}
	}
	return ViInfoElement();
}


void ViInfoElement::fromXml(QString xml)
{
	QDomDocument document("");
	document.setContent(xml);
	QDomNode node = document.firstChild();
	fromDom(node);
}

QString ViInfoElement::toXml()
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

void ViInfoElement::fromDom(QDomNode &dom)
{
	setName(dom.toElement().tagName());
	QDomNamedNodeMap attributes = dom.attributes();
	for(int i = 0; i < attributes.size(); ++i)
	{
		addAttribute(ViInfoAttribute(attributes.item(i).nodeName(), attributes.item(i).nodeValue()));
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
				ViInfoElement child;
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

QDomNode ViInfoElement::toDom()
{
	QDomDocument document;
	QDomElement node = document.createElement(name());
	for(int i = 0; i < attributeCount(); ++i)
	{
		node.setAttribute(mAttributes[i].name(), mAttributes[i].valueString());
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
		node.appendChild(document.createTextNode(valueString()));
	}
	return node;
}
