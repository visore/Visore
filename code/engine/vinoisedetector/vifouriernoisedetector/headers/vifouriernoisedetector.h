/*#ifndef VIFOURIERNOISEDETECTOR_H
#define VIFOURIERNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vifouriertransformer.h>

class ViFourierNoiseDetector : public ViNoiseDetector
{

    public:

		ViFourierNoiseDetector();
		ViFourierNoiseDetector(const ViFourierNoiseDetector &other);
		~ViFourierNoiseDetector();
		ViFourierNoiseDetector* clone();

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		ViFourierTransformer *mTransformer;

};

#endif

*/
