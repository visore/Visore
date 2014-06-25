#ifndef VINOISEDETECTOR_H
#define VINOISEDETECTOR_H

#include <vinotifier.h>
#include <viname.h>
#include <vibuffer.h>
#include <viclassificationerror.h>
#include <QVector>

class ViNoiseDetector : public ViNotifier, public ViName
{

	public:

		ViNoiseDetector();
		ViNoiseDetector(const ViNoiseDetector &other);
		virtual ~ViNoiseDetector();

		qreal detect(ViBuffer *corrupted, ViBuffer *noiseMap); // Returns the maximum noise value, therefore in [0, max]

		void mask(ViBuffer *noiseMap, ViBuffer *noiseMask, const qreal &threshold);

		ViClassificationErrorCollection error(ViBuffer *noiseMask, ViBuffer *sizeMask);
		ViClassificationErrorCollection error(ViBuffer *noiseMap, ViBuffer *sizeMask, const qreal &threshold);

		virtual bool validParameters() const;

		void setParameter(const QString &name, const qreal &value);
		void setParameter(const int &index, const qreal &value);
		bool hasParameter(const QString &name) const;
		int parameterCount() const;
		QStringList parameters(const bool &allCaps = true) const;
		QString parameter(const int &index, const bool &allCaps = true) const;

		qreal unscaleThreshold(const qreal &threshold);

	protected:

		qreal parameter(const QString &name);
		void addParameter(const QString &name);

		void setOffset(const int &offset);
		void setScale(const qreal &scale);

		virtual void initialize(); // Use this to set all the parameters
		virtual void detect(QVector<qreal> &samples, QVector<qreal> &noise) = 0; // returns the number of samples to remove.

	private:

		int mOffset;
		qreal mScale;
		QMap<QString, qreal> mParameters;

};

#endif
