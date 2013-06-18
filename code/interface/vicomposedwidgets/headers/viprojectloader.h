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

		enum Mode
		{
			MultipleProjects,
			SingleProject,
			SingleTrack
		};

	signals:

		void trackChanged();
		void projectChanged();
		void finished();

	private slots:

		void loadProjects();
		void loadTracks();
		void analyseTrack(int index);
		void changeMode(int mode);

	public:

		ViProjectLoader(QWidget *parent = 0);
		~ViProjectLoader();

		ViProjectQueue projects();
		ViProject* project();
		ViAudioObjectQueue objects();
		ViAudioObjectPointer object();

		void clear();
		ViAudio::Type processTypes();

		void setMode(ViProjectLoader::Mode mode);

		void enableBufferSelection(bool enable = true);
		void disableBufferSelection(bool disable = true);

	private:

		Ui::ViProjectLoader *mUi;

		ViProjectQueue mProjects;
		ViAudioObjectQueue mObjects;
		int mSelectedObject;

		ViProjectLoader::Mode mMode;
		int mProjectCount;

		bool mEnableBufferSelection;

};

#endif
