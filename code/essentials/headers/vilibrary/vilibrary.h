#ifndef VILIBRARY_H
#define VILIBRARY_H

#include <QString>
#include <vicommon.h>
#include <viname.h>

class ViLibrary : public ViName
{

	public:

		ViLibrary();
		ViLibrary(const ViLibrary &other);
		virtual ~ViLibrary();

        virtual ViLibrary* clone();

};

#endif
