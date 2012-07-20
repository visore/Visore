/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_DIAL_H
#define QWT_DIAL_H 1

#include "qwt_global.h"
#include "qwt_abstract_slider.h"
#include "qwt_abstract_scale_draw.h"
#include <qframe.h>
#include <qpalette.h>

class QwtDialNeedle;
class QwtRoundScaleDraw;

/*!
  \brief QwtDial class provides a rounded range control.

  QwtDial is intended as base class for dial widgets like
  speedometers, compass widgets, clocks ...

  \image html dials2.png

  A dial contains a scale and a needle indicating the current value
  of the dial. Depending on Mode one of them is fixed and the
  other is rotating. If not isReadOnly() the
  dial can be rotated by dragging the mouse or using keyboard inputs
  (see keyPressEvent()). A dial might be wrapping, what means
  a rotation below/above one limit continues on the other limit (f.e compass).
  The scale might cover any arc of the dial, its values are related to
  the origin() of the dial.

  Qwt is missing a set of good looking needles (QwtDialNeedle).
  Contributions are very welcome.

  \sa QwtCompass, QwtAnalogClock, QwtDialNeedle
  \note The examples/dials example shows different types of dials.
*/

class QWT_EXPORT QwtDial: public QwtAbstractSlider
{
    Q_OBJECT

    Q_ENUMS( Shadow )
    Q_ENUMS( Mode )
    Q_ENUMS( Direction )

    Q_PROPERTY( int lineWidth READ lineWidth WRITE setLineWidth )
    Q_PROPERTY( Shadow frameShadow READ frameShadow WRITE setFrameShadow )
    Q_PROPERTY( Mode mode READ mode WRITE setMode )
    Q_PROPERTY( double origin READ origin WRITE setOrigin )
    Q_PROPERTY( Direction direction READ direction WRITE setDirection )

    friend class QwtDialScaleDraw;
public:

    /*!
        \brief Frame shadow

         Unfortunately it is not possible to use QFrame::Shadow
         as a property of a widget that is not derived from QFrame.
         The following enum is made for the designer only. It is safe
         to use QFrame::Shadow instead.
     */
    enum Shadow
    {
        //! QFrame::Plain
        Plain = QFrame::Plain,

        //! QFrame::Raised
        Raised = QFrame::Raised,

        //! QFrame::Sunken
        Sunken = QFrame::Sunken
    };

    //! Mode controlling wether the needle or the scale is rotating
    enum Mode
    {
        //! The needle is rotating
        RotateNeedle,

        //! The needle is fixed, the scales are rotating
        RotateScale
    };

    //! Direction of the dial
    enum Direction
    {
        //! Clockwise
        Clockwise,

        //! Counter clockwise
        CounterClockwise
    };

    explicit QwtDial( QWidget *parent = NULL );
    virtual ~QwtDial();

    void setFrameShadow( Shadow );
    Shadow frameShadow() const;

    void setLineWidth( int );
    int lineWidth() const;

    void setMode( Mode );
    Mode mode() const;

    void setScaleArc( double min, double max );

    double minScaleArc() const;
    double maxScaleArc() const;

    virtual void setOrigin( double );
    double origin() const;

    void setDirection( Direction );
    Direction direction() const;

    void setNeedle( QwtDialNeedle * );
    const QwtDialNeedle *needle() const;
    QwtDialNeedle *needle();

    QRectF boundingRect() const;
    QRectF innerRect() const;

    virtual QRectF scaleInnerRect() const;

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    void setScaleDraw( QwtRoundScaleDraw * );

    QwtRoundScaleDraw *scaleDraw();
    const QwtRoundScaleDraw *scaleDraw() const;

protected:
    virtual void mousePressEvent( QMouseEvent * );
    virtual void wheelEvent( QWheelEvent * );
    virtual void paintEvent( QPaintEvent * );

    virtual void drawFrame( QPainter *p );
    virtual void drawContents( QPainter * ) const;
    virtual void drawFocusIndicator( QPainter * ) const;

    virtual void drawScale( 
        QPainter *, const QPointF &center,
        double radius, double origin, 
        double arcMin, double arcMax ) const;

    /*!
      Draw the contents inside the scale

      Paints nothing.

      \param painter Painter
      \param center Center of the contents circle
      \param radius Radius of the contents circle
    */
    virtual void drawScaleContents( QPainter *painter, 
        const QPointF &center, double radius ) const;

    virtual void drawNeedle( QPainter *, const QPointF &,
        double radius, double direction, QPalette::ColorGroup ) const;

    virtual void scaleChange();

    virtual double valueAt( const QPoint & );
    virtual bool isScrollPosition( const QPoint & ) const;

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif
