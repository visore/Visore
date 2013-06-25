#include "vicorrelationwidget.h"
#include "ui_vicorrelationwidget.h"
#include <viglobalcorrelation.h>
#include <vicorrelatormanager.h>

ViCorrelationWidget::ViCorrelationWidget(QWidget *parent)
	: ViWidget(parent)
{
	mUi = new Ui::ViCorrelationWidget();
	mUi->setupUi(this);
    mProject = NULL;

    mUi->correctedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mUi->correctedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mUi->correctedTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    mUi->corruptedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mUi->corruptedTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QObject::connect(mUi->correlatorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCorrelator()));

    ViFont font;
    font.setPointSize(16);
    font.setBold(true);
    font.setColor(ViThemeManager::color(ViThemeColors::TextColor1));

    mUi->meanLabel->setStyleSheet(font.styleSheet());
    mUi->minimumLabel->setStyleSheet(font.styleSheet());
    mUi->maximumLabel->setStyleSheet(font.styleSheet());
    mUi->correctedLabel->setStyleSheet(font.styleSheet());
    mUi->corruptedLabel->setStyleSheet(font.styleSheet());

    font.setPointSize(20);
    mUi->improvementLabel->setStyleSheet(font.styleSheet());
}

ViCorrelationWidget::~ViCorrelationWidget()
{
	delete mUi;
}

void ViCorrelationWidget::clear()
{
    mUi->correctedTable->clearContents();
    mUi->corruptedTable->clearContents();

    mUi->correlatorComboBox->clear();

    mUi->improvementLabel->setText(percentage(0));
    mUi->meanLabel->setText(percentage(0));
    mUi->minimumLabel->setText(percentage(0));
    mUi->maximumLabel->setText(percentage(0));
    mUi->correctedLabel->setText(percentage(0));
    mUi->corruptedLabel->setText(percentage(0));
}

void ViCorrelationWidget::setProject(ViProject *project)
{
    mProject = project;
    if(mProject != NULL)
    {
        clear();
        updateCorrelators();
    }
}

void ViCorrelationWidget::updateCorrelators()
{
    ViCorrelations correlations;
    bool contains;
    for(int i = 0; i < mProject->objectCount(); ++i)
    {
        correlations = mProject->object(i)->correlations();
        for(int j = 0; j < correlations.size(); ++j)
        {
            contains = false;
            for(int k = 0; k < mUi->correlatorComboBox->count(); ++k)
            {
                if(mUi->correlatorComboBox->itemText(k) == correlations[j].correlator())
                {
                    contains = true;
                    break;
                }
            }
            if(!contains)
            {
                mUi->correlatorComboBox->addItem(correlations[j].correlator());
            }
        }
    }
    mUi->correlatorComboBox->setCurrentText(ViCorrelatorManager::defaultName());
}

void ViCorrelationWidget::changeCorrelator()
{
    if(mProject != NULL)
    {
        QString correlator = mUi->correlatorComboBox->currentText();

        ViCorrelation correctedCorrelation, corruptedCorrelation;
        ViGlobalCorrelation globalCorrelation;
        ViAudioObjectPointer object;
        int correctedCount = 0, corruptedCount = 0;

        for(int i = 0; i < mProject->objectCount(); ++i)
        {
            object = mProject->object(i);

            correctedCorrelation = object->correlation(correlator, ViAudio::Target, ViAudio::Corrected);
            if(correctedCorrelation.isValid())
            {
                mUi->correctedTable->setItem(correctedCount, 0, QString::number(object->sideNumber()), Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 1, QString::number(object->trackNumber()), Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 2, object->fileName(false), Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 3, percentage(correctedCorrelation.minimum()), Qt::AlignRight | Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 4, percentage(correctedCorrelation.maximum()), Qt::AlignRight | Qt::AlignVCenter);
                mUi->correctedTable->setItem(correctedCount, 5, percentage(correctedCorrelation.mean()), Qt::AlignRight | Qt::AlignVCenter);
                ++correctedCount;
                globalCorrelation.add(correctedCorrelation);
            }

            corruptedCorrelation = object->correlation(correlator, ViAudio::Target, ViAudio::Corrupted);
            if(corruptedCorrelation.isValid())
            {
                mUi->corruptedTable->setItem(corruptedCount, 0, QString::number(object->sideNumber()), Qt::AlignVCenter);
                mUi->corruptedTable->setItem(corruptedCount, 1, QString::number(object->trackNumber()), Qt::AlignVCenter);
                mUi->corruptedTable->setItem(corruptedCount, 2, object->fileName(false), Qt::AlignVCenter);
                mUi->corruptedTable->setItem(corruptedCount, 3, percentage(corruptedCorrelation.minimum()), Qt::AlignRight | Qt::AlignVCenter);
                mUi->corruptedTable->setItem(corruptedCount, 4, percentage(corruptedCorrelation.maximum()), Qt::AlignRight | Qt::AlignVCenter);
                mUi->corruptedTable->setItem(corruptedCount, 5, percentage(corruptedCorrelation.mean()), Qt::AlignRight | Qt::AlignVCenter);
                ++corruptedCount;
                globalCorrelation.add(corruptedCorrelation);
            }
        }

        correctedCorrelation = globalCorrelation.correlation(correlator, ViAudio::Target, ViAudio::Corrected);
        corruptedCorrelation = globalCorrelation.correlation(correlator, ViAudio::Target, ViAudio::Corrupted);

        if(corruptedCorrelation.isValid())
        {
            mUi->corruptedLabel->setText(percentage(corruptedCorrelation.mean()));
        }

        if(correctedCorrelation.isValid())
        {
            mUi->correctedLabel->setText(percentage(correctedCorrelation.mean()));
            mUi->meanLabel->setText(percentage(correctedCorrelation.mean()));
            mUi->minimumLabel->setText(percentage(correctedCorrelation.minimum()));
            mUi->maximumLabel->setText(percentage(correctedCorrelation.maximum()));
        }

        if(correctedCorrelation.isValid() && corruptedCorrelation.isValid())
        {
            ViFont font;
            font.setPointSize(20);
            font.setBold(true);

            qreal imporovement = correctedCorrelation.mean() - corruptedCorrelation.mean();
            if(imporovement < 0) font.setColor(Qt::red);
            else if(imporovement > 0) font.setColor(Qt::green);
            else font.setColor(ViThemeManager::color(ViThemeColors::TextColor1));

            mUi->improvementLabel->setStyleSheet(font.styleSheet());
            mUi->improvementLabel->setText(percentage(imporovement));
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
