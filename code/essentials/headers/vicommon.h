#ifndef VICOMMON_H
#define VICOMMON_H

#include <QAudio>
#include <QString>
#include <QQueue>
#include <QtGlobal>
#include <QStringList>
#include <math.h>
#include <typeinfo>

#define ROUND(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#define round(x) ROUND(x)

#define CLASSNAME stripClassName(typeid(*this).name()) // call className() from within an object
#define TEMPLATE_CLASSNAME stripTemplateClassName(__PRETTY_FUNCTION__) // call className() from within an object
inline QString stripClassName(const char *text);
inline QString stripTemplateClassName(const char *text);
template <typename Type> inline QString className();

template <typename ForwardIterator>
inline void viDeleteAll(ForwardIterator begin, ForwardIterator end);

template <typename Container>
inline void viDeleteAll(Container &container);

template<typename From, typename To>
inline QList<To> viConvertList(QList<From> list);

namespace Vi
{
    enum BoolType
    {
        YesNo,
        TrueFalse,
        OneZero
    };
}

inline QString viBoolToString(bool value, Vi::BoolType type = Vi::YesNo, bool lowerCase = false);
inline bool viStringToBool(QString value);

typedef unsigned char qbyte8u;
typedef unsigned short qbyte16u;
typedef unsigned int qbyte32u;
typedef qbyte8u qbyteu;

typedef signed char qbyte8s;
typedef signed short qbyte16s;
typedef signed int qbyte32s;
typedef qbyte8s qbytes;

typedef qbyte8u qbyte8;
typedef qbyte16u qbyte16;
typedef qbyte32u qbyte32;
typedef qbyte8 qbyte;

typedef float qfloat;
typedef double qdouble;

typedef qint32 qint;

typedef QList<bool> ViBoolList;
typedef QList<qint> ViIntList;
typedef ViIntList ViIntegerList;
typedef QList<qfloat> ViFloatList;
typedef QList<qreal> ViRealList;
typedef ViRealList ViDoubleList;
typedef QStringList ViStringList;

typedef QList<ViBoolList> ViBoolMatrix;
typedef QList<ViIntList> ViIntMatrix;
typedef ViIntMatrix ViIntegerMatrix;
typedef QList<ViFloatList> ViFloatMatrix;
typedef QList<ViRealList> ViRealMatrix;
typedef ViRealMatrix ViDoubleMatrix;
typedef ViStringList ViStringMatrix;

typedef QQueue<bool> ViBoolQueue;
typedef QQueue<qint> ViIntQueue;
typedef ViIntQueue ViIntegerQueue;
typedef QQueue<qfloat> ViFloatQueue;
typedef QQueue<qreal> ViRealQueue;
typedef ViRealQueue ViDoubleQueue;
typedef QQueue<QString> ViStringQueue;

namespace Vi
{

	enum Direction
	{
		Input,
		Output
	};

}

/*******************************************************************************************************************

    VIAUDIO

*******************************************************************************************************************/

namespace ViAudio
{

	// Values are important for combining types
    enum Type
    {
		Undefined = 0x1,
		Target = 0x2,
		Corrupted = 0x4,
		Corrected = 0x8,
		Temporary = 0x16,
        Correlated = Corrected,
        Temp = Temporary,
        All = Target | Corrupted | Corrected | Temporary
    };

    enum Mode
    {
        AudioInput,
        AudioOutput,
        AudioInputOutput
    };

    enum Input
    {
        Line,
        File,
        Project
    };

    using namespace QAudio;

}

typedef QPair<ViAudio::Type, ViAudio::Type> ViAudioTypePair;

inline QString viTypeToString(ViAudio::Type type);
inline ViAudio::Type viStringToType(QString type);

#include <vicommon.cpp>

#endif
