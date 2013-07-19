#ifndef VIMETADATAWIDGET_H
#define VIMETADATAWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMetadataWidget;
}

class ViMetadataWidget : public ViWidget
{

	Q_OBJECT

	public:

		ViMetadataWidget(QWidget *parent = 0);
		~ViMetadataWidget();

		void clear();

		void setMetadata(const ViMetadata &metadata);
		void setFileInfo(const qint64 &milliseconds, const qint64 &fileBytes);
		void setFormat(const ViAudioFormat &format);

	private:

		Ui::ViMetadataWidget *mUi;

};

#endif
