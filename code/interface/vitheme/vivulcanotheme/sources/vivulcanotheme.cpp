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

	QString orange1 = mColors->color(ViThemeColors::BorderColor1).name();
	QString orange2 = mColors->color(ViThemeColors::BorderColor2).name();

	QString downIconNormal = ViThemeIcon("down").path(ViThemeIcon::Normal);
	QString upIconNormal = ViThemeIcon("up").path(ViThemeIcon::Normal);

	//http://qt-project.org/doc/qt-5.0/qtwidgets/stylesheet-examples.html#customizing-qcombobox

	// QCombobBox
	mGlobalStyleSheet += "QComboBox { height: 30px; min-height: 30px; border: 2px solid " + orange2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QComboBox:hover { border-color: " + orange1 + "; }";
	mGlobalStyleSheet += "QComboBox::drop-down { width: 24px; min-width: 24px; height: 30px; min-height: 30px; top: -2px; left: 2px; border: 2px solid " + orange2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QComboBox::drop-down:hover { background: " + orange1 + "; }";
	mGlobalStyleSheet += "QComboBox::down-arrow { image: url(" + downIconNormal + "); }";
	mGlobalStyleSheet += "QComboBox QAbstractItemView { border: 2px solid " + orange1 + "; border-radius: 4px; }";
	//

	// QSpinBox
	mGlobalStyleSheet += "QSpinBox { height: 30px; min-height: 30px; border: 2px solid " + orange2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QSpinBox:hover { border-color: " + orange1 + "; }";
	mGlobalStyleSheet += "QSpinBox::up-button { width: 24px; min-width: 24px; height: 14px; min-height: 14px; margin-right: -2px; margin-top: -2px; border: 2px solid " + orange2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QSpinBox::up-button:hover { background: " + orange1 + "; }";
	mGlobalStyleSheet += "QSpinBox::up-arrow { image: url(" + upIconNormal + "); }";
	mGlobalStyleSheet += "QSpinBox::down-button { width: 24px; min-width: 24px; height: 14px; min-height: 14px; margin-right: -2px; margin-bottom: -2px; border: 2px solid " + orange2 + "; border-radius: 4px; }";
	mGlobalStyleSheet += "QSpinBox::down-button:hover { background: " + orange1 + "; }";
	mGlobalStyleSheet += "QSpinBox::down-arrow { image: url(" + downIconNormal + "); }";
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
