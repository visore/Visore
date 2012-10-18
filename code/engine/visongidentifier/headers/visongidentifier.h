#ifndef VISONGIDENTIFIER_H
#define VISONGIDENTIFIER_H

#include <vibuffer.h>
#include <visonginfo.h>

class ViSongIdentifier : public QObject
{

	Q_OBJECT

	signals:

		void songDetected(ViSongInfo info);

	public:

		ViSongIdentifier();
		bool found();
		virtual bool identify(ViBuffer *buffer) = 0;

	protected:

		void detected(ViSongInfo info);

	protected:

		bool mFound;

};

#endif
