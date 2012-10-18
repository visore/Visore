#ifndef VIAUDIOOBJECTPOINTER_H
#define VIAUDIOOBJECTPOINTER_H

#include "vipointer.h"
#include <QObject>
/*
class ViAudioObject;

class ViAudioObjectPointer : public QObject, public ViPointer<ViAudioObject>
{

    Q_OBJECT

	signals:

		void unused();

	public:

		ViAudioObjectPointer();
		ViAudioObjectPointer(ViAudioObject *pointer);
		ViAudioObjectPointer(const ViAudioObjectPointer &other);
		virtual ~ViAudioObjectPointer();
		
		void setUnusedLimit(int unusedLimit); // At which reference count the unused() signal should be emitted
		int unusedLimit();

		ViAudioObjectPointer& operator = (const ViAudioObjectPointer &other);

	protected:

		void checkUnused();

	private:

		ViAtomicInt *mLimit;

};
*/
#endif
