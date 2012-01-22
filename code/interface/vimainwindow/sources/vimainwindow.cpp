#include "vimainwindow.h"

ViMainWindow::ViMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	mLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	m1 = new QPushButton("Start Input");
	m2 = new QPushButton("Pause Input");
	m3 = new QPushButton("Stop Input");
	m4 = new QPushButton("Start Output");
	m5 = new QPushButton("Pause Output");
	m6 = new QPushButton("Stop Output");
	mLayout->addWidget(m1);
	mLayout->addWidget(m2);
	mLayout->addWidget(m3);
	mLayout->addSpacing(60);
	mLayout->addWidget(m4);
	mLayout->addWidget(m5);
	mLayout->addWidget(m6);
	mWidget = new QWidget();
	mWidget->setLayout(mLayout);
	mWidget->resize(800,800);
	setCentralWidget(mWidget);
	resize(800,800);
}

void ViMainWindow::setEngine(ViAudioEngine *engine)
{
	mEngine = engine;
	QObject::connect(m1, SIGNAL(clicked()), mEngine, SLOT(startInput()));
	QObject::connect(m2, SIGNAL(clicked()), mEngine, SLOT(pauseInput()));
	QObject::connect(m3, SIGNAL(clicked()), mEngine, SLOT(stopInput()));
	QObject::connect(m4, SIGNAL(clicked()), mEngine, SLOT(startOutput()));
	QObject::connect(m5, SIGNAL(clicked()), mEngine, SLOT(pauseOutput()));
	QObject::connect(m6, SIGNAL(clicked()), mEngine, SLOT(stopOutput()));
}

#ifdef __cplusplus
extern "C"
{
#endif

QMainWindow* createObject(ViAudioEngine *engine)
{
	ViMainWindow *window = new ViMainWindow();
	window->setEngine(engine);
	return window;
}

#ifdef __cplusplus
}
#endif
