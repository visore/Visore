#ifndef VIPREDICTOR_H
#define VIPREDICTOR_H

#include <vibuffer.h>
#include <viname.h>
#include <vinotifier.h>
#include <vierror.h>
#include <QVector>

class ViPredictor : public ViName, public ViNotifier
{

	public:

		ViPredictor();
		ViPredictor(const ViPredictor &other);
		virtual ~ViPredictor();

		virtual void setWindowSize(const int &size);
		int windowSize();

		void setOffset(const int &offset);
		int offset();

		void predict(ViBuffer *input, ViBuffer *output, ViError *modelError = NULL);
		void predict(ViBuffer *input, ViBuffer *output, const int &predictionCount, ViError *modelError = NULL);

		void predict(ViBuffer *input, ViBuffer *output, ViErrorCollection *predictionErrors, ViError *modelError = NULL);
		void predict(ViBuffer *input, ViBuffer *output, const int &predictionCount, ViErrorCollection *predictionErrors, ViError *modelError = NULL);

		bool predict(qreal *samples, const int &size, qreal &prediction, const int &channel = 0);
		virtual bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL, const int &channel = 0) = 0;

		virtual void setParameter(const int &number, const qreal &value);
		void setParameter(const QString &name, const qreal &value);
		void setParameters(const qreal &parameter1);
		void setParameters(const qreal &parameter1, const qreal &parameter2);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4);
		void setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4, const qreal &parameter5);

		virtual bool validParameters();
		virtual bool hasParameter(const QString &name);

		QString parameterName(const int &index, const bool &allCaps = true);
		QStringList parameters();

		virtual void initialize(const int &channelCount, const int &predictionCount); // Called before the first prediction

	protected:

		void addParameterName(const QString &name);
		void adjustValue(qreal &value);

		void setPredictionIndex(const int &index); // From all the n predicted samples, which one to use for the output

	protected:

		int mWindowSize;
		int mOffset;
		int mPredictionIndex;
		QList<QString> mParameterNames;

};

#endif
