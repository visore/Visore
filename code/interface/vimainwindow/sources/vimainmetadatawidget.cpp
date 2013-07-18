#include <vimainmetadatawidget.h>
#include <ui_vimainmetadatawidget.h>

ViMainMetadataWidget::ViMainMetadataWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainMetadataWidget();
	mUi->setupUi(this);

	clear();

	QObject::connect(mUi->projectLoader, SIGNAL(finished()), this, SLOT(changeProject()));
}

ViMainMetadataWidget::~ViMainMetadataWidget()
{
	clear();
	delete mUi;
}

void ViMainMetadataWidget::clear()
{
	mUi->projectLoader->clear();
	mUi->projectLoader->setMode(ViProjectLoader::SingleProject);
	mUi->projectLoader->disableBufferSelection();

	mUi->projectEditor->clear();
	mUi->projectEditor->hide();
}

void ViMainMetadataWidget::setProject(ViProject *project)
{
	clear();
	mUi->projectLoader->hide();
	mUi->projectEditor->show();
	mUi->projectEditor->setProject(project, true);
}

void ViMainMetadataWidget::changeProject()
{
	mUi->projectEditor->show();
	mUi->projectEditor->setProject(mUi->projectLoader->project(), false);
}
