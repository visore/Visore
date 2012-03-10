#include "viwindow.h"
#include "ui_viwindow.h"

#include <iostream>
using namespace std;

ViWindow::ViWindow(QMainWindow *parent)
	: QWidget(parent)
{
	mUi = new Ui::ViWindow();
	mUi->setupUi(this);

	mParent = parent;
	//mParent->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	mUi->bottomRight->setParameters(parent, ViWindowControlWidget::BottomRight, 5);
	mUi->topRight->setParameters(parent, ViWindowControlWidget::TopRight, 5);
	mUi->bottomLeft->setParameters(parent, ViWindowControlWidget::BottomLeft, 5);
	mUi->topLeft->setParameters(parent, ViWindowControlWidget::TopLeft, 5);
	mUi->right->setParameters(parent, ViWindowControlWidget::Right, 5);
	mUi->left->setParameters(parent, ViWindowControlWidget::Left, 5);
	mUi->top->setParameters(parent, ViWindowControlWidget::Top, 1);
	mUi->bottom->setParameters(parent, ViWindowControlWidget::Bottom, 5);
	mUi->titleBar->setParameters(parent, ViWindowControlWidget::Move, ICON_SIZE / 2);

	mUi->closeButton->setSize(13, 13);
	mUi->fullscreenButton->setSize(13, 13);
	mUi->maximizeButton->setSize(13, 13);
	mUi->minimizeButton->setSize(13, 13);

	mUi->closeButton->setNormalIcon(ViThemeManager::image("close.png", ViThemeManager::Normal));
	mUi->fullscreenButton->setNormalIcon(ViThemeManager::image("fullscreen.png", ViThemeManager::Normal));
	mUi->maximizeButton->setNormalIcon(ViThemeManager::image("maximize.png", ViThemeManager::Normal));
	mUi->minimizeButton->setNormalIcon(ViThemeManager::image("minimize.png", ViThemeManager::Normal));

	mUi->closeButton->setHoverIcon(ViThemeManager::image("close.png", ViThemeManager::Colored));
	mUi->fullscreenButton->setHoverIcon(ViThemeManager::image("fullscreen.png", ViThemeManager::Colored));
	mUi->maximizeButton->setHoverIcon(ViThemeManager::image("maximize.png", ViThemeManager::Colored));
	mUi->minimizeButton->setHoverIcon(ViThemeManager::image("minimize.png", ViThemeManager::Colored));

	mUi->closeButton->setSelectedIcon(ViThemeManager::image("close.png", ViThemeManager::Colored));
	mUi->fullscreenButton->setSelectedIcon(ViThemeManager::image("fullscreen.png", ViThemeManager::Colored));
	mUi->maximizeButton->setSelectedIcon(ViThemeManager::image("maximize.png", ViThemeManager::Colored));
	mUi->minimizeButton->setSelectedIcon(ViThemeManager::image("minimize.png", ViThemeManager::Colored));

	mUi->maximizeButton->setCheckable();
	mUi->fullscreenButton->setCheckable();

	ViObject::connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(close()));
	ViObject::connect(mUi->minimizeButton, SIGNAL(clicked()), this, SLOT(minimize()));
	ViObject::connect(mUi->maximizeButton, SIGNAL(clicked(bool)), this, SLOT(maximize(bool)));
	ViObject::connect(mUi->fullscreenButton, SIGNAL(clicked(bool)), this, SLOT(fullscreen(bool)));
}

ViWindow::~ViWindow()
{
	delete mUi;
}

void ViWindow::close()
{
	mParent->close();
}

void ViWindow::maximize(bool value)
{
	mUi->fullscreenButton->setChecked(false);
	if(value)
	{
		mParent->showNormal(); // Required, otherwise Qt goes into normal mode
		mParent->showMaximized();
	}
	else
	{
		mParent->showNormal();
	}
}

void ViWindow::minimize()
{
	mUi->maximizeButton->setChecked(false);
	mUi->fullscreenButton->setChecked(false);
	mParent->showMinimized();
}

void ViWindow::fullscreen(bool value)
{
	mUi->maximizeButton->setChecked(false);
	if(value)
	{
		mParent->showFullScreen();
	}
	else
	{
		mParent->showNormal();
	}
}

void ViWindow::calculateClipping()
{
	mClipper = QPainterPath();
	QPainterPath clipper2;

	mClipper.setFillRule(Qt::WindingFill);
	clipper2.moveTo(BORDER_WIDTH, ICON_SIZE / 2);
	clipper2.cubicTo(ICON_SIZE, 2 * ICON_SIZE, ICON_SIZE, ICON_SIZE / 2, 4 * ICON_SIZE, ICON_SIZE / 2);
	mClipper.addRect(BORDER_WIDTH, BORDER_WIDTH, ICON_SIZE / 2 + BORDER_WIDTH, ICON_SIZE + 2);
	clipper2.addPath(mClipper);

	QPainterPath clipper3 = mClipper.intersected(clipper2);
	clipper2.addPath(clipper3);
	clipper2.addRect(BORDER_WIDTH, BORDER_WIDTH, ICON_SIZE / 2 + BORDER_WIDTH, ICON_SIZE + 2);

	QPainterPath clipper4 = clipper2.intersected(clipper2);
	mClipper.addPath(clipper4);
	mClipper.addRect(BORDER_WIDTH, 0, width() - 2 * BORDER_WIDTH, ICON_SIZE / 2);
	mClipper.addRect(4 * ICON_SIZE, ICON_SIZE / 2, width() - 4 * ICON_SIZE - BORDER_WIDTH, 1);
	mClipper.addRect(4 * ICON_SIZE - 21, ICON_SIZE / 2 + 1, width() - 4 * ICON_SIZE - BORDER_WIDTH + 21, 1);
	mClipper.addRect(width() - BORDER_WIDTH, 0, BORDER_WIDTH, height());
	mClipper.addRect(0, 0,  BORDER_WIDTH, height());
	mClipper.addRect(BORDER_WIDTH, height() - BORDER_WIDTH, width() - BORDER_WIDTH, BORDER_WIDTH);

	repaint();
}

void ViWindow::resizeEvent(QResizeEvent *event)
{
	calculateClipping();
}

void ViWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	QLinearGradient linearGradient(QPointF(0, 0), QPointF(ICON_SIZE, ICON_SIZE * 2));
	linearGradient.setColorAt(0, ViThemeManager::color(13));
	linearGradient.setColorAt(0.2, ViThemeManager::color(14));
	linearGradient.setColorAt(1, ViThemeManager::color(1));
	painter.setBrush(linearGradient);
	painter.setPen(Qt::NoPen);

	QImage image = ViThemeManager::background("tile.png");
	for(int i = 0; i < width() / image.width() + 1; ++i)
	{
		for(int j = 0; j < height() / image.height() + 1; ++j)
		{
			painter.drawImage(QRect(i * image.width(), j * image.height(), image.width(), image.height()), image);
		}
	}

	painter.setClipPath(mClipper);
	painter.drawRect(0, 0, width(), height());

	painter.drawImage(QRect(2 * BORDER_WIDTH, 2 * BORDER_WIDTH, 40 - 2 * BORDER_WIDTH, 40 - 2 * BORDER_WIDTH), ViThemeManager::image("logo.gif"));
}

void ViWindow::setContent(QWidget *widget)
{
	mUi->content->layout()->addWidget(widget);
}
