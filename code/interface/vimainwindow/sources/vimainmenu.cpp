#include <vimainmenu.h>
#include <ui_vimainmenu.h>
#include <vistackedwidget.h>
#include <vimainrecordingwidget.h>
#include <vimaincorrelationwidget.h>
#include <vimainwavewidget.h>
#include <vimaincorrectionwidget.h>
#include <vimainmetadatawidget.h>
#include <vimainplaybackwidget.h>

ViMainMenu::ViMainMenu(QWidget *parent)
    : ViWidget(parent)
{
    mUi = new Ui::ViMainMenu();
    mUi->setupUi(this);

    QFont font;
	font.setFamily("Harabara");
	font.setPointSize(16);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);

	QColor textColor = ViThemeManager::color(ViThemeColors::TextColor1);
	int index;

	mUi->createProjectButton->setIcon(ViThemeManager::icon("microphone"), 80);
	mUi->createProjectButton->setText("Record Project", textColor, font);
	mUi->createProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainRecordingWidget(), false);
	mUi->createProjectButton->setProperty("index", index);
	QObject::connect(mUi->createProjectButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->metadataButton->setIcon(ViThemeManager::icon("metadata"), 80);
	mUi->metadataButton->setText("Update Metadata", textColor, font);
	mUi->metadataButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainMetadataWidget(), false);
	mUi->metadataButton->setProperty("index", index);
	QObject::connect(mUi->metadataButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->projectPlaybackButton->setIcon(ViThemeManager::icon("playback"), 80);
	mUi->projectPlaybackButton->setText("Playback Project", textColor, font);
	mUi->projectPlaybackButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainPlaybackWidget(), false);
	mUi->projectPlaybackButton->setProperty("index", index);
	QObject::connect(mUi->projectPlaybackButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->analyseWaveButton->setIcon(ViThemeManager::icon("soundwave"), 80);
	mUi->analyseWaveButton->setText("Sound Wave", textColor, font);
	mUi->analyseWaveButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainWaveWidget(), false);
	mUi->analyseWaveButton->setProperty("index", index);
	QObject::connect(mUi->analyseWaveButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->correlateProjectButton->setIcon(ViThemeManager::icon("correlation"), 80);
	mUi->correlateProjectButton->setText("Correlate Project", textColor, font);
	mUi->correlateProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainCorrelationWidget());
	mUi->correlateProjectButton->setProperty("index", index);
	QObject::connect(mUi->correlateProjectButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

	mUi->correctProjectButton->setIcon(ViThemeManager::icon("correction"), 80);
	mUi->correctProjectButton->setText("Correct Project", textColor, font);
	mUi->correctProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViMainCorrectionWidget());
	mUi->correctProjectButton->setProperty("index", index);
	QObject::connect(mUi->correctProjectButton, SIGNAL(clicked()), ViStackedWidget::instance().data(), SLOT(changeCurrentIndex()));

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
