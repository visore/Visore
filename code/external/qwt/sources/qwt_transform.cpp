/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_transform.h"
#include "qwt_math.h"

#if QT_VERSION < 0x040601
#define qExp(x) ::exp(x)
#endif

//! Smallest allowed value for logarithmic scales: 1.0e-150
QT_STATIC_CONST_IMPL double QwtLogTransform::LogMin = 1.0e-150;

//! Largest allowed value for logarithmic scales: 1.0e150
QT_STATIC_CONST_IMPL double QwtLogTransform::LogMax = 1.0e150;

QwtTransform::QwtTransform()
{
}

QwtTransform::~QwtTransform()
{
}

double QwtTransform::bounded( double value ) const
{
    return value;
}

QwtNullTransform::QwtNullTransform():
    QwtTransform()
{
}

QwtNullTransform::~QwtNullTransform()
{
}


double QwtNullTransform::transform( double value ) const
{
    return value;
}

double QwtNullTransform::invTransform( double value ) const
{
    return value;
}


QwtTransform *QwtNullTransform::copy() const
{
    return new QwtNullTransform();
}

QwtLogTransform::QwtLogTransform():
    QwtTransform()
{
}

QwtLogTransform::~QwtLogTransform()
{
}

double QwtLogTransform::transform( double value ) const
{
    return ::log( value );
}

double QwtLogTransform::invTransform( double value ) const
{
    return qExp( value );
}

double QwtLogTransform::bounded( double value ) const
{
    return qBound( LogMin, value, LogMax );
}

QwtTransform *QwtLogTransform::copy() const
{
    return new QwtLogTransform();
}

QwtPowerTransform::QwtPowerTransform( double factor ):
    QwtTransform(),
    d_factor( factor )
{
}

QwtPowerTransform::~QwtPowerTransform()
{
}

double QwtPowerTransform::transform( double value ) const
{
    return qPow( value, 1.0 / d_factor );
}

double QwtPowerTransform::invTransform( double value ) const
{
    return qPow( value, d_factor );
}

QwtTransform *QwtPowerTransform::copy() const
{
    return new QwtPowerTransform( d_factor );
}
