#ifndef VITHEMERESOURCE_H
#define VITHEMERESOURCE_H

#include <QString>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QStringList>

class ViThemeResource
{

	public:

		ViThemeResource();
		ViThemeResource(const ViThemeResource &other);

		void setDirectory(QString directory);
		QString directory();
		QString name();

	protected:

		QString generatePath(QString sub = "");
		QStringList formats();
		bool exists(QString path);

	private:

		QString mDirectory;
		QString mName;

};

#endif
