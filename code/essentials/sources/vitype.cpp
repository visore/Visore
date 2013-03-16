#ifdef VITYPE_H

template <typename T>
ViType<T>::ViType()
{
	static int id = qRegisterMetaType<T>(TEMPLATE_CLASSNAME.toLatin1().data());
}

#endif
