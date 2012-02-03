#include "viwaveformwidget.h"

ViWaveFormWidgetThread::ViWaveFormWidgetThread(ViWaveFormWidget *widget)
	: QThread()
{
	mWidget = widget;
}

ViWaveFormWidgetThread::~ViWaveFormWidgetThread()
{
	/*for(int i = 0; i < mTiles.size(); ++i)
	{
		if(mTiles[i] != NULL)
		{
			delete mTiles[i];
			mTiles[i] = NULL;
		}
	}*/
}

void ViWaveFormWidgetThread::run()
{
	while(!mLists.isEmpty())
	{
		QList<double> list = mLists.first();
		mLists.removeFirst();
		for(int i = 0; i < list.size(); i += 100)
		{
			ViWaveFormTile *tile = new ViWaveFormTile(list.mid(i, 100));
			tile->draw(100);
			mTiles.append(tile);
			//mWidget->repaint();
			emit tileAvailable();
		}
	}
}

void ViWaveFormWidgetThread::changed(QList<double> list)
{
	mLists.append(list);
	if(!isRunning())
	{
		start();
	}
}


ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	repaint();
	mThread = new ViWaveFormWidgetThread(this);
	ViObject::connect(mEngine, SIGNAL(waveFormChanged(QList<double>)), mThread, SLOT(changed(QList<double>)));
	//ViObject::connect(mThread, SIGNAL(tileAvailable()), this, SLOT(receiveTile()));
	//ViObject::connectDirect(mThread, SIGNAL(tileAvailable()), this, SLOT(update()));
	//mThread->start();
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	delete mThread;
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{
	/*QPainter painter(this);
	//for(int i = 0; i < mThread->mTiles.size(); ++i)
	//{
	if(!mThread->mTiles.isEmpty())
	{
		QPixmap pixmap;
if(mThread->mTiles.last()->tile()==NULL)cout<<"PP"<<endl;
		pixmap.convertFromImage(*mThread->mTiles.last()->tile());
		//painter.drawPixmap(0, 0, pixmap);
	}*/
	//}
}

void ViWaveFormWidget::receiveTile()
{
	
}
