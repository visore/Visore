#include <viprojectmetadatawidget.h>
#include <ui_viprojectmetadatawidget.h>

ViProjectMetadataWidget::ViProjectMetadataWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectMetadataWidget();
	mUi->setupUi(this);

	clear();

	mUi->projectLoader->setMode(ViProjectLoader::SingleProject);
	mUi->projectLoader->disableBufferSelection();
	QObject::connect(mUi->projectLoader, SIGNAL(finished()), this, SLOT(changeProject()));
}

ViProjectMetadataWidget::~ViProjectMetadataWidget()
{
	clear();
	delete mUi;
}

void ViProjectMetadataWidget::clear()
{
	mUi->projectLoader->clear();
	mUi->projectEditor->clear();
	mUi->projectEditor->hide();
}

void ViProjectMetadataWidget::setProject(ViProject *project)
{
	clear();
	mUi->projectLoader->hide();
	mUi->projectEditor->show();
	mUi->projectEditor->setProject(project, true);
}

void ViProjectMetadataWidget::changeProject()
{
	mUi->projectEditor->show();
	mUi->projectEditor->setProject(mUi->projectLoader->project(), false);
}
