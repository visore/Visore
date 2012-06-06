#ifndef VIPROCESSOR_H
#define VIPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include "vichunk.h"

class ViProcessor : public QObject, public QRunnable
{
	Q_OBJECT

	signals:

		void changed();

	public:

		ViProcessor();
		virtual void run() = 0;

};

class ViRawProcessor : public ViProcessor
{

	public:

		ViRawProcessor();
		void setData(ViChunk<char> *data);
		virtual void run() = 0;

	protected:

		ViChunk<char> *mData;

};

class ViRealProcessor : public ViProcessor
{

	public:

		ViRealProcessor();
		void setData(ViChunk<double> *data);
		virtual void run() = 0;

	protected:

		ViChunk<double> *mData;

};

#endif
