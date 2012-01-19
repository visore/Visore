#ifndef VIERROR_H
#define VIERROR_H

#include "vilogger.h"
#include <QList>

struct ViErrorInfo
{
	public:
		enum ViErrorType
		{
			Fatal = 0,
			MediumFatal = 1,
			NonFatal = 2
		};

		ViErrorInfo(QString name, QString description, ViErrorInfo::ViErrorType type);
		void setName(QString name);
		void setDescription(QString description);
		void setType(ViErrorInfo::ViErrorType type);
		QString name();
		QString description();
		ViErrorInfo::ViErrorType type();
		void print();

	private:
		QString mName;
		QString mDescription;
		ViErrorInfo::ViErrorType mType;
};

class ViError
{
	public:	
		void setErrorParameters(QString name = "", QString description = "", ViErrorInfo::ViErrorType type = ViErrorInfo::MediumFatal, bool debugPrint = true);
		ViErrorInfo error(int index = 0);
		int numberOfErrors();
		void clearErrors();

	private:
		QList<ViErrorInfo> mErrors;
};

#endif
