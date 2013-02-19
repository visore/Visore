#ifndef VIWAVEWIDGETGROUP_H
#define VIWAVEWIDGETGROUP_H

#include <viwavewidget.h>
#include <QSet>

class ViWaveWidgetGroup : public QObject
{

	Q_OBJECT

	private slots:

		void changePointer(qint32 position);
		void changeZoom(qint16 level);
		void changePosition(ViAudioPosition position);
		void showToolbars();
		void hideToolbars();

	public:

		enum Action
		{
			Zoom,
			Pointer,
			Position,
			Toolbars
		};

	public:

		ViWaveWidgetGroup();

		void addWidget(ViWaveWidget *widget);
		bool removeWidget(ViWaveWidget *widget);
		bool hasWidget(ViWaveWidget *widget);

		void enableAction(ViWaveWidgetGroup::Action action, bool enable = true);
		void disableAction(ViWaveWidgetGroup::Action action, bool disable = true);

	private:

		void updateBindings();
		void blockSignals();
		void unblockSignals();

	private:

		QSet<ViWaveWidget*> mWidgets;
		QSet<ViWaveWidgetGroup::Action> mActions;
		
};

#endif
