#ifdef VISINGLETONMANAGER_H

template <class T>
ViSingletonManager<T>::~ViSingletonManager()
{
	for(int i = 0; i < mLibraries.size(); ++i)
	{
		if(mLibraries[i] != NULL)
		{
			delete mLibraries[i];
			mLibraries[i] = NULL;
		}
	}
}

template <class T>
void ViSingletonManager<T>::searchPath(QString path)
{
	QList<QString> libraries = ViLibraryDetector::detectLibraries(path);
	for(int i = 0; i < libraries.length(); ++i)
	{			
		ViLibrary<T> *library = new ViLibrary<T>();
		if(library->open(libraries[i]))
		{
			mLibraries.append(library);
			mSingletons.append(library->createObject("create"));
		}
	}
}

template <class T>
QList<T*> ViSingletonManager<T>::all()
{
	QList<T*> result;
	for(int i = 0; i < mSingletons.length(); ++i)
	{
		result.append(mSingletons[i]);
	}
	return result;
}

template <class T>
QList<T*> ViSingletonManager<T>::selected(QList<QString> names)
{
	QList<T*> results;
	for(int i = 0; i < mSingletons.length(); ++i)
	{
		for(int j = 0; j < names.length(); ++j)
		{
			if(names[i] == mSingletons[i]->name())
			{
				results.append(mSingletons[i]);
			}
		}
	}
	return results;
}

template <class T>
T* ViSingletonManager<T>::selected(QString name)
{
	for(int i = 0; i < mSingletons.length(); ++i)
	{
		if(name == mSingletons[i]->name())
		{
			return mSingletons[i];
		}
	}
}

template <class T>
QList<T*>* ViSingletonManager<T>::singletons()
{
	return &mSingletons;
}

#endif
