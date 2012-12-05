#ifndef VITYPECHECKER_H
#define VITYPECHECKER_H

#include <QList>

template <typename T = void>
class ViTypeChecker
{

	public:

		static bool check(QObject *object, QString type); // return true if the object is of type
		static bool check(QObject *object); // return true if the object is of type T
		static bool assign(QObject *object, T *assignee); // if object is of type T, let assignee point to it
		static QList<T*> filter(QList<QObject*> objects); // returns a list with the objects that are of type T

};

#include "vitypechecker.cpp"

#endif
