#include <viwavecontroltoolbar.h>
#include <vibutton.h>

ViWaveControlToolbar::ViWaveControlToolbar(QWidget *parent)
	: ViWidgetToolbar(Qt::AlignCenter | Qt::AlignRight, parent)
{
	ViButton *button;

	button = new ViButton(this);
	button->setSize(36, 36);
	button->disableBackground();
	button->disableBorder();
	button->setIcon(ViThemeManager::icon("zoomin"), 24);
	addWidget(button);
	QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(zoomedIn()));

	button = new ViButton(this);
	button->setSize(36, 36);
	button->disableBackground();
	button->disableBorder();
	button->setIcon(ViThemeManager::icon("zoomout"), 24);
	addWidget(button);
	QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(zoomedOut()));

	button = new ViButton(this);
	button->setCheckable(true);
	button->setSize(36, 36);
	button->disableBackground();
	button->disableBorder();
	button->setIcon(ViThemeManager::icon("switch") , 24);
	addWidget(button);
	QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(toggledGroup()));
}
