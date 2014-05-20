#ifndef VIPREDICTOR_H
#define VIPREDICTOR_H

#include <vibuffer.h>
#include <viname.h>

class ViPredictor : public ViName
{

	public:

		ViPredictor();
		ViPredictor(const ViPredictor &other);
		~ViPredictor();

		void setWindowSize(const int &size);
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

		virtual void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount) = 0;
		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal *expectations, qreal *mse);

	protected:

		int mWindowSize;
		int mOffset;
		QList<QString> mParameterNames;

};

class ViDegreePredictor : public ViPredictor
{

	public:

		ViDegreePredictor();
		ViDegreePredictor(const ViDegreePredictor &other);
		~ViDegreePredictor();

		void setDegree(const int &degree);
		int degree();

	protected:

		int mDegree;

};

#endif
