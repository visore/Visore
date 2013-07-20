#include <viaboutdialog.h>
#include <ui_viaboutdialog.h>

ViAboutDialog::ViAboutDialog(QWidget *parent)
	: QDialog(parent, Qt::Popup)
{
	mUi = new Ui::ViAboutDialog();
	mUi->setupUi(this);

	mUi->logoButton->disableBackground();
	mUi->logoButton->disableBorder();
	mUi->logoButton->addStyleSheet("background: qradialgradient(cx: 0.5, cy: 0.5, radius: 1, fx: 0.5, fy: 0.5, stop: 0 " + ViThemeManager::color(ViThemeColors::ButtonNormalColor2).name() + ", stop: 0.45 transparent);", ViButton::Hovered);
	mUi->logoButton->setSize(64, 64);
	mUi->logoButton->setIcon(ViThemeManager::icon("logo"), 64);

	QString color = ViThemeManager::color(ViThemeColors::MainColor6).name();
	ViFont font = ViThemeManager::font(ViThemeFonts::MainFont);
	font.setPointSize(30);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	mUi->logoLabel->setFont(font);
	mUi->logoLabel->setStyleSheet("color: " + color);

	mUi->version->setText(ViManager::version().toString());
	mUi->license->setText(ViManager::license());

	mUi->homepage->setText("<a style=\"color: " + color + ";\" + href=\"" + ViManager::url().toString() + "\">" + ViManager::url().toString() + "</a>");
	mUi->homepage->setTextFormat(Qt::RichText);
	mUi->homepage->setTextInteractionFlags(Qt::TextBrowserInteraction);
	mUi->homepage->setOpenExternalLinks(true);

	mUi->git->setText("<a style=\"color: " + color + ";\" + href=\"" + ViManager::gitUrl().toString() + "\">" + ViManager::gitUrl().toString() + "</a>");
	mUi->git->setTextFormat(Qt::RichText);
	mUi->git->setTextInteractionFlags(Qt::TextBrowserInteraction);
	mUi->git->setOpenExternalLinks(true);

	mUi->email->setText("<a style=\"color: " + color + ";\" + href=\"mailto:" + ViManager::email() + "\">" + ViManager::email() + "</a>");
	mUi->email->setTextFormat(Qt::RichText);
	mUi->email->setTextInteractionFlags(Qt::TextBrowserInteraction);
	mUi->email->setOpenExternalLinks(true);

	mUi->developer->setText(ViManager::developer());

	mUi->shortDescription->setText(ViManager::shortDescription());
	mUi->shortDescription->setStyleSheet("color: " + color + ";");

	mUi->longDescription->setText(ViManager::longDescription());

	mUi->copyright->setText(ViManager::copyright().replace("(c)", "©"));
}

ViAboutDialog::~ViAboutDialog()
{
	delete mUi;
}
