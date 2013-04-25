#include <vimainwindow.h>
#include <ui_vimainwindow.h>
#include <vistackedwidget.h>
#include <vimainmenu.h>
#include <viloadingwidget.h>

#include <vineuralnetworkwidget.h>
#include <vineuralnetworkfactory.h>
#include <viactivationfunctionmanager.h>
#include <viweightinitializermanager.h>
#include <vitrainermanager.h>
#include <vifixedweightinitializer.h>
#include <visigmoidactivationfunction.h>


ViMainWindow* ViMainWindow::mWindow = NULL;

ViMainWindow::ViMainWindow()
	: QMainWindow()
{
}

ViMainWindow::~ViMainWindow()
{
	delete mUi;
}

ViMainWindow* ViMainWindow::instance()
{
	if(mWindow == NULL)
	{
		mWindow = new ViMainWindow();
		mWindow->initialize();
	}
	return mWindow;
}

void ViMainWindow::show()
{
	if(ViManager::isWindowFullScreen())
	{
		showFullScreen();
	}
	else if(ViManager::isWindowMaximized())
	{
		showMaximized();
	}
	else
	{
		resize(ViManager::windowSize());
		move(ViManager::windowPosition());
		QMainWindow::show();
	}
}

void ViMainWindow::resizeEvent(QResizeEvent *event)
{
	ViLoadingWidget::instance()->resize(event->size());
    event->accept();
	ViManager::setWindowSize(event->size());
}

void ViMainWindow::moveEvent(QMoveEvent *event)
{
	ViManager::setWindowPosition(event->pos());
}

void ViMainWindow::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::WindowStateChange)
	{
		ViManager::setWindowFullScreen(isFullScreen());
		ViManager::setWindowMaximized(isMaximized());
	}
}

void ViMainWindow::initialize()
{
	mUi = new Ui::ViMainWindow();
	mUi->setupUi(this);
	mEngine = ViAudioEngine::instance();

	setStyleSheet("QWidget#centralWidget{background-image: url(" + ViThemeManager::image("tile").path() + ") repeat-x;}");

	mUi->stack->layout()->addWidget(ViStackedWidget::widget());

	int index = ViStackedWidget::addWidget(new ViMainMenu());
	ViStackedWidget::setCurrentIndex(index);
	mUi->logoButton->addFunctionCall(SIGNAL(clicked()), ViFunctionCall(ViStackedWidget::instance().data(), "setCurrentIndex", index));

	mUi->logoButton->setSize(64, 64);
	mUi->logoButton->setIcon("logo", 64);
	mUi->logoButton->disableBackground();
	mUi->logoButton->setGlow(ViGradientCreator::Circle);

	ViFont font = ViThemeManager::font(ViThemeFonts::MainFont);
	font.setPointSize(30);
	font.setLetterSpacing(QFont::PercentageSpacing, 105);
	mUi->logoLabel->setFont(font);
	mUi->logoLabel->setStyleSheet("color: " + ViThemeManager::color(ViThemeColors::MainColor6).name());













ViNeuralNetworkWidget * nnWidget = new ViNeuralNetworkWidget();
index = ViStackedWidget::addWidget(nnWidget);
	ViStackedWidget::setCurrentIndex(index);

ViNeuralNetworkFactory mFactory;

	mFactory.setActivationFunction(ViActivationFunctionManager::create("ViSigmoidActivationFunction"));
	mFactory.addLayer(3);
	mFactory.addLayer(2);
	mFactory.addLayer(1);
	ViNeuralNetwork *network = mFactory.create();

	network->setInputs({1,0,1});



	ViTrainer *trainer = ViTrainerManager::createDefault();
	//ViWeightInitializer *weightInitializer = ViWeightInitializerManager::createDefault();
	ViFixedWeightInitializer *weightInitializer = (ViFixedWeightInitializer*) ViWeightInitializerManager::create("Fixed");

	weightInitializer->setValues({0.5,0.4,0.3,0.2,0.1,0.2,0.3,0.4});
	weightInitializer->initialize(network, trainer->learningRate());

network->run();
nnWidget->setNeuralNetwork(network);
trainer->trainSingle(network);

cout<<"hidden 1: "<<network->at(1)->at(0)->value()<<endl;
cout<<"hidden 2: "<<network->at(1)->at(1)->value()<<endl;
cout<<"output: "<<network->at(2)->at(0)->value()<<endl;

	int c = 0;
	for(int i = 0; i < network->size()-1; ++i)
	{
		for(int j = 0; j < network->at(i)->size(); ++j)
		{
			for(int k = 0; k < network->at(i)->at(j)->outputSize(); ++k)
			{
				cout << "w" << ++c << ": " << network->at(i)->at(j)->outputAt(k)->weight() << endl;
			}
		}
	}

	trainer->trainSingle(network);
	c = 0;
	for(int i = 0; i < network->size()-1; ++i)
	{
		for(int j = 0; j < network->at(i)->size(); ++j)
		{
			for(int k = 0; k < network->at(i)->at(j)->outputSize(); ++k)
			{
				cout << "w" << ++c << ": " << network->at(i)->at(j)->outputAt(k)->weight() << endl;
			}
		}
	}





}
