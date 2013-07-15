#include <viprojectmetadatawidget.h>
#include <ui_viprojectmetadatawidget.h>

ViProjectMetadataWidget::ViProjectMetadataWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViProjectMetadataWidget();
	mUi->setupUi(this);

	clear();

	mUi->projectLoader->setStyleSheet(mUi->projectLoader->styleSheet() + "QLabel { width: 150px; min-width: 150px; }");
	mUi->projectEditor->setStyleSheet(mUi->projectEditor->styleSheet() + "QLabel { width: 150px; min-width: 150px; }");

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
	mUi->projectLoader->setProject(project);
}

void ViProjectMetadataWidget::changeProject()
{
	mUi->projectEditor->show();
	mUi->projectEditor->setProject(mUi->projectLoader->project());
}

void ViProjectMetadataWidget::hideEvent(QHideEvent *event)
{
	clear();
}
