#ifndef VICORRECTIONINMODE_H
#define VICORRECTIONINMODE_H

#include <QStringList>

class ViCorrectionMode
{

	public:

		enum Mode
		{
			Custom,
			Quick,
			Medium,
			Advanced,
			Extreme
		};

	public:

		virtual void setMode(ViCorrectionMode::Mode mode);

		static QString modeToString(ViCorrectionMode::Mode mode);
		static ViCorrectionMode::Mode stringToMode(QString mode);
		static QStringList modes();
		static QString defaultMode();

};

#endif
