#include <vicorrelationwidget.h>
#include <ui_vicorrelationwidget.h>
#include <viglobalcorrelation.h>

ViCorrelationWidget::ViCorrelationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViCorrelationWidget();
	mUi->setupUi(this);
    mProject = NULL;

	mUi->correctedTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mUi->correctedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mUi->correctedTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	mUi->corruptedTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mUi->corruptedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mUi->corruptedTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    ViFont font;
    font.setPointSize(16);
    font.setBold(true);
	font.setColor(ViThemeManager::color(ViThemeColors::BorderColor2));

    mUi->currentMean->setStyleSheet(font.styleSheet());
    mUi->currentMinimum->setStyleSheet(font.styleSheet());
    mUi->currentMaximum->setStyleSheet(font.styleSheet());

    mUi->globalMean->setStyleSheet(font.styleSheet());
    mUi->globalMinimum->setStyleSheet(font.styleSheet());
    mUi->globalMaximum->setStyleSheet(font.styleSheet());

    mUi->correctedLabel->setStyleSheet(font.styleSheet());
    mUi->corruptedLabel->setStyleSheet(font.styleSheet());

    font.setPointSize(20);
    mUi->currentImprovement->setStyleSheet(font.styleSheet());
    mUi->globalImprovement->setStyleSheet(font.styleSheet());
}

ViCorrelationWidget::~ViCorrelationWidget()
{
	clear();
	delete mUi;
}

void ViCorrelationWidget::clear()
{
	mObjects.clear();
	mProject = NULL;

    mUi->correctedTable->clearContents();
    mUi->corruptedTable->clearContents();

    mUi->currentImprovement->setText(percentage(0));
    mUi->currentMean->setText(percentage(0));
    mUi->currentMinimum->setText(percentage(0));
    mUi->currentMaximum->setText(percentage(0));
    mUi->currentId->setText("-");

    mUi->globalImprovement->setText(percentage(0));
    mUi->globalMean->setText(percentage(0));
    mUi->globalMinimum->setText(percentage(0));
    mUi->globalMaximum->setText(percentage(0));
    mUi->globalId->setText("-");

    mUi->correctedLabel->setText(percentage(0));
    mUi->corruptedLabel->setText(percentage(0));
}

void ViCorrelationWidget::setObject(ViAudioObjectPointer object)
{
	clear();
	mObjects.append(object);
}

void ViCorrelationWidget::setObjects(ViAudioObjectQueue objects)
{
	clear();
	mObjects = objects;
}

void ViCorrelationWidget::setProject(ViProject *project)
{
	setObjects(project->objects());
	mProject = project;
}

void ViCorrelationWidget::changeCorrelator(QString correlator)
{
	if(!mObjects.isEmpty())
    {
        ViCorrelation correctedCorrelation, corruptedCorrelation;
        ViGlobalCorrelation globalCorrelation;
        ViAudioObjectPointer object;
        int correctedCount = 0, corruptedCount = 0;

		bool hasCorrectedCorrelation = false, hasCorruptedCorrelation = false;

		for(int i = 0; i < mObjects.size(); ++i)
        {
			object = mObjects[i];

            correctedCorrelation = object->correlation(correlator, ViAudio::Target, ViAudio::Corrected);
            if(correctedCorrelation.isValid())
            {
				hasCorrectedCorrelation = true;
				mUi->correctedTable->setItem(correctedCount, 0, QString::number(object->sideNumber()));
				mUi->correctedTable->setItem(correctedCount, 1, QString::number(object->trackNumber()));
				mUi->correctedTable->setItem(correctedCount, 2, object->fileName(false));
                mUi->correctedTable->setItem(correctedCount, 3, percentage(correctedCorrelation.minimum()), Qt::AlignRight | Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 4, percentage(correctedCorrelation.maximum()), Qt::AlignRight | Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 5, percentage(correctedCorrelation.mean()), Qt::AlignRight | Qt::AlignVCenter);
                ++correctedCount;
                globalCorrelation.add(object->correlation(ViAudio::Target, ViAudio::Corrected));
            }

            corruptedCorrelation = object->correlation(correlator, ViAudio::Target, ViAudio::Corrupted);
            if(corruptedCorrelation.isValid())
            {
				hasCorruptedCorrelation = true;
				mUi->corruptedTable->setItem(corruptedCount, 0, QString::number(object->sideNumber()));
				mUi->corruptedTable->setItem(corruptedCount, 1, QString::number(object->trackNumber()));
				mUi->corruptedTable->setItem(corruptedCount, 2, object->fileName(false));
				mUi->corruptedTable->setItem(corruptedCount, 3, percentage(corruptedCorrelation.minimum()), Qt::AlignRight | Qt::AlignVCenter);
				mUi->corruptedTable->setItem(corruptedCount, 4, percentage(corruptedCorrelation.maximum()), Qt::AlignRight | Qt::AlignVCenter);
				mUi->corruptedTable->setItem(corruptedCount, 5, percentage(corruptedCorrelation.mean()), Qt::AlignRight | Qt::AlignVCenter);
                ++corruptedCount;
                globalCorrelation.add(object->correlation(ViAudio::Target, ViAudio::Corrupted));
            }
        }

		mUi->correctedGroupBox->setVisible(hasCorrectedCorrelation);
		mUi->corruptedGroupBox->setVisible(hasCorruptedCorrelation);

		if(hasCorrectedCorrelation && hasCorruptedCorrelation)
		{
			mUi->globalGroupBox->show();
			correctedCorrelation = globalCorrelation.correlation(ViAudio::Target, ViAudio::Corrected).correlation(correlator);
			corruptedCorrelation = globalCorrelation.correlation(ViAudio::Target, ViAudio::Corrupted).correlation(correlator);

			if(corruptedCorrelation.isValid())
			{
				mUi->corruptedLabel->setText(percentage(corruptedCorrelation.mean()));
			}

			if(correctedCorrelation.isValid())
			{
				mUi->correctedLabel->setText(percentage(correctedCorrelation.mean()));
				mUi->currentMean->setText(percentage(correctedCorrelation.mean()));
				mUi->currentMinimum->setText(percentage(correctedCorrelation.minimum()));
				mUi->currentMaximum->setText(percentage(correctedCorrelation.maximum()));
			}

			if(mProject == NULL)
			{
				mUi->globalContainer->hide();
			}
			else
			{
				mUi->globalContainer->show();

				ViFont font;
				font.setPointSize(20);
				font.setBold(true);

				if(correctedCorrelation.isValid() && corruptedCorrelation.isValid())
				{
					qreal imporovement = (correctedCorrelation.mean() - corruptedCorrelation.mean()) / (1 - corruptedCorrelation.mean());
					if(imporovement < 0) font.setColor(Qt::red);
					else if(imporovement > 0) font.setColor(Qt::green);
					else font.setColor(ViThemeManager::color(ViThemeColors::TextColor1));

					mUi->currentImprovement->setStyleSheet(font.styleSheet());
					mUi->currentImprovement->setText(percentage(imporovement));
					mUi->currentId->setText(mProject->currentCorrectionId());
				}

				correctedCorrelation = mProject->bestCorrelation(correlator);
				if(correctedCorrelation.isValid())
				{
					qreal imporovement = mProject->bestImprovement(correlator);
					if(imporovement < 0) font.setColor(Qt::red);
					else if(imporovement > 0) font.setColor(Qt::green);
					else font.setColor(ViThemeManager::color(ViThemeColors::TextColor1));

					mUi->globalImprovement->setStyleSheet(font.styleSheet());
					mUi->globalImprovement->setText(percentage(imporovement));

					ViCorrelation best = mProject->bestCorrelation(correlator);
					mUi->globalMean->setText(percentage(best.mean()));
					mUi->globalMinimum->setText(percentage(best.minimum()));
					mUi->globalMaximum->setText(percentage(best.maximum()));
					mUi->globalId->setText(mProject->bestCorrectionId(correlator));

				}
			}
		}
		else
		{
			mUi->globalGroupBox->hide();
		}
    }
}

QString ViCorrelationWidget::percentage(qreal value)
{
    return QString::number(value * 100, 'f', 5) + "%";
}

void ViCorrelationWidget::adjustTables()
{
    /*int size = mUi->tableWidget->horizontalHeader()->height();
	for(int i = 0; i < mUi->tableWidget->rowCount(); ++i)
	{
		size += mUi->tableWidget->rowHeight(i);
		size += mUi->tableWidget->contentsMargins().bottom();
	}
    mUi->tableWidget->setFixedHeight(size);*/
}
