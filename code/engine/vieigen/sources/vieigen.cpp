#include <vieigen.h>
#include <vilogger.h>

bool ViEigenManager::mInitialized = false;
QHash<int, ViEigenBase*> ViEigenManager::mEigens = QHash<int, ViEigenBase*>();

int ViEigenManager::bitPrecision(const int &decimalPrecision)
{
	return ceil(decimalPrecision / log10(2));
}

bool ViEigenManager::isSupported(const int &decimalPrecision)
{
	initialize();
	int bits = bitPrecision(decimalPrecision);
	QList<int> keys = mEigens.keys();
	qSort(keys);
	return keys.last() >= bits;
}

ViEigenBase* ViEigenManager::getByBits(const int &bitPrecision)
{
	initialize();
	QList<int> keys = mEigens.keys();
	qSort(keys);
	for(int i = 0; i < keys.size(); ++i)
	{
		if(bitPrecision <= keys[i])
		{
			//STATICLOG("Using Eigen with a bit precision of " + QString::number(keys[i]), QtDebugMsg, "ViEigenManager");
			return mEigens[keys[i]];
		}
	}
	STATICLOG("The bit precision of " + QString::number(bitPrecision) + " is currently not supported by Visore.", QtFatalMsg, "ViEigenManager");
	exit(-1);
}

ViEigenBase* ViEigenManager::getByDecimals(const int &decimalPrecision)
{
	initialize();
	int bits = bitPrecision(decimalPrecision);
	QList<int> keys = mEigens.keys();
	qSort(keys);
	for(int i = 0; i < keys.size(); ++i)
	{
		if(bits <= keys[i])
		{
			//STATICLOG("Using Eigen with a bit precision of " + QString::number(keys[i]), QtDebugMsg, "ViEigenManager");
			return mEigens[keys[i]];
		}
	}
	STATICLOG("The decimal precision of " + QString::number(decimalPrecision) + " is currently not supported by Visore.", QtFatalMsg, "ViEigenManager");
	exit(-1);
}

void ViEigenManager::initialize()
{
	if(!mInitialized)
	{
		mEigens.insert(24, new ViEigen24());
		mEigens.insert(53, new ViEigen53());
		mEigens.insert(64, new ViEigen64());
		mEigens.insert(72, new ViEigen72());
		mEigens.insert(80, new ViEigen80());
		mEigens.insert(88, new ViEigen88());
		mEigens.insert(96, new ViEigen96());
		mEigens.insert(104, new ViEigen104());
		mEigens.insert(112, new ViEigen112());
		mEigens.insert(120, new ViEigen120());
		mEigens.insert(128, new ViEigen128());
		mEigens.insert(144, new ViEigen144());
		mEigens.insert(160, new ViEigen160());
		mEigens.insert(176, new ViEigen176());
		mEigens.insert(192, new ViEigen192());
		mEigens.insert(208, new ViEigen208());
		mEigens.insert(224, new ViEigen224());
		mEigens.insert(240, new ViEigen240());
		mEigens.insert(256, new ViEigen256());
		mEigens.insert(288, new ViEigen288());
		mEigens.insert(320, new ViEigen320());
		mEigens.insert(352, new ViEigen352());
		mEigens.insert(384, new ViEigen384());
		mEigens.insert(416, new ViEigen416());
		mEigens.insert(448, new ViEigen448());
		mEigens.insert(480, new ViEigen480());
		mEigens.insert(512, new ViEigen512());
		mEigens.insert(640, new ViEigen640());
		mEigens.insert(768, new ViEigen768());
		mEigens.insert(896, new ViEigen896());
		mEigens.insert(1024, new ViEigen1024());

		mInitialized = true;
	}
}
