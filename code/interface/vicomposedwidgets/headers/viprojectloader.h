#ifndef VIPROJECTLOADER_H
#define VIPROJECTLOADER_H

#include <viwidget.h>

namespace Ui
{
    class ViProjectLoader;
}

class ViProjectLoader : public ViWidget
{

	Q_OBJECT

	signals:

		void opened();
		void trackChanged(ViAudioObjectPointer object);
		void started();
		void projectChanged();

	private slots:

		void loadProject();
		void loadTracks();
		void analyseTrack(int index);
		void checkStart();

	public:

		ViProjectLoader(QWidget *parent = 0);
		~ViProjectLoader();
		ViProject* project();
		ViAudioObjectPointer currentObject();
		void clear();
		ViAudioObject::Type processTypes();

	private:

		Ui::ViProjectLoader *mUi;
		ViProject *mProject;
		ViAudioObjectList mObjects;
		ViAudioObjectPointer mObject;

};

#endif
