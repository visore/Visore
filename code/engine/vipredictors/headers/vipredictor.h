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

	protected:

		void addParameterName(const QString &name);
		void adjustValue(qreal &value);

		virtual bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL) = 0;

	protected:

		int mWindowSize;
		int mOffset;
		QList<QString> mParameterNames;

};

#endif
