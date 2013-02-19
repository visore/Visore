#include <vimainmenu.h>
#include <ui_vimainmenu.h>
#include <vistackedwidget.h>
#include <viprojectrecordingwidget.h>
#include <viprojectcorrelationwidget.h>
#include <viwaveanalysiswidget.h>

ViMainMenu::ViMainMenu(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViMainMenu();
	mUi->setupUi(this);

	QFont font;
	font.setFamily("Bauhaus");
	font.setPointSize(16);
	font.setBold(true);

	QColor textColor = ViThemeManager::color(ViThemeColors::TextColor1);
	int index;

	mUi->createProjectButton->setIcon(ViThemeManager::icon("recordingproject"), 80);
	mUi->createProjectButton->setText("Record Project", textColor, font);
	mUi->createProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViProjectRecordingWidget());
	mUi->createProjectButton->addFunctionCall(SIGNAL(clicked()), ViStackedWidget::currentIndexCall(index));

	mUi->projectPlaybackButton->setIcon(ViThemeManager::icon("recordingproject"), 80);
	mUi->projectPlaybackButton->setText("Playback Project", textColor, font);
	mUi->projectPlaybackButton->setSize(250, 100);

	mUi->linePlaybackButton->setIcon(ViThemeManager::icon("recordingproject"), 80);
	mUi->linePlaybackButton->setText("Playback Vinyl", textColor, font);
	mUi->linePlaybackButton->setSize(250, 100);

	mUi->filePlaybackButton->setIcon(ViThemeManager::icon("recordingproject"), 80);
	mUi->filePlaybackButton->setText("Playback File", textColor, font);
	mUi->filePlaybackButton->setSize(250, 100);

	mUi->analyseWaveButton->setIcon(ViThemeManager::icon("recordingproject"), 80);
	mUi->analyseWaveButton->setText("Analyse Sound Wave", textColor, font);
	mUi->analyseWaveButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViWaveAnalysisWidget());
	mUi->analyseWaveButton->addFunctionCall(SIGNAL(clicked()), ViStackedWidget::currentIndexCall(index));

	mUi->correlateProjectButton->setIcon(ViThemeManager::icon("recordingproject"), 80);
	mUi->correlateProjectButton->setText("Correlate Project", textColor, font);
	mUi->correlateProjectButton->setSize(250, 100);
	index = ViStackedWidget::addWidget(new ViProjectCorrelationWidget());
	mUi->correlateProjectButton->addFunctionCall(SIGNAL(clicked()), ViStackedWidget::currentIndexCall(index));

	mUi->creationHeading->setFont(font);
	mUi->playbackHeading->setFont(font);
	mUi->analysisHeading->setFont(font);
	mUi->correctionHeading->setFont(font);

	mUi->creationHeading->setStyleSheet("color: " + textColor.name());
	mUi->playbackHeading->setStyleSheet("color: " + textColor.name());
	mUi->analysisHeading->setStyleSheet("color: " + textColor.name());
	mUi->correctionHeading->setStyleSheet("color: " + textColor.name());

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
