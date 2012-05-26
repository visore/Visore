#ifndef VIWINDOWFUNCTION_H
#define VIWINDOWFUNCTION_H

#include <QList>
#include <QStringList>
#include <QVector>
#include "math.h"
#include <qmath.h>

/**********************************************************
ViWindowFunction
**********************************************************/

template <typename T>
class ViWindowFunction
{

	typedef QVector<T> ViWindow;

	public:

		void apply(T *data, int size);
		void create(int size);

	protected:

		void fillWindow(int size);
		virtual T calculate(int currentSample, int totalSamples) = 0;

	protected:

		ViWindow mWindow;

};

/**********************************************************
ViWindowFunctionManager
**********************************************************/
template <typename T>
class ViWindowFunctionManager
{

	public:

		static ViWindowFunction<T>* createFunction(QString functionName);
		static QStringList functions();

};

/**********************************************************
ViRectangularFunction
**********************************************************/

template <typename T>
class ViRectangularFunction : public ViWindowFunction<T>
{

	protected:

		T calculate(int currentSample, int totalSamples);

};

/**********************************************************
ViHammingFunction
**********************************************************/

template <typename T>
class ViHammingFunction : public ViWindowFunction<T>
{

	protected:

		T calculate(int currentSample, int totalSamples);

};

/**********************************************************
ViHannFunction
**********************************************************/

template <typename T>
class ViHannFunction : public ViWindowFunction<T>
{

	protected:

		T calculate(int currentSample, int totalSamples);

};

#endif
