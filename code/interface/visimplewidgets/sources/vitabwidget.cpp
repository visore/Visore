#include <vitabwidget.h>
#include <vithememanager.h>
#include <QTabBar>

ViTabWidget::ViTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	tabBar()->setCursor(Qt::PointingHandCursor);

	ViFont font;
	font.setFamily("Harabara");
	font.setPointSize(18);
	font.setBold(true);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	font.setColor(ViThemeManager::color(ViThemeColors::ButtonTextColor1));

	QString backgroundColor1 = ViThemeManager::color(ViThemeColors::ButtonNormalColor1).name();
	QString backgroundColor2 = ViThemeManager::color(ViThemeColors::ButtonNormalColor2).name();

	setStyleSheet("\
		QTabWidget:pane\
		{\
			border: 2px solid " + ViThemeManager::color(ViThemeColors::BorderColor1).name() + ";\
			border-top-right-radius: 5px;\
			border-bottom-left-radius: 5px;\
			border-bottom-right-radius: 5px;\
			top: -2px;\
		}\
		QTabBar::tab\
		{\
			background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.05 " + backgroundColor1 + ", stop: 0.5 " + backgroundColor2 + ", stop: 0.95 " + backgroundColor1 + ", stop: 1.0 " + backgroundColor1 + ");\
			border: 2px solid " + ViThemeManager::color(ViThemeColors::BorderColor1).name() + ";\
			border-top-left-radius: 5px;\
			border-top-right-radius: 5px;\
			padding-left: 6px;\
			padding-right: 6px;\
			padding-top: 3px;\
			padding-bottom: 3px;\
			" + font.styleSheet() + "\
		}\
		QTabBar::tab:!selected\
		{\
			margin-top: 2px; /* make non-selected tabs look smaller */\
		}\
		/* make use of negative margins for overlapping tabs */\
		QTabBar::tab:selected\
		{\
			background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.4 " + backgroundColor2 + ", stop: 0.6 " + backgroundColor2 + ", stop: 1.0 " + backgroundColor1 + ");\
			/* expand/overlap to the left and right by 4px */\
			margin-left: -2px;\
			margin-right: -2px;\
		}\
		QTabBar::tab:hover\
		{\
			background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 " + backgroundColor1 + ", stop: 0.4 " + backgroundColor2 + ", stop: 0.6 " + backgroundColor2 + ", stop: 1.0 " + backgroundColor1 + ");\
		}\
		QTabBar::tab:first:selected\
		{\
			margin-left: 0; /* the first selected tab has nothing to overlap with on the left */\
		}\
		QTabBar::tab:last:selected\
		{\
			margin-right: 0; /* the last selected tab has nothing to overlap with on the right */\
		}\
		QTabBar::tab:only-one\
		{\
			margin: 0; /* if there is only one tab, we don't want overlapping margins */\
		}\
	");
}
