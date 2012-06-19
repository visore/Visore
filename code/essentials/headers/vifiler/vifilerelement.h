#ifndef VIFILERELEMENT_H
#define VIFILERELEMENT_H

#include <QString>
#include <QDomElement>
#include <QDomDocument>

class ViFilerElement
{

	public:

		ViFilerElement(QDomDocument *document);
		bool isNull();
		QDomElement createNode(QString name);
		QDomElement createNode(QString name, QString text);

		virtual QDomElement toDom() = 0;

	protected:

		virtual bool fromDom() = 0;

	protected:

		QDomDocument *mDocument;
		bool mIsNull;

};

#endif
