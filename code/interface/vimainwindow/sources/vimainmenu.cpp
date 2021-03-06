#include <vimainmenu.h>
#include <ui_vimainmenu.h>
#include <vistackedwidget.h>
#include <vimainrecordingwidget.h>
#include <vimaincorrelationwidget.h>
#include <vimainwavewidget.h>
#include <vimaincorrectionwidget.h>
#include <vimainmetadatawidget.h>
#include <vimainplaybackwidget.h>
#include <vimainspectrumwidget.h>
#include <vimainnoisewidget.h>

ViMainMenu::ViMainMenu(QWidget *parent)
    : ViWidget(parent)
{
    mUi = new Ui::ViMainMenu();
    mUi->setupUi(this);

    QFont font;
	font.setFamily("Harabara");
	font.setPointSize(18);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);

	QColor textColor = ViThemeManager::color(ViThemeColors::TextColor1);
	int index;

	mUi->createProjectButton->setIcon(ViThemeManager::icon("microphone"), 80);
	mUi->createProjectButton->setText("   Record\n   Project", textColor, font);
	mUi->createProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainRecordingWidget(), false);
	mUi->createProjectButton->setProperty("index", index);
	QObject::connect(mUi->createProjectButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->metadataButton->setIcon(ViThemeManager::icon("metadata"), 80);
	mUi->metadataButton->setText("   Update\n   Metadata", textColor, font);
	mUi->metadataButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainMetadataWidget(), false);
	mUi->metadataButton->setProperty("index", index);
	QObject::connect(mUi->metadataButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->projectPlaybackButton->setIcon(ViThemeManager::icon("playback"), 80);
	mUi->projectPlaybackButton->setText("   Playback\n   Project", textColor, font);
	mUi->projectPlaybackButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainPlaybackWidget(), false);
	mUi->projectPlaybackButton->setProperty("index", index);
	QObject::connect(mUi->projectPlaybackButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->analyseWaveButton->setIcon(ViThemeManager::icon("soundwave"), 80);
	mUi->analyseWaveButton->setText("   Soundwave", textColor, font);
	mUi->analyseWaveButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainWaveWidget(), false);
	mUi->analyseWaveButton->setProperty("index", index);
	QObject::connect(mUi->analyseWaveButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->spectrumButton->setIcon(ViThemeManager::icon("spectrum"), 80);
	mUi->spectrumButton->setText("   Frequency\n   Spectrum", textColor, font);
	mUi->spectrumButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainSpectrumWidget(), false);
	mUi->spectrumButton->setProperty("index", index);
	QObject::connect(mUi->spectrumButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->correlateProjectButton->setIcon(ViThemeManager::icon("correlation"), 80);
	mUi->correlateProjectButton->setText("   Correlate\n   Project", textColor, font);
	mUi->correlateProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainCorrelationWidget(), false);
	mUi->correlateProjectButton->setProperty("index", index);
	QObject::connect(mUi->correlateProjectButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->correctProjectButton->setIcon(ViThemeManager::icon("correction"), 80);
	mUi->correctProjectButton->setText("   Correct\n   Project", textColor, font);
	mUi->correctProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainCorrectionWidget(), false);
	mUi->correctProjectButton->setProperty("index", index);
	QObject::connect(mUi->correctProjectButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->noiseDetectionButton->setIcon(ViThemeManager::icon("noise"), 80);
	mUi->noiseDetectionButton->setText("   Detect\n   Noise", textColor, font);
	mUi->noiseDetectionButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainNoiseWidget(), false);
	mUi->noiseDetectionButton->setProperty("index", index);
	QObject::connect(mUi->noiseDetectionButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

    QFont headingFont;
	headingFont.setFamily("Harabara");
	headingFont.setPointSize(24);
	headingFont.setBold(true);
	headingFont.setLetterSpacing(QFont::PercentageSpacing, 105);

	QColor headingColor = ViThemeManager::color(ViThemeColors::MainColor6);

	mUi->creationHeading->setFont(headingFont);
	mUi->playbackHeading->setFont(headingFont);
	mUi->analysisHeading->setFont(headingFont);
	mUi->correctionHeading->setFont(headingFont);

	mUi->creationHeading->setStyleSheet("color: " + headingColor.name());
	mUi->playbackHeading->setStyleSheet("color: " + headingColor.name());
	mUi->analysisHeading->setStyleSheet("color: " + headingColor.name());
    mUi->correctionHeading->setStyleSheet("color: " + headingColor.name());

	/*QColor lineColor = ViThemeManager::color(2);
	mUi->line1->setStyleSheet("color: " + lineColor.name());
	mUi->line2->setStyleSheet("color: " + lineColor.name());
	mUi->line3->setStyleSheet("color: " + lineColor.name());
	mUi->line4->setStyleSheet("color: " + lineColor.name());
	mUi->line5->setStyleSheet("color: " + lineColor.name());*/
}

ViMainMenu::~ViMainMenu()
{
    delete mUi;
}
