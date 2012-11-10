#ifdef VITYPECHECKER_H

template <typename T>
bool ViTypeChecker::check(void *object)
{
	return dynamic_cast<T*>(object) != 0;
}

#endif
