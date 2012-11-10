#ifndef VICOMMON_H
#define VICOMMON_H

#include <QString>
#include <QtGlobal>
#include <QStringList>
#include <math.h>
#include <typeinfo>

#define ROUND(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#define round(x) ROUND(x)

#define CLASSNAME stripClassName(typeid(*this).name()) // call className() from within an object
QString stripClassName(const char *text);

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

typedef QList<bool> QBoolList;
typedef QList<qint> QIntList;
typedef QList<qfloat> QFloatList;
typedef QList<qreal> QRealList;

#endif
