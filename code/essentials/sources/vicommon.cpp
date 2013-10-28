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

template <typename Type>
inline QString className()
{
    return stripClassName(typeid(Type).name());
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

inline QList<ViAudio::Type> ViAudio::types()
{
	return {ViAudio::Target, ViAudio::Corrupted, ViAudio::Corrected, ViAudio::Noise, ViAudio::NoiseMask, ViAudio::Custom, ViAudio::CustomMask};
}

inline QList<ViAudio::Type> ViAudio::types(const ViAudio::Type &type, const ViAudio::Type &exclude)
{
	QList<ViAudio::Type> result;
	if(type & ViAudio::Target && !(exclude & ViAudio::Target))
	{
		result.append(ViAudio::Target);
	}
	if(type & ViAudio::Corrupted && !(exclude & ViAudio::Corrupted))
	{
		result.append(ViAudio::Corrupted);
	}
	if(type & ViAudio::Corrected && !(exclude & ViAudio::Corrected))
	{
		result.append(ViAudio::Corrected);
	}
	if(type & ViAudio::Noise && !(exclude & ViAudio::Noise))
	{
		result.append(ViAudio::Noise);
	}
	if(type & ViAudio::NoiseMask && !(exclude & ViAudio::NoiseMask))
	{
		result.append(ViAudio::NoiseMask);
	}
	if(type & ViAudio::Custom && !(exclude & ViAudio::Custom))
	{
		result.append(ViAudio::Custom);
	}
	if(type & ViAudio::CustomMask && !(exclude & ViAudio::CustomMask))
	{
		result.append(ViAudio::CustomMask);
	}
	return result;
}

inline ViAudio::Type ViAudio::types(const QList<ViAudio::Type> &types, const ViAudio::Type &exclude)
{
	int result = 0;
	for(int i = 0; i < types.size(); ++i)
	{
		if(!(types[i] & exclude))
		{
			result |= types[i];
		}
	}
	return (ViAudio::Type) result;
}

inline QString ViAudio::toString(ViAudio::Type type)
{
	if(type == ViAudio::Target) return "Target";
	else if(type == ViAudio::Corrupted) return "Corrupted";
	else if(type == ViAudio::Corrected) return "Corrected";
	else if(type == ViAudio::Noise) return "Noise";
	else if(type == ViAudio::NoiseMask) return "NoiseMask";
	else if(type == ViAudio::Custom) return "Custom";
	else if(type == ViAudio::CustomMask) return "CustomMask";
	else return "Undefinied";
}

inline ViAudio::Type ViAudio::toType(QString type)
{
	type = type.toLower().trimmed();
	if(type == "target") return ViAudio::Target;
	else if(type == "corrupted") return ViAudio::Corrupted;
	else if(type == "corrected") return ViAudio::Corrected;
	else if(type == "noise") return ViAudio::Noise;
	else if(type == "noisemask") return ViAudio::NoiseMask;
	else if(type == "custom") return ViAudio::Custom;
	else if(type == "custommask") return ViAudio::CustomMask;
	else return ViAudio::Undefined;
}

#endif
