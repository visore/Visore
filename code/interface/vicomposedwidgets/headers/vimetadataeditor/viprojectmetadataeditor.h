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
		void setGlobalArtist();
		void setGlobalAlbum();

	public:

		ViProjectMetadataEditor(QWidget *parent = 0);
		~ViProjectMetadataEditor();

		void clear();

		void setProject(ViProject *project, bool autoClear = true); // Auto clear if widget is hidden
		bool hasProject();

	protected:

		void hideEvent(QHideEvent *event);

	private:

		Ui::ViProjectMetadataEditor *mUi;
		bool mAutoClear;
		ViProject *mProject;
		QString mGlobalArtist;
		QString mGlobalAlbum;

};

#endif
