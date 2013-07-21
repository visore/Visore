#ifdef VILIBRARYMANAGER_H

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::ViLibraryManager()
{
	mWasLoaded = false;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::~ViLibraryManager()
{
	unload();
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
LIBRARY_TYPE* ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::create(QString name, QString functionName)
{
	ViLibraryManagerPointer manager = ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::instance();
	if(!manager->mWasLoaded)
	{
		manager->mWasLoaded = true;
		manager->load(manager->libraryPath(), functionName);
	}

    LIBRARY_TYPE *library = NULL;
	QStringList possibleNames;

    QString newName = name.toLower().trimmed().replace(" ", "");
    possibleNames.append(newName);

	QString postfix = manager->trailer().toLower();
    QString prefix = "vi";
    bool hasPrefix = newName.startsWith(prefix);
	bool hasPostfix = newName.endsWith(postfix);

    if(hasPrefix && !hasPostfix)
	{
		possibleNames.append(newName.remove(0, 2) + postfix);
	}
	else if(hasPrefix)
	{
		possibleNames.append(newName.remove(0, 2));
	}
    else if(!hasPostfix)
    {
        possibleNames.append(newName + postfix);
    }
    else if(!hasPrefix)
    {
        possibleNames.append(prefix + newName);
    }
	for(int i = 0; i < possibleNames.size(); ++i)
    {
		library = manager->createLibrary(possibleNames[i]);
		if(library != NULL)
		{
			break;
		}
	}

	if(library == NULL)
	{
		STATICLOG("The specified library (" + name + ") could not be found.", QtCriticalMsg);
	}
	return library;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
LIBRARY_TYPE* ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::create(ViElement element, QString functionName)
{
	if(element.name() != "Name")
	{
		if(element.child("Name").isNull())
		{
			return NULL;
		}
		return create(element.child("Name").toString(), functionName); 
	}
	return create(element.toString(), functionName);
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
LIBRARY_TYPE* ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::createDefault(QString functionName)
{
	return create(ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::instance()->defaultLibrary(), functionName);
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
QList<LIBRARY_TYPE*> ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::createAll(QString functionName)
{
	ViLibraryManagerPointer manager = ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::instance();
	if(!manager->mWasLoaded)
	{
		manager->mWasLoaded = true;
		manager->load(manager->libraryPath(), functionName);
	}
	QList<LIBRARY_TYPE*> result;
	for(int i = 0; i < manager->mLibraries.size(); ++i)
	{
		result.append((LIBRARY_TYPE*) manager->mLibraries[i]->clone());
	}
	return result;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
LIBRARY_TYPE* ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::createLibrary(QString name)
{
    name = name.toLower();
	for(int i = 0; i < mLibraries.size(); ++i)
    {
		if(mLibraries[i]->name().toLower() == name)
		{
			return (LIBRARY_TYPE*) mLibraries[i]->clone();
		}
	}
	return NULL;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
QStringList ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::names(QString replace, bool spaced, QString functionName)
{
    ViLibraryManagerPointer manager = ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::instance();
    if(!manager->mWasLoaded)
    {
        manager->mWasLoaded = true;
        manager->load(manager->libraryPath(), functionName);
    }
	QStringList result;
    for(int i = 0; i < manager->mLibraries.size(); ++i)
	{
        result.append(manager->mLibraries[i]->name(replace, spaced));
	}
	return result;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
QString ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::defaultName(QString replace, bool spaced)
{
    LIBRARY_TYPE *library = createDefault();
    QString name = library->name(replace, spaced);
    delete library;
    return name;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
int ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::load(QString directory, QString functionName)
{
    int loadCount = 0;
	QStringList libraries = find(directory);
	for(int i = 0; i < libraries.size(); ++i)
	{
        if(!contains(libraries[i]))
        {
			QLibrary *library = new QLibrary(libraries[i]);
			if(library->load())
			{
				++loadCount;
                mLibraryFiles.append(library);
			}
		}
	}
    typedef LIBRARY_TYPE* (*FunctionPointer)();
	for(int i = 0; i < mLibraryFiles.size(); ++i)
	{
		QFunctionPointer pointer = mLibraryFiles[i]->resolve(functionName.toLatin1().data());
        if(pointer == NULL)
        {
            LOG("The library (" + QFileInfo(mLibraryFiles[i]->fileName()).baseName() + ") does not have a " + functionName + "() function.", QtFatalMsg);
		}
        else
        {
            mLibraries.append(((FunctionPointer) pointer)());
        }
	}
	return loadCount;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
void ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::unload()
{
	for(int i = 0; i < mLibraryFiles.size(); ++i)
	{
		mLibraryFiles[i]->unload();
	}
	viDeleteAll(mLibraryFiles);
	viDeleteAll(mLibraries);
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
bool ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::contains(QLibrary *library)
{
	return contains(library->fileName());
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
bool ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::contains(QString libraryPath)
{
	libraryPath = libraryPath.trimmed();
	for(int i = 0; i < mLibraryFiles.size(); ++i)
	{
		if(mLibraryFiles[i]->fileName() == libraryPath)
		{
			return true;
		}
	}
	return false;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
QStringList ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::skipLibraries()
{
    QStringList result;
    result.append(className<LIBRARY_TYPE>());
    return result;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
QStringList ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::find(QString directory)
{
	QDir dir(directory);
	QStringList libraries;
    QFileInfoList allFiles = dir.entryInfoList(QDir::Files);
    QStringList skipNames = skipLibraries();
    QString name, path;
    bool skip;

	for(int i = 0; i < allFiles.size(); ++i)
    {
        skip = false;
        path = allFiles[i].absoluteFilePath();
        name = allFiles[i].baseName();

        for(int j = 0; j < skipNames.size(); ++j)
        {
            if(name.contains(skipNames[j], Qt::CaseInsensitive))
            {
                skip = true;
                break;
            }
        }

        if(!skip && QLibrary::isLibrary(path))
        {
            libraries.append(path);
		}
	}

	return libraries;
}

template<typename MANAGER_TYPE, typename LIBRARY_TYPE>
QString ViLibraryManager<MANAGER_TYPE, LIBRARY_TYPE>::trailer()
{
	if(mLibraries.isEmpty())
	{
		return "";
	}
	QString name = mLibraries[0]->name();
	for(int i = name.size() - 1; i >= 0; --i)
	{
		if(name[i].isUpper())
		{
			return name.right(name.size() - i);
		}
	}
	return "";
}

#endif
