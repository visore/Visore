#include <viwavewidgetgroup.h>

ViWaveWidgetGroup::ViWaveWidgetGroup()
	: QObject()
{
	enableAction(ViWaveWidgetGroup::Pointer);
	enableAction(ViWaveWidgetGroup::Position);
	enableAction(ViWaveWidgetGroup::Toolbars);
}

void ViWaveWidgetGroup::addWidget(ViWaveWidget *widget)
{
	mWidgets.insert(widget);
	widget->setGroup(this);
	updateBindings();
}

bool ViWaveWidgetGroup::removeWidget(ViWaveWidget *widget)
{
	if(mWidgets.remove(widget))
	{
		widget->disconnect(this);
		return true;
	}
	return false;
}

bool ViWaveWidgetGroup::hasWidget(ViWaveWidget *widget)
{
	return mWidgets.contains(widget);
}

void ViWaveWidgetGroup::enableAction(ViWaveWidgetGroup::Action action, bool enable)
{
	if(enable)
	{
		mActions.insert(action);
	}
	else
	{
		mActions.remove(action);
	}
	updateBindings();
}

void ViWaveWidgetGroup::disableAction(ViWaveWidgetGroup::Action action, bool disable)
{
	if(disable)
	{
		mActions.remove(action);
	}
	else
	{
		mActions.insert(action);
	}
	updateBindings();
}

void ViWaveWidgetGroup::updateBindings()
{
	ViWaveWidget *widget;
	ViWaveWidgetGroup::Action action;
	foreach(widget, mWidgets)
	{
		widget->disconnect(this);
		foreach(action, mActions)
		{
			if(action == ViWaveWidgetGroup::Zoom)
			{
				QObject::connect(widget, SIGNAL(zoomChanged(qint16)), this, SLOT(changeZoom(qint16)));
			}
			else if(action == ViWaveWidgetGroup::Pointer)
			{
				QObject::connect(widget, SIGNAL(pointerChanged(qint32)), this, SLOT(changePointer(qint32)));
			}
			else if(action == ViWaveWidgetGroup::Position)
			{
				QObject::connect(widget, SIGNAL(positionChanged(ViAudioPosition)), this, SLOT(changePosition(ViAudioPosition)));
			}
			else if(action == ViWaveWidgetGroup::Toolbars)
			{
				QObject::connect(widget, SIGNAL(toolbarsShown()), this, SLOT(showToolbars()));
				QObject::connect(widget, SIGNAL(toolbarsHidden()), this, SLOT(hideToolbars()));
			}
		}
	}
}

void ViWaveWidgetGroup::blockSignals()
{
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->blockSignals(true);
	}
}

void ViWaveWidgetGroup::unblockSignals()
{
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->blockSignals(false);
	}
}

void ViWaveWidgetGroup::changePointer(qint32 position)
{
	blockSignals();
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->setPointer(position);
	}
	unblockSignals();
}

void ViWaveWidgetGroup::changeZoom(qint16 level)
{
	blockSignals();
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->setZoom(level);
	}
	unblockSignals();
}

void ViWaveWidgetGroup::changePosition(ViAudioPosition position)
{
	blockSignals();
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->setPosition(position);
	}
	unblockSignals();
}

void ViWaveWidgetGroup::showToolbars()
{
	blockSignals();
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->showToolbars();
	}
	unblockSignals();
}

void ViWaveWidgetGroup::hideToolbars()
{
	blockSignals();
	ViWaveWidget *widget;
	foreach(widget, mWidgets)
	{
		widget->hideToolbars();
	}
	unblockSignals();
}
