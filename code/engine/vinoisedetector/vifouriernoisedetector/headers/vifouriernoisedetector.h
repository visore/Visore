#ifndef VIFOURIERNOISEDETECTOR_H
#define VIFOURIERNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vifouriertransformer.h>

class ViFourierNoiseDetector : public ViNoiseDetector
{

	Q_OBJECT

	protected slots:

		void changeParameter(QString name, qreal value);

    public:

		ViFourierNoiseDetector();
		ViFourierNoiseDetector(const ViFourierNoiseDetector &other);
		~ViFourierNoiseDetector();

		void setWindowSize(const int &size);
		void setStart(const qreal &start); // precentage
		void setEnd(const qreal &end); // precentage
		void setRange(const qreal &start, const qreal &end); // precentage

		bool validParameters();

	protected:

		void detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel = 0);

	private:

		ViFourierTransformer *mTransformer;
		int mWindowSize;
		qreal mStart;
		qreal mEnd;
		int mStartSample;
		int mEndSample;
		int mSampleCount;
		qreal *mInput;
		qreal *mOutput;

};

#endif
