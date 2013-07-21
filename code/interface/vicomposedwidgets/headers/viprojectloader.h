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

	public:

		enum ProjectMode
		{
			MultipleProjects,
			SingleProject,
			SingleTrack
		};

		//How many buffer types can be selected
		enum TypeMode
		{
			NoTypes,
			MultipleTypes,
			SingleType
		};

	signals:

		void projectModeChanged();
		void trackChanged();
		void projectChanged();
		void typesChanged();
		void finished();

	private slots:

		void loadProjects();
		void loadProjects(QList<ViProject*> projects);
		void loadTracks();
		void analyseTrack(int index);
		void changeProjectMode(int mode);
		void changeRadioTypes(bool checked);

	public:

		ViProjectLoader(QWidget *parent = 0);
		~ViProjectLoader();

		ViProjectQueue projects();
		ViProject* project();
		ViAudioObjectQueue objects();
		ViAudioObjectPointer object();

		void clear();
		ViAudio::Type type();
		ViAudio::Type types();

		void setProjectMode(ViProjectLoader::ProjectMode mode);
		ViProjectLoader::ProjectMode projectMode();
		void setTypeMode(ViProjectLoader::TypeMode mode);
		ViProjectLoader::TypeMode typeMode();

	private:

		Ui::ViProjectLoader *mUi;

		ViProjectQueue mProjects;
		ViAudioObjectQueue mObjects;
		int mSelectedObject;

		ViProjectLoader::ProjectMode mProjectMode;
		int mProjectCount;

		ViProjectLoader::TypeMode mTypeMode;

};

#endif
