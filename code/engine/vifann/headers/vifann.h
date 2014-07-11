#ifndef VIFANN_H
#define VIFANN_H

#include <QList>
#include <QVector>
#include <doublefann.h> // FANN uses doubles
//#include <floatfann.h> // FANN uses floats

class ViFann;

class ViFannTrain
{

	friend class ViFann;

	public:

		~ViFannTrain();

		bool setInput(const int &index, const qreal *data);
		bool setOutput(const int &index, const qreal *data);

	protected:

		ViFannTrain(const int &dataCount, const int &inputs, const int &outputs);
		ViFannTrain(const ViFannTrain &other);

		fann_train_data* data();
		bool isSame(const int &dataCount, const int &inputs, const int &outputs);

	private:

		fann_train_data *mData;

};

class ViFann
{

	public:

		enum Type
		{
			NoType,
			Standard,
			Sparse,
			Shortcut
		};

		enum Weights
		{
			NoWeights,
			Random,
			WidrowNguyen
		};

		enum Activation
		{
			NoActivation,
			Sigmoid,		// Symetric sigmoid (-1,1)
			Elliot,			// Symetric David Elliot (-1,1)
			Gaussian,		// Symetric Gaussian (-1,1)
			Linear,			// Symetric bounded linear (-1,1)
			Cosine,			// Symetric periodical cosinus [-1,1]
			Sine,			// Symetric periodical sinus [-1,1]
		};

		enum Training
		{
			Fixed,
			Cascade
		};

		enum Algorithm
		{
			Incremental,
			Batch,
			RProp,
			QuickProp
		};

	public:

		ViFann();
		ViFann(const ViFann &other);
		~ViFann();

		void clear();

		// Initializes the network structure
		// A list containing the number of neurons on each layer
		// Connection rate only for sparse networks (1 = fully connected, 0 = no connections)
		bool setStructure(const Type &type, const QList<int> &neurons, const qreal &connectionRate = 0.5);

		// Initializes the network inputs
		bool setInputCount(const int &count);

		// Initializes the network outputs
		bool setOutputCount(const int &count);

		// Initialize the weights
		// Minimum and maximum will only be used if the initialization is random
		bool setWeights(const Weights &initialization, const qreal &minimum = -0.1, const qreal &maximum = 0.1);

		// Set the activation function
		// Can be different for every layer
		bool setActivation(const Activation &activation); // Sets for all layers
		bool setActivation(const QList<Activation> &activations); // For each layer
		bool setActivation(const Activation &activation, const int &layer);

		// In [0,1]
		// Default in FANN: 0.7 (typical at 0.3 with momentum at 0.9)
		// Training parameter that controls the size of weight and bias changes during learning
		void setLearningRate(const qreal &rate);

		// In [0,1]
		// Default in FANN: 0 (typical at 0.9 with learning rate at 0.3)
		// Momentum simply adds a fraction m of the previous weight update to the current one.
		// The momentum parameter is used to prevent the system from converging to a local minimum or saddle point.
		// A high momentum parameter can also help to increase the speed of convergence of the system.
		// However, setting the momentum parameter too high can create a risk of overshooting the minimum, which can cause the system to become unstable.
		// A momentum coefficient that is too low cannot reliably avoid local minima, and can also slow down the training of the system.
		void setLearningMomentum(const qreal &momentum);

		// Set the type of training and the training algorithm
		void setTraining(const Training &training, const Algorithm &algorithm);
		void setTraining(const Training &training);
		void setTraining(const Algorithm &algorithm);

		// Stopping conditions for training
		void setStopEpochs(const int &maxEpochs); // Only applies to fixed training
		void setStopNeurons(const int &maxNeurons); // Only applies to cascade training
		void setStopMse(const qreal &mse);
		void setStopStagnation(const qreal &fraction, const int &iterations); // Will stop training if the MSE of "iterations" sequnetial epochs is lower than "fraction". "fraction" is in [0,1]. Eg: "fraction" of 0.01 is equal to 1% change in MSE.

		// Create a training set
		ViFannTrain* createTrain(const int &dataCount);
		bool setTrainInput(const int &index, const qreal *data);
		bool setTrainOutput(const int &index, const qreal *data);

		// Run the input through the network and generate the output
		// Does not train the newtork
		// Input array should at least have the same size than the number of input neurons, same with output array
		void run(const qreal *input, qreal *output);
		void run(const qreal *input, qreal &output); // For a single output neuron
		void run(const qreal *input, qreal *output, const int &interations); // Generates 1 output at a time, feeds the output into the input and generates the next output (for interations outputs)

		// Train the network with a single interation, using the input and desired output
		// Input array should at least have the same size than the number of input neurons, same with output array
		void train(const qreal *input, const qreal *desiredOutput);
		void train(const qreal *input, const qreal &desiredOutput);

		// Train for a number of iterations or until a certain MSE was reached
		void train(const bool &debug = false);

		// First run the input, and then train it
		void runTrain(const qreal *input, qreal *output, const qreal *desiredOutput);
		void runTrain(const qreal *input, qreal &output, const qreal &desiredOutput);

		// Checks if all the settings and paramters work together
		bool isValid();

		// Getters

		Type type();
		int inputCount();
		int outputCount();
		int layerCount();
		int neuronCount(const int &layer);
		QList<int> neurons();
		qreal connectionRate();

		Weights weights();
		qreal weightsMinimum();
		qreal weightsMaximum();

		qreal mse();
		fann* network();

	protected:

		inline void adjustSamples(qreal *samples, const int &size);
		inline void adjustSample(qreal &sample);

	private:

		// Other
		int mI;

		// Network
		fann *mNetwork;

		//Structure
		Type mType;
		int mInputCount;
		int mOutputCount;
		QList<int> mNeurons;
		qreal mConnectionRate;

		// Weights
		Weights mWeights;
		qreal mWeightsMinimum;
		qreal mWeightsMaximum;

		// Train
		ViFannTrain *mTrain;
		Training mTraining;
		Algorithm mAlgorithm;
		int mTrainEpochs;
		int mTrainNeurons;
		qreal mTrainMse;
		qreal mTrainStagnationFraction;
		int mTrainStagnationIterations;

};

#endif
