#ifdef VISIZECONVERTER_H

template <typename T1, typename T2>
int ViSizeConverter<T1, T2>::convert(int size)
{
	return size * (sizeof(T2) / float(sizeof(T1)));
}

#endif
