#include "vivulcanotheme.h"
#include "vivulcanothemecolors.h"
#include "vivulcanothemefonts.h"
#include <vithememanager.h>

ViVulcanoTheme::ViVulcanoTheme()
	: ViTheme()
{
	mName = "Vulcano";
	mColors = new ViVulcanoThemeColors();
	mFonts = new ViVulcanoThemeFonts();

	QString borderColor1 = mColors->color(ViThemeColors::BorderColor1).name();
	QString borderColor2 = mColors->color(ViThemeColors::BorderColor2).name();
	QString buttonColor1 = mColors->color(ViThemeColors::ButtonNormalColor1).name();
	QString buttonColor2 = mColors->color(ViThemeColors::ButtonNormalColor2).name();

	QString downIconNormal = ViThemeIcon("down").path(ViThemeIcon::Normal);
	QString upIconNormal = ViThemeIcon("up").path(ViThemeIcon::Normal);

	QString normalGradient = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + buttonColor1 + ", stop: 0.05 " + buttonColor1 + ", stop: 0.5 " + buttonColor2 + ", stop: 0.95 " + buttonColor1 + ", stop: 1.0 " + buttonColor1 + ")";
	QString selectedGradient = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + buttonColor1 + ", stop: 0.4 " + buttonColor2 + ", stop: 0.6 " + buttonColor2 + ", stop: 1.0 " + buttonColor1 + ");";

	ViFont font;
	font.setFamily("Harabara");
	font.setPointSize(16);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	font.setColor(mColors->color(ViThemeColors::ButtonTextColor1));

	//http://qt-project.org/doc/qt-5.0/qtwidgets/stylesheet-examples.html#customizing-qcombobox

	// QCombobBox
	mGlobalStyleSheet += "QComboBox { height: 30px; min-height: 30px; border: 2px solid " + borderColor2 + "; border-radius: 4px; padding-left: 3px; padding-right: 3px; }";
	mGlobalStyleSheet += "QComboBox:hover { border-color: " + borderColor1 + "; }";
	mGlobalStyleSheet += "QComboBox::drop-down { width: 24px; min-width: 24px; height: 30px; min-height: 30px; top: -2px; left: 2px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QComboBox::drop-down:hover { background: " + selectedGradient + "; }";
	mGlobalStyleSheet += "QComboBox::down-arrow { image: url(" + downIconNormal + "); }";
	mGlobalStyleSheet += "QComboBox QAbstractItemView { padding: 3px; min-height: 24px; border: 2px solid " + borderColor1 + "; border-radius: 4px; }";

	// QSpinBox
	mGlobalStyleSheet += "QSpinBox { height: 30px; min-height: 30px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QSpinBox:hover { border-color: " + borderColor1 + "; }";
	mGlobalStyleSheet += "QSpinBox::up-button { width: 24px; min-width: 24px; height: 14px; min-height: 14px; margin-right: -2px; margin-top: -2px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QSpinBox::up-button:hover { background: " + selectedGradient + "; }";
	mGlobalStyleSheet += "QSpinBox::up-arrow { image: url(" + upIconNormal + "); }";
	mGlobalStyleSheet += "QSpinBox::down-button { width: 24px; min-width: 24px; height: 14px; min-height: 14px; margin-right: -2px; margin-bottom: -2px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QSpinBox::down-button:hover { background: " + selectedGradient + "; }";
	mGlobalStyleSheet += "QSpinBox::down-arrow { image: url(" + downIconNormal + "); }";

	// QDoubleSpinBox
	mGlobalStyleSheet += "QDoubleSpinBox { height: 30px; min-height: 30px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QDoubleSpinBox:hover { border-color: " + borderColor1 + "; }";
	mGlobalStyleSheet += "QDoubleSpinBox::up-button { width: 24px; min-width: 24px; height: 14px; min-height: 14px; margin-right: -2px; margin-top: -2px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QDoubleSpinBox::up-button:hover { background: " + selectedGradient + "; }";
	mGlobalStyleSheet += "QDoubleSpinBox::up-arrow { image: url(" + upIconNormal + "); }";
	mGlobalStyleSheet += "QDoubleSpinBox::down-button { width: 24px; min-width: 24px; height: 14px; min-height: 14px; margin-right: -2px; margin-bottom: -2px; border: 2px solid " + borderColor2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QDoubleSpinBox::down-button:hover { background: " + selectedGradient + "; }";
	mGlobalStyleSheet += "QDoubleSpinBox::down-arrow { image: url(" + downIconNormal + "); }";

	// QLineEdit
	mGlobalStyleSheet += "QLineEdit { height: 30px; min-height: 30px; border: 2px solid " + borderColor2 + "; border-radius: 4px; padding-left: 3px; padding-right: 3px; }";
	mGlobalStyleSheet += "QLineEdit:hover { border-color: " + borderColor1 + "; }";

	// QTabWidget
	mGlobalStyleSheet += "QTabWidget:pane { border: 2px solid " + borderColor2 + "; border-top-right-radius: 4px; border-bottom-left-radius: 4px; border-bottom-right-radius: 4px; top: -2px; }";
	mGlobalStyleSheet += "QTabBar::tab { background: " + normalGradient + "; border: 2px solid " + borderColor2 + "; border-top-left-radius: 4px; border-top-right-radius: 4px; padding-left: 6px; padding-right: 6px; padding-top: 3px; padding-bottom: 3px; " + font.styleSheet() + "}";
	mGlobalStyleSheet += "QTabBar::tab:!selected { margin-top: 2px; }";
	mGlobalStyleSheet += "QTabBar::tab:!first { margin-left: -2px; }";
	mGlobalStyleSheet += "QTabBar::tab:selected { margin-left: -2px; margin-right: -2px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + buttonColor1 + ", stop: 0.4 " + buttonColor2 + ", stop: 0.6 " + buttonColor2 + ", stop: 1.0 " + buttonColor1 + "); }";
	mGlobalStyleSheet += "QTabBar::tab:hover { background: " + selectedGradient + " }";
	mGlobalStyleSheet += "QTabBar::tab:first:selected { margin-left: 0; }";
	mGlobalStyleSheet += "QTabBar::tab:last:selected { margin-right: 0; }";
	mGlobalStyleSheet += "QTabBar::tab:only-one { margin: 0; }";

	// QListWidget
	mGlobalStyleSheet += "QListWidget { border: 2px solid " + borderColor2 + "; border-radius: 4px; padding: 3px; }";

	// QCheckBox
	mGlobalStyleSheet += "QCheckBox { min-height: 30px; }";

	// QRadioButton
	mGlobalStyleSheet += "QRadioButton { min-height: 30px; }";

	// QLabel
	mGlobalStyleSheet += "QLabel { min-height: 30px; text-align: left center; }";

	// QGroupBox
	font.setColor(borderColor2);
	mGlobalStyleSheet += "QGroupBox { margin-top: 1ex; border: 1px solid " + borderColor2 + "; border-radius: 4px; font-weight: bold; }";
	mGlobalStyleSheet += "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0px 3px; " + font.styleSheet() + "; }";

}

#ifdef __cplusplus
extern "C"
{
#endif

ViThemePointer createTheme()
{
	return viPointerCast<ViTheme>(ViVulcanoTheme::instance());
}

#ifdef __cplusplus
}
#endif
