#ifndef VIFANN_H
#define VIFANN_H

#include <QList>
#include <doublefann.h> // FANN uses doubles
//#include <floatfann.h> // FANN uses floats

class ViFann
{

	public:

		enum Type
		{
			Standard,
			Sparse,
			Shortcut
		};

		enum WeightInitialization
		{
			Random,
			WidrowNguyen
		};

	public:

		ViFann();
		ViFann(const ViFann &other);
		~ViFann();

		void clear();

		// A list containing the number of neurons on each layer
		// Connection rate only for sparse networks (1 = fully connected, 0 = no connections)
		void create(const Type &type, const QList<int> &neurons, const qreal &connectionRate = 0.5);


	private:

		fann *mNetwork;

};

#endif
