#ifndef VIINFOELEMENT_H
#define VIINFOELEMENT_H

#include <viinfoattribute.h>

class ViInfoElement;
typedef QList<ViInfoElement> ViInfoElementList;

class ViInfoElement : public ViInfoPrimitive
{

	public:

		ViInfoElement();
		ViInfoElement(QString name);
		ViInfoElement(QString name, QVariant value);
		ViInfoElement(const ViInfoElement &other);

		ViInfoElement& addChild(ViInfoElement child);
		ViInfoAttribute& addAttribute(ViInfoAttribute attribute);
		ViInfoElement& addChild(QString name);
		ViInfoAttribute& addAttribute(QString name);
		ViInfoElement& addChild(QString name, QVariant value);
		ViInfoAttribute& addAttribute(QString name, QVariant value);

		ViInfoAttributeList attributes();
		ViInfoElementList children();

		int attributeCount();
		int childrenCount();

		ViInfoAttribute attribute(int index);
		ViInfoAttribute attribute(QString name);
		ViInfoElement child(int index);
		ViInfoElement child(QString name);

		void fromString(QString xml);
		QString toString();

		void fromDom(QDomNode &dom);
		QDomNode toDom();

	private:

		ViInfoAttributeList mAttributes;
		ViInfoElementList mChildren;

};

#endif
