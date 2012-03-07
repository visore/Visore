#ifndef VIWINDOWCONTROLWIDGET_H
#define VIWINDOWCONTROLWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class ViWindowControlWidget : public QWidget
{
	Q_OBJECT

	public:
		enum ViWindowControlType
		{
			TopRight = 0,
			TopLeft = 1,
			BottomRight = 2,
			BottomLeft = 3,
			Top = 4,
			Bottom = 5,
			Left = 6,
			Right = 7,
			Move = 8
		};
		ViWindowControlWidget(QWidget *parent = 0);
		void setParameters(QWidget *parent, ViWindowControlWidget::ViWindowControlType type, int size);

	protected:
		void setResizeType(ViWindowControlWidget::ViWindowControlType type);
		void setMainParent(QWidget *parent);
		void setSize(int size);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private:
		ViWindowControlWidget::ViWindowControlType mType;
		QWidget *mParent;
		bool mIsSelected;
		QPoint mOldPosition;
};

#endif
