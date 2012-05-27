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
		void enabled(bool enabled);

	public:

		enum Type
		{
			Observer = 0,
			Modifier = 1
		};

		ViProcessor(ViProcessor::Type type);
		ViProcessor::Type type();
		void enable(bool enable = true);
		bool isEnabled();
		virtual void run() = 0;

	private:

		ViProcessor::Type mType;
		bool mIsEnabled;

};

class ViRawProcessor : public ViProcessor
{

	public:

		ViRawProcessor(ViProcessor::Type type);
		void setData(ViChunk<char> *data);

	protected:

		ViChunk<char> *mData;

};

class ViRealProcessor : public ViProcessor
{

	public:

		ViRealProcessor(ViProcessor::Type type);
		void setData(ViChunk<double> *data);

	protected:

		ViChunk<double> *mData;

};

#endif
