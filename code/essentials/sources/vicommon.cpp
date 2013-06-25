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

template<typename From, typename To>
inline QList<To> viConvertList(QList<From> list)
{
    QList<To> result;
    for(int i = 0; i < list.size(); ++i)
    {
        result.append(list[i]);
    }
    return result;
}

inline QString viTypeToString(ViAudio::Type type)
{
    if(type == ViAudio::Target) return "Target";
    else if(type == ViAudio::Corrupted) return "Corrupted";
    else if(type == ViAudio::Corrected) return "Corrected";
    else return "Undefinied";
}

inline ViAudio::Type viStringToType(QString type)
{
    type = type.toLower().trimmed();
    if(type == "target") return ViAudio::Target;
    else if(type == "corrupted") return ViAudio::Corrupted;
    else if(type == "corrected") return ViAudio::Corrected;
    else return ViAudio::Undefined;
}

QString viBoolToString(bool value, Vi::BoolType type, bool lowerCase)
{
    QString result = "";

    if(type == Vi::YesNo)
    {
        if(value) result = "Yes";
        else result = "No";
    }
    else if(type == Vi::TrueFalse)
    {
        if(value) result = "True";
        else result = "False";
    }
    else if(type == Vi::OneZero)
    {
        if(value) result = "1";
        else result = "0";
    }

    if(lowerCase)
    {
        result = result.toLower();
    }

    return result;
}

bool viStringToBool(QString value)
{
    value = value.toLower().trimmed();
    if(value == "yes" || value == "true" || value == "1")
    {
        return true;
    }
    return false;
}

#endif
