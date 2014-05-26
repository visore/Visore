#ifndef VIPREDICTOR_H
#define VIPREDICTOR_H

#include <vibuffer.h>
#include <viname.h>
#include <vinotifier.h>
#include <vivector.h>
#include <QVector>

class ViPredictor : public ViName, public ViNotifier
{

	public:

		ViPredictor();
		ViPredictor(const ViPredictor &other);
		~ViPredictor();

		virtual void setWindowSize(const int &size);
		int windowSize();

		void setOffset(const int &offset);
		int offset();

		void predict(ViBuffer *input, ViBuffer *output);
		void predict(ViBuffer *input, ViBuffer *output, const int &predictionCount, qreal *rmse);

		virtual void setParameter(const int &number, const qreal &value);
		void setParameter(const QString &name, const qreal &value);
		void setParameters(const qreal &parameter1);
		void setParameters(const qreal &parameter1, const qreal &parameter2);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4, const qreal &parameter5);

		QString parameterName(const int &index, const bool &allCaps = true);

	protected:

		void addParameterName(const QString &name);
		void adjustValue(qreal &value);

		virtual bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount) = 0;

	protected:

		int mWindowSize;
		int mOffset;
		QList<QString> mParameterNames;

};

class ViModelPredictor : public ViPredictor
{

	public:

		enum Estimation
		{
			Fixed,
			Best
		};

	public:

		ViModelPredictor(const int &degreeCount, const Estimation &estimation = Fixed);
		ViModelPredictor(const ViModelPredictor &other);
		~ViModelPredictor();

		virtual void setDegree(const int &degree, const int &degreeIndex = 0);
		int degree(const int &degreeIndex = 0);

		QVector<int> bestDegrees(const int &degreeIndex = 0);
		int bestDegrees(const int &degree, const int &degreeIndex = 0);

	protected:

		void increaseBestDegrees(const int &degree);
		void increaseBestDegrees(const int &degreeIndex, const int &degree);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

		virtual void predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		virtual void predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

		virtual bool estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		virtual void predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start);
		virtual qreal calculateScore(const qreal *observed, const qreal *predicted, const int &size); // Default uses MSE. Model with the lowest score is best

	private:

		void (ViModelPredictor::*predictPointer)(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

		Estimation mEstimation;
		QVector<int> mDegrees;
		QVector<QVector<int>> mBestDegrees;

};

#endif
