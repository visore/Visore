#ifndef VILIBRARY_H
#define VILIBRARY_H

#include <QString>
#include <vicommon.h>

class ViLibrary
{

	public:

		ViLibrary();
		ViLibrary(const ViLibrary &other);
		virtual ~ViLibrary();

		virtual QString name(QString replace = "", bool spaced = false);

		virtual ViLibrary* clone() = 0;

};

#endif
