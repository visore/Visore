#include "viwaveformwidget.h"

ViWaveFormWidgetThread::ViWaveFormWidgetThread(ViWaveFormWidget *widget)
	: QThread()
{
	mWidget = widget;
	mPosition = -1;
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
			ViWaveFormTile *tile = new ViWaveFormTile();
			tile->draw(list.mid(i, 100), mWidget->height());
			mTiles.append(tile);
		}
		emit tileAvailable();
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

void ViWaveFormWidgetThread::positionChanged(qint64 bytes, qint64 milliseconds)
{
	mPosition = bytes / 200;
	//emit tileAvailable();
}


ViWaveFormWidget::ViWaveFormWidget(ViAudioEngine *engine, QWidget *parent)
	: ViWidget(engine, parent)
{
	mCurrentX = 0;
mPainter = NULL;
	mWaveForm = ViWaveFormWidget::Combined;
	mThread = new ViWaveFormWidgetThread(this);
	ViObject::connect(mEngine, SIGNAL(waveFormChanged(QList<double>)), mThread, SLOT(changed(QList<double>)));
	ViObject::connect(mEngine, SIGNAL(positionChanged(qint64, qint64)), mThread, SLOT(positionChanged(qint64, qint64)));
	//ViObject::connect(mThread, SIGNAL(tileAvailable()), this, SLOT(receiveTile()));
	QObject::connect(mThread, SIGNAL(tileAvailable()), this, SLOT(update()), Qt::QueuedConnection);
	//mThread->start();
}

ViWaveFormWidget::~ViWaveFormWidget()
{
	//delete mThread;
//delete mPainter;
}

void ViWaveFormWidget::paintEvent(QPaintEvent *event)
{

/*QPainter painter(this);
QPen pen(Qt::white);
painter.setPen(pen);
painter.fillRect(rect(), Qt::black);
int halfHeight = height() / 2;
		int i = mThread->mPosition - 100;
		if(i < 0)
		{
			i = 0;
		}

		int j = mThread->mPosition + 100;
		if(j > mThread->mList.size())
		{
			j = mThread->mList.size();
		}

		int counter = 0;
		if(mWaveForm == ViWaveFormWidget::Combined)
		{
			for(i; i < j; ++i)
			{
				painter.drawLine(counter, halfHeight, counter, halfHeight - (1000 * halfHeight * mThread->mList[i]));
				if(i % 2 != 0)
				{
					++counter;
				}
			}
		}
		else
		{
			for(i; i < j; ++i)
			{
				painter.drawLine(counter, halfHeight, counter, halfHeight - (halfHeight * mThread->mList[i]));
				++counter;
			}
		}

QPen pen2(Qt::red);
painter.setPen(pen2);
painter.drawLine(100, 0, 100, height());*/




if(mPainter == NULL)
{
	mPainter = new QPainter(this);
}



	while(!mThread->mTiles.isEmpty())
	{
		QPixmap pixmap;
		ViWaveFormTile *tile = mThread->mTiles.first();
		pixmap.convertFromImage(*tile->tile());
		mPainter->drawPixmap(mCurrentX, 0, pixmap);
		mCurrentX += tile->tile()->width();
		mThread->mTiles.removeFirst();
	}
	//}
/*
while(!mThread->mLists.isEmpty())
{
	QPen pen(Qt::white);
	painter.setPen(pen);
	int halfHeight = height() / 2;
	QList<double> list = mThread->mLists.first();
//	mThread->mLists.removeFirst();
cout<<mThread->mLists.size()<<endl;
	for(int i = 0; i < list.size(); ++i)
	{
		//painter.drawLine(i, halfHeight, i, halfHeight - (halfHeight * list[i]));
		//QList<double> mValues = list.mid(i, 100);
		for(int j = 0; j < mValues.size(); ++j)
		{
			painter.drawLine(j, halfHeight, j, halfHeight - (halfHeight * 100*mValues[j]));
		}
	}
}*/

}

ViWaveFormWidget::ViWaveForm ViWaveFormWidget::waveForm()
{
	return mWaveForm;
}

void ViWaveFormWidget::setWaveForm(ViWaveFormWidget::ViWaveForm form)
{
	mWaveForm = form;
}
