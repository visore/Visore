#include "vicorrelationwidget.h"
#include "ui_vicorrelationwidget.h"
#include "vimainwindow.h"

ViCorrelationWidget::ViCorrelationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViCorrelationWidget();
	mUi->setupUi(this);
	mWasInitialized = false;

	mUi->tableWidget->setRowCount(2);
	mUi->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	mUi->tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	
	mUi->tableWidget->setItem(0, 0, new QTableWidgetItem("Sample correlator"));
	mUi->tableWidget->setItem(0, 1, new QTableWidgetItem("-"));
	mUi->tableWidget->setItem(0, 2, new QTableWidgetItem("-"));
	mUi->tableWidget->setItem(0, 3, new QTableWidgetItem("-"));

	mUi->tableWidget->setItem(1, 0, new QTableWidgetItem("Cross correlator"));
	mUi->tableWidget->setItem(1, 1, new QTableWidgetItem("-"));
	mUi->tableWidget->setItem(1, 2, new QTableWidgetItem("-"));
	mUi->tableWidget->setItem(1, 3, new QTableWidgetItem("-"));

	QObject::connect(mEngine, SIGNAL(correlationFinished()), this, SLOT(update()));
	//QObject::connect(mEngine, SIGNAL(correlationProgressed(short)), ViMainWindow::instance(), SLOT(progress(short)));
}

ViCorrelationWidget::~ViCorrelationWidget()
{
	delete mUi;
}

void ViCorrelationWidget::showEvent(QShowEvent *event)
{
	if(!mWasInitialized)
	{
		mWasInitialized = true;
		recalculate();
	}
	ViWidget::showEvent(event);
}

void ViCorrelationWidget::recalculate()
{
	//ViLoadingWidget::show(true, false, ViLoadingWidget::Text, "Correlating Signals");
	mEngine->calculateCorrelation();
}

void ViCorrelationWidget::update()
{
	ViCorrelationResult &result = mEngine->correlation();

	mUi->tableWidget->item(0, 1)->setText(QString::number(result.sampleCorrelation(ViCorrelationResult::Worst) * 100, 'f', 5) + "%");
	mUi->tableWidget->item(0, 2)->setText(QString::number(result.sampleCorrelation(ViCorrelationResult::Best) * 100, 'f', 5) + "%");
	mUi->tableWidget->item(0, 3)->setText(QString::number(result.sampleCorrelation(ViCorrelationResult::Average) * 100, 'f', 5) + "%");

	mUi->tableWidget->item(1, 1)->setText(QString::number(result.crossCorrelation(ViCorrelationResult::Worst) * 100, 'f', 5) + "%");
	mUi->tableWidget->item(1, 2)->setText(QString::number(result.crossCorrelation(ViCorrelationResult::Best) * 100, 'f', 5) + "%");
	mUi->tableWidget->item(1, 3)->setText(QString::number(result.crossCorrelation(ViCorrelationResult::Average) * 100, 'f', 5) + "%");

	adjustTable();
	//ViLoadingWidget::hide();
}

void ViCorrelationWidget::adjustTable()
{
	int size = mUi->tableWidget->horizontalHeader()->height();
	for(int i = 0; i < mUi->tableWidget->rowCount(); ++i)
	{
		size += mUi->tableWidget->rowHeight(i);
		size += mUi->tableWidget->contentsMargins().bottom();
	}
	mUi->tableWidget->setFixedHeight(size);
}
