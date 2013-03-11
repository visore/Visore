#ifndef VINEURON_H
#define VINEURON_H

#include <vineuralunit.h>
#include <viactivationfunction.h>
#include <visynapse.h>
#include <viid.h>
#include <QRunnable>

class ViNeuronFactory;

class ViNeuron : public ViNeuralUnit, public QRunnable, public ViId
{

	friend class ViNeuronFactory;

	public:

		enum Type
		{
			UnknownNeuron,
			InputNeuron,
			OutputNeuron,
			HiddenNeuron,
			BiasNeuron
		};

	public:

		ViNeuron(const ViNeuron &other);
		virtual ~ViNeuron();

		virtual void run();

		ViNeuron::Type type();
		
		void setType(ViNeuron::Type type);

		double value();

		void setValue(double value);

		ViActivationFunction* activationFunction() const;

		void setActivationFunction(ViActivationFunction *activationFunction);

		bool add(Vi::Direction direction, ViSynapse *synapse);
		bool addInput(ViSynapse *synapse);
		bool addOutput(ViSynapse *synapse);

		bool contains(ViSynapse *synapse) const;
		bool contains(Vi::Direction direction, ViSynapse *synapse) const;
		bool containsInput(ViSynapse *synapse) const;
		bool containsOutput(ViSynapse *synapse) const;

		int inputSize() const;
		int outputSize() const;

		ViSynapse* inputAt(int index) const;
		ViSynapse* outputAt(int index) const;

		bool operator == (const ViNeuron &other) const;

		virtual ViElement exportData();
		virtual bool importData(ViElement element);

		static QString typeToString(ViNeuron::Type type);
		static ViNeuron::Type stringToType(QString type);
		
	protected:

		ViNeuron();
		ViNeuron(ViNeuron::Type type, ViActivationFunction *activationFunction = NULL);

	private:

		ViActivationFunction *mActivationFunction;
		ViSynapseList mInputs;
		ViSynapseList mOutputs;
		double mValue;
		ViNeuron::Type mType;

};

typedef QList<ViNeuron*> ViNeuronList;

#endif
