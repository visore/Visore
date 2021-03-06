#include <viloadingwidget.h>
#include <ui_viloadingwidget.h>
#include <vimainwindow.h>
#include <vitimeconverter.h>

#define TIME_UPDATE_INTERVAL 1000

ViLoadingWidget::ViLoadingWidget()
	: ViWidget(ViMainWindow::instance())
{
	mUi = new Ui::ViLoadingWidget();
	mUi->setupUi(this);

	setStyleSheet("ViLoadingWidget{background-color: rgba(0, 0, 0, 200);}");
	mUi->animationWidget->setAnimation(ViThemeManager::animation("loading"));

	mUi->cancelButton->setSize(50, 20);
	//mUi->cancelButton->setGlow(ViThemeManager::color(ViThemeColors::ButtonGlowColor1));
	//mUi->cancelButton->setIcon(ViThemeManager::image("record.png", ViThemeImage::Normal, ViThemeManager::Icon), ViThemeImage::Normal);

    QObject::connect(engine().data(), SIGNAL(progressed(qreal)), this, SLOT(progress(qreal)));
    QObject::connect(engine().data(), SIGNAL(progressFinished()), this, SLOT(stopProgress()));
	QObject::connect(engine().data(), SIGNAL(statusChanged(QString)), this, SLOT(setText(QString)));

	QColor textColor = ViThemeManager::color(ViThemeColors::TextColor1);
	mUi->elapsedHeading->setStyleSheet("color: " + textColor.name());
	mUi->remainingHeading->setStyleSheet("color: " + textColor.name());
	mUi->elapsedLabel->setStyleSheet("color: " + textColor.name());
	mUi->remainingLabel->setStyleSheet("color: " + textColor.name());

	showAnimation(true);
	showButton(false);
	setProgressStyle(ViProgressBar::Finite);

	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(updateTimes()));

	mUi->animationWidget->stop();
	hide();
}

ViLoadingWidget::~ViLoadingWidget()
{
    delete mUi;
}

void ViLoadingWidget::progress(qreal percentage)
{
	if(!isVisible())
	{
		start();
	}

	if(percentage < 0)
	{
		mUi->progressBar->setValue(100);
		enableRemainingTime(false);
	}
	else
	{
		mUi->progressBar->setValue(percentage);
	}

    /*if(percentage >= 100)
	{
		stop();
    }*/
}

void ViLoadingWidget::stopProgress()
{
    stop();
}

void ViLoadingWidget::updateTimes()
{
	mUi->elapsedLabel->setText(ViTimeConverter::toOptimalString(mTime.elapsed(), ViTimeConverter::Milliseconds));
	mUi->remainingLabel->setText(ViTimeConverter::toOptimalString(remainingTime(mTime.elapsed(), mUi->progressBar->value()), ViTimeConverter::Milliseconds));
}

void ViLoadingWidget::showAnimation(bool show)
{
	mHasAnimation = show;
	mUi->animationWidget->setVisible(mHasAnimation);
	if(show)
	{
		mUi->animationWidget->start(100);
	}
	else
	{
		mUi->animationWidget->stop();
	}
}

void ViLoadingWidget::showButton(bool show)
{
	mUi->cancelButton->setVisible(show);
}

void ViLoadingWidget::setTextStyle(ViProgressBar::TextStyle style)
{
	mUi->progressBar->setTextStyle(style);
}

void ViLoadingWidget::setProgressStyle(ViProgressBar::ProgressStyle style)
{
	mUi->progressBar->setProgressStyle(style);
}

void ViLoadingWidget::setText(QString text)
{
	if(mUi->progressBar->text() != "")
	{
		mProcesses.append(QPair<QString, qint64>(mUi->progressBar->text(), mTime.elapsed()));
        mTotalTime += mProcesses.last().second;
		mTime.start();
	}
	mUi->progressBar->setTextStyle(ViProgressBar::Text);
	mUi->progressBar->setText(text);
	if(!isVisible())
	{
		progress(0);
	}
}

void ViLoadingWidget::start()
{
	start(true, false, "", ViProgressBar::None, ViProgressBar::Finite);
}

void ViLoadingWidget::start(bool animation, bool button, QString text, ViProgressBar::TextStyle textStyle, ViProgressBar::ProgressStyle progressStyle)
{
	ViLoadingWidgetPointer instance = ViLoadingWidget::instance();

	instance->mProcesses.clear();
    instance->mTotalTime = 0;

	instance->mTime.start();
	instance->mTimer.start(TIME_UPDATE_INTERVAL);
	instance->show();
	instance->showAnimation(animation);
	instance->showButton(button);
	instance->mUi->progressBar->setText(text); //Important to not call ViLoadingWidget::setText, since this will reset mProcesses
	instance->setTextStyle(textStyle);
	instance->setProgressStyle(progressStyle);
	instance->enableRemainingTime(true);
}

void ViLoadingWidget::stop()
{
	ViLoadingWidgetPointer instance = ViLoadingWidget::instance();
	instance->mTimer.stop();
	instance->mUi->animationWidget->stop();
	instance->hide();

    instance->mProcesses.append(QPair<QString, qint64>(instance->mUi->progressBar->text(), instance->mTime.elapsed()));
    instance->mTotalTime += instance->mProcesses.last().second;

	if(instance->mProcesses.size() > 0)
	{
		QString message = "";
		for(int i = 0; i < instance->mProcesses.size(); ++i)
		{
			message += "\n\t" + instance->mProcesses[i].first + " (Ellapsed time: " + ViTimeConverter::toOptimalString(instance->mProcesses[i].second, ViTimeConverter::Milliseconds) + ")";
		}
        message = "Processes finished (Ellapsed time: " + ViTimeConverter::toOptimalString(instance->mTotalTime, ViTimeConverter::Milliseconds) + ")" + message;
		STATICLOG(message);
		instance->mUi->progressBar->setText("");
		instance->mProcesses.clear();
	}
}

qint64 ViLoadingWidget::remainingTime(qint64 elapsed, qreal percentage)
{
	return (elapsed / percentage) * (100 - percentage);
}

void ViLoadingWidget::enableRemainingTime(bool enable)
{
	if(enable)
	{
		mUi->remainingLabel->show();
		mUi->remainingHeading->show();
		mUi->spacerWidget->show();
	}
	else
	{
		mUi->remainingLabel->hide();
		mUi->remainingHeading->hide();
		mUi->spacerWidget->hide();
	}
}
