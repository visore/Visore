#ifndef VIELEMENT_H
#define VIELEMENT_H

#include <QDomNode>
#include <viattribute.h>

class ViElement;
typedef QList<ViElement> ViElementList;

class ViElement : public ViValue
{

	public:

		ViElement();
		ViElement(QString name);
		ViElement(QString name, QVariant value);
		ViElement(const ViElement &other);

		ViElement& addChild(ViElement child);
		ViAttribute& addAttribute(ViAttribute attribute);
		ViElement& addChild(QString name);
		ViAttribute& addAttribute(QString name);
		ViElement& addChild(QString name, QVariant value);
		ViAttribute& addAttribute(QString name, QVariant value);

		ViAttributeList attributes();
		ViElementList children();

		int attributeCount();
		int childrenCount();

		bool hasAttributes();
		bool hasChildren();

		ViAttribute attribute(int index);
		ViAttribute attribute(QString name);
		ViElement child(int index);
		ViElement child(QString name);

		void fromXml(QString xml);
		QString toXml();

		void fromDom(QDomNode &dom);
		QDomNode toDom();

	private:

		ViAttributeList mAttributes;
		ViElementList mChildren;

};

#endif
