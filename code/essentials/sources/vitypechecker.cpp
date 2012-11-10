#ifdef VITYPECHECKER_H

template <typename T>
bool ViTypeChecker<T>::check(QObject *object)
{
	return dynamic_cast<T*>(object) != 0;
}

template <typename T>
bool ViTypeChecker<T>::assign(QObject *object, T *assignee)
{
	T* result = dynamic_cast<T*>(object);
	if(result != 0)
	{
		assignee = result;
		return true;
	}
	return false;
}

template <typename T>
QList<T*> ViTypeChecker<T>::filter(QList<QObject*> objects)
{
	QList<T*> result;
	for(int i = 0; i < objects.size(); ++i)
	{
		if(dynamic_cast<T*>(objects[i]) != 0)
		{
			result.append(objects[i]);
		}
	}
	return result;
}

#endif
