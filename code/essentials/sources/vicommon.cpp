#ifdef VICOMMON_H

inline QString stripClassName(const char *text)
{
	QString name(text);
	if(name.startsWith("P") && name[1].isDigit())
	{
		name.remove(0, 1);
	}
	while(name.size() > 0 && name[0].isDigit())
	{
		name.remove(0, 1);
	}
	return name;
}

inline QString stripTemplateClassName(const char *text)
{
	QString name(text);
	int startIndex = name.indexOf("[with T = ");
	if(startIndex == -1)
	{
		return "";
	}
	startIndex += 10;
	return name.mid(startIndex, name.indexOf("]") - startIndex);
}

template <typename ForwardIterator>
inline void viDeleteAll(ForwardIterator begin, ForwardIterator end)
{
	while(begin != end)
	{
		if(*begin != NULL)
		{
			delete *begin;
			*begin = NULL;
		}
		++begin;
	}
}

template <typename Container>
inline void viDeleteAll(Container &container)
{
    viDeleteAll(container.begin(), container.end());
	container.clear();
}

#endif
