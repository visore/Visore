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
		virtual ~ViElement();

		ViAttribute& addAttribute(ViAttribute attribute);
		ViAttribute& addAttribute(QString name);
		ViAttribute& addAttribute(QString name, QVariant value);

        ViElement& prependChild(ViElement child);
        ViElement& prependChild(QString name);
        ViElement& prependChild(QString name, QVariant value);

        ViElement& addChild(ViElement child);
		ViElement& addChild(QString name);
		ViElement& addChild(QString name, QVariant value);

		ViAttributeList attributes(QString nameFilter = "");
		ViElementList children(QString nameFilter = "");

		int attributeCount();
		int childrenCount();

		bool hasAttributes();
		bool hasChildren();
        bool hasChild(QString name);

		ViAttribute& attribute(int index);
		ViAttribute& attribute(QString name);
		ViElement& child(int index);
		ViElement& child(QString name);

		void fromXml(QString xml);
		QString toXml();

		void fromDom(QDomNode &dom);
		QDomNode toDom();

		bool saveToFile(QString fileName);
		bool loadFromFile(QString fileName);

	private:

		ViAttribute& dummyAttribute();
		ViElement& dummyElement();

	private:

		ViAttributeList mAttributes;
		ViElementList mChildren;

		ViAttribute *mDummyAttribute;
		ViElement *mDummyElement;

};

#endif
