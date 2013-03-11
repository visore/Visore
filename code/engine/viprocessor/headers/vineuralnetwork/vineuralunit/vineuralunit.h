#ifndef VINEURALUNIT_H
#define VINEURALUNIT_H

#include <vicommon.h>
#include <vilogger.h>
#include <viserializer.h>
#include <QList>

class ViNeuralUnit : public ViSerializer
{

	public:

		ViNeuralUnit();
		ViNeuralUnit(const ViNeuralUnit &other);
		virtual ~ViNeuralUnit();

};

#endif
