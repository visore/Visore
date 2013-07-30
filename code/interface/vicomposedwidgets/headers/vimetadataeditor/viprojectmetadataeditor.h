#ifndef VIPROJECTMETADATAEDITOR_H
#define VIPROJECTMETADATAEDITOR_H

#include <viwidget.h>

namespace Ui
{
	class ViProjectMetadataEditor;
}

class ViProjectMetadataEditor : public ViWidget
{

	Q_OBJECT

	private slots:

		void save();
		void changeTrack();
		void updateTracks();
		void changeProjectName();
		void changeAlbum();
		void changeArtist();
		void setGlobalArtist();
		void setGlobalAlbum();

	public:

		ViProjectMetadataEditor(QWidget *parent = 0);
		~ViProjectMetadataEditor();

		void clear();

		void setProject(ViProject *project, bool takeOwnership = true);
		bool hasProject();

	private:

		Ui::ViProjectMetadataEditor *mUi;
		bool mTakeOwnership;
		ViProject *mProject;

};

#endif
