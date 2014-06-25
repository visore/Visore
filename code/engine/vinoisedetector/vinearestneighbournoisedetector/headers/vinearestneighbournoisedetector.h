#ifndef VINEARESTNEIGHBOURNOISEDETECTOR_H
#define VINEARESTNEIGHBOURNOISEDETECTOR_H

#include <vinoisedetector.h>

// ftp://cs.uku.fi/franti/papers/Hautamaki/P2.pdf
// http://www.ijcttjournal.org/Volume3/issue-2/IJCTT-V3I2P119.pdf
// http://www.researchgate.net/profile/Markus_Goldstein/publication/230856452_Nearest-Neighbor_and_Clustering_based_Anomaly_Detection_Algorithms_for_RapidMiner/file/9fcfd50574518b8df5.pdf
// http://www.dfki.de/web/forschung/publikationen/renameFileForDownload?filename=slides.pdf&file_id=uploads_1635

class ViNearestNeighbourNoiseDetector : public ViNoiseDetector
{

    public:

		ViNearestNeighbourNoiseDetector();
		ViNearestNeighbourNoiseDetector(const ViNearestNeighbourNoiseDetector &other);

		void setK(const int &k); // k nearest neighbours

		bool validParameters();

	protected:

		void initialize();
		void detect(QVector<qreal> &samples, QVector<qreal> &noise);

	private:

		int mK;
		int mHalfK;

};

#endif
