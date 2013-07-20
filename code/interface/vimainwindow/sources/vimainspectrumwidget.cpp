#include <vimainspectrumwidget.h>
#include <ui_vimainspectrumwidget.h>

ViMainSpectrumWidget::ViMainSpectrumWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainSpectrumWidget();
	mUi->setupUi(this);

	mUi->projectLoader->setProjectMode(ViProjectLoader::SingleTrack);
	mUi->projectLoader->setTypeMode(ViProjectLoader::SingleType);

	clear();
}

ViMainSpectrumWidget::~ViMainSpectrumWidget()
{
	clear();
	delete mUi;
}

void ViMainSpectrumWidget::clear()
{
	mUi->projectLoader->disconnect();
	mUi->spectrumWidget->disconnect();

	mUi->projectLoader->clear();
	mUi->spectrumWidget->clear();
	mUi->scrollArea->hide();

	QObject::connect(mUi->projectLoader, SIGNAL(typesChanged()), this, SLOT(generate()));
	QObject::connect(mUi->spectrumWidget, SIGNAL(windowSizeChanged()), this, SLOT(generate()));
	QObject::connect(mUi->spectrumWidget, SIGNAL(windowFunctionChanged()), this, SLOT(generate()));
}

void ViMainSpectrumWidget::generate()
{
	mUi->spectrumWidget->disableClearOnHide();
	mUi->scrollArea->hide();
	mUi->spectrumWidget->enableClearOnHide();
	QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(assign()));
	engine()->generateSpectrum(mUi->projectLoader->object(), mUi->projectLoader->type(), mUi->spectrumWidget->windowSize(), mUi->spectrumWidget->windowFunction());
}

void ViMainSpectrumWidget::assign()
{
	mUi->scrollArea->show();
	QObject::disconnect(engine().data(), SIGNAL(progressFinished()), this, SLOT(assign()));
	mUi->spectrumWidget->setSpectrum(mUi->projectLoader->object()->spectrum(mUi->projectLoader->type()));
}
