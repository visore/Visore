#include "vislider.h"

ViSlider::ViSlider(QWidget *parent)
	: QSlider(parent)
{
	QString color1 = ViThemeManager::color(ViThemeColors::MainColor3).name();
	QString color2 = ViThemeManager::color(ViThemeColors::MainColor4).name();
	QString color3 = ViThemeManager::color(ViThemeColors::MainColor5).name();
	QString color4 = ViThemeManager::color(ViThemeColors::BorderColor1).name();
	QString color5 = ViThemeManager::color(ViThemeColors::BorderColor2).name();

	setStyleSheet("\
		\
		QSlider::groove:horizontal\
		{\
			border: 1px solid " + color4 + ";\
			height: 5px;\
			border-radius: 3px;\
		}\
		\
		QSlider::sub-page:horizontal\
		{\
			background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + color1 + ", stop: 1 " + color3 + ");\
			border: 1px solid " + color4 + ";\
			height: 10px;\
			border-radius: 3px;\
		}\
		\
		QSlider::sub-page:horizontal:disabled\
		{\
			background: #bbb;\
			border-color: " + color4 + ";\
		}\
		\
		QSlider::add-page:horizontal\
		{\
			background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + color4 + ", stop: 1 " + color5 + ");\
			border: 1px solid " + color4 + ";\
			height: 10px;\
			border-radius: 3px;\
		}\
		\
		QSlider::add-page:horizontal:disabled\
		{\
			background: #eee;\
			border-color: " + color4 + ";\
		}\
		\
		QSlider::handle:horizontal\
		{\
			background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 " + color4 + ", stop:1 " + color5 + ");\
			border: 1px solid " + color4 + ";\
			width: 20px;\
			margin-top: -4px;\
			margin-bottom: -4px;\
			margin-right: -1px;\
			margin-left: -1px;\
			border-radius: 5px;\
		}\
		\
		QSlider::handle:horizontal:hover\
		{\
			background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 " + color5 + ", stop:1 " + color4 + ");\
			border: 1px solid " + color1 + ";\
			border-radius: 6px;\
		}\
		\
		QSlider::handle:horizontal:pressed\
		{\
			background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " + color1 + ", stop: 1 " + color3 + ");\
		}\
		\
		QSlider::handle:horizontal:disabled\
		{\
			background: #eee;\
			border: 1px solid #aaa;\
			border-radius: 5px;\
		}\
		\
	");
}
