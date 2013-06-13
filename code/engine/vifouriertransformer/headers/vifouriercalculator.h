#ifndef VIFOURIERCALCULATOR_H
#define VIFOURIERCALCULATOR_H

class ViFourierCalculator
{

	public:
		
        ViFourierCalculator();
        virtual ~ViFourierCalculator();

		virtual void setData(const double input[], double output[]);
		virtual void setData(double input[]);
		virtual void setSize(const int size);
		virtual int size() const;

		virtual void forward() = 0;
		virtual void inverse() = 0;
		virtual void rescale() = 0;

	protected:

		const double *mInput;
		double *mOutput;
		int mSize;

};

#endif
