#ifndef VIPROJECT_H
#define VIPROJECT_H

#include "viid.h"
#include "vimanager.h"
#include "viarchive.h"
#include "viaudioobject.h"
#include "viaudiocoder.h"
#include <QMap>

class ViProject : public QObject, public ViId
{

	Q_OBJECT

	signals:

		void finished();

	public slots:

		void serialize(ViAudioObjectPointer object);
		void setFormat(ViAudioFormat format);

		/*******************************************************************************************************************

			LOAD & SAVE

		*******************************************************************************************************************/

		bool load(bool minimal = false); // minimal - Only load the vml files
		void save();

	public:

		/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

		*******************************************************************************************************************/

		ViProject(QString filePath);
		ViProject(QString projectName, QString filePath, int sides);
		~ViProject();

		/*******************************************************************************************************************

			BASICS

		*******************************************************************************************************************/

		qint64 size();

		void setFilePath(QString filePath);
		QString filePath();

		void setProjectName(QString name);
		QString projectName();

		/*******************************************************************************************************************

			SIDES

		*******************************************************************************************************************/

		void setSides(int sides);
		int sides();
		int currentSide();
		void nextSide();

	protected:

		bool createTempStructure();
		bool removeTempStructure();

		QString generateFileName(ViSongInfo info, QString folder = "");

	private:

		int mSides;
		int mCurrentSide;
		int mCurrentTrack;

		QMap<QString,QString> mPaths;

		ViArchive mArchive;
		ViAudioFormat mFormat;
		ViAudioCoder mEncoder;

		ViAudioObjectPointer t;
};

#endif
