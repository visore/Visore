#ifndef VISTATISTICIAN_H
#define VISTATISTICIAN_H

#include <viaudioobject.h>
#include <vinoisedetector.h>
#include <QTime>

class ViStatistician : public QObject
{

	Q_OBJECT

	public:

		enum Mode
		{
			Mean,
			StandardDeviation,	// Corrected sample standard deviation
			Pearson				// Sample Pearson coefficient
		};

	private slots:

		void quit();

		void nextFile();
		void process();

		void addDir(QString dirName);

	public:

		ViStatistician();
		virtual ~ViStatistician();

		void run(QString folder = "");

	protected:

		qreal mean(ViSampleChunk &data);
		qreal standardDeviation(ViSampleChunk &data);
		qreal pearson(ViSampleChunk &data, qreal &theTop, qreal &theBottom1, qreal &theBottom2);

		void printFileData(double value);
		void printFileDataPearson(double value1, double value2);
		void printFileFinal();
		void printFileFinalPearson();
		void printTerminal(double value);
		void printTerminalPearson(double value1, double value2);

	private:

		Mode mMode;
		int mWindowSize;
		QQueue<QString> mFiles;
		QString mCurrentFile;
		ViAudioObjectPointer mCurrentObject;
		QString mFile;
		qreal mValue;
		qint64 mCount;

		qreal mValuePos, mValueNeg;
		qint64 mCountPos, mCountNeg;

};

#endif
