#include <viwavecontroltoolbar.h>
#include <vibutton.h>

ViWaveControlToolbar::ViWaveControlToolbar(QWidget *parent)
	: ViWidgetToolbar(Qt::AlignCenter | Qt::AlignRight, parent)
{
	ViButton *button;

	button = new ViButton(this);
	button->setSize(36, 36);
	button->setIcon(ViThemeManager::icon("zoomin"), 24);
	addWidget(button);
	QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(zoomedIn()));

	button = new ViButton(this);
	button->setSize(36, 36);
	button->setIcon(ViThemeManager::icon("zoomout"), 24);
	addWidget(button);
	QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(zoomedOut()));

	button = new ViButton(this);
	button->setSize(36, 36);
	ViThemeIcon icon = ViThemeManager::icon("zoomout");
	icon.set(ViThemeIcon::Selected, ViThemeManager::icon("zoomin"));
	button->setIcon(icon , 24);
	addWidget(button);
	QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(toggledGroup()));
	
}
