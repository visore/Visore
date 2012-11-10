#ifndef VITYPECHECKER_H
#define VITYPECHECKER_H

template <typename T>
class ViTypeChecker
{

	public:

		static bool check(void *object);

};

#include "../sources/vitypechecker.cpp"

#endif
