/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_ABSTRACT_SLIDER_H
#define QWT_ABSTRACT_SLIDER_H

#include "qwt_global.h"
#include "qwt_abstract_scale.h"
#include "qwt_scale_map.h"

/*!
  \brief An abstract base class for slider widgets

  QwtAbstractSlider is a base class for slider widgets. 
  It handles mouse events and updates the slider's value accordingly. 
  Derived classes only have to implement the valueAt() and
  getScrollMode() members, and should react to a
  valueChange(), which normally requires repainting.
*/

class QWT_EXPORT QwtAbstractSlider: public QwtAbstractScale
{
    Q_OBJECT

    Q_PROPERTY( double value READ value WRITE setValue )
    Q_PROPERTY( double minimum READ minimum WRITE setMinimum )
    Q_PROPERTY( double maximum READ maximum WRITE setMaximum )

    Q_PROPERTY( double singleStep READ singleStep WRITE setSingleStep )
    Q_PROPERTY( int pageStepCount READ pageStepCount WRITE setPageStepCount )
    Q_PROPERTY( bool stepAlignment READ stepAlignment WRITE setStepAlignment )

    Q_PROPERTY( bool readOnly READ isReadOnly WRITE setReadOnly )
    Q_PROPERTY( bool tracking READ isTracking WRITE setTracking )
    Q_PROPERTY( bool wrapping READ wrapping WRITE setWrapping )

	Q_PROPERTY( double scaleValue READ scaleValue WRITE setScaleValue )

public:
    explicit QwtAbstractSlider( QWidget *parent = NULL );
    virtual ~QwtAbstractSlider();

    void setValid( bool );
    bool isValid() const;

    double value() const;
    double scaleValue() const;

    void setWrapping( bool tf );
    bool wrapping() const;

    void setSingleStep( double );
    double singleStep() const;

    void setPageStepCount( int );
    int pageStepCount() const;

    void setStepAlignment( bool on ); 
    bool stepAlignment() const;

    void setRange( double vmin, double vmax );

    void setMinimum( double min );
    double minimum() const;

    void setMaximum( double max );
    double maximum() const;

    void setTracking( bool enable );
    bool isTracking() const;

    void setReadOnly( bool );
    bool isReadOnly() const;

public Q_SLOTS:
    void setValue( double val );
    void setScaleValue( double value );

Q_SIGNALS:

   /*!
      \brief Notify a change of value translated into 
             scale coordinates.

      \param value New value
    */
    void scaleValueChanged( double value );

    /*!
      \brief Notify a change of value.

      In the default setting
      (tracking enabled), this signal will be emitted every
      time the value changes ( see setTracking() ).

      \param value New value
    */
    void valueChanged( double value );

    /*!
      This signal is emitted when the user presses the
      movable part of the slider (start ScrMouse Mode).
    */
    void sliderPressed();

    /*!
      This signal is emitted when the user releases the
      movable part of the slider.
    */

    void sliderReleased();
    /*!
      This signal is emitted when the user moves the
      slider with the mouse.
      \param value new value
    */
    void sliderMoved( double value );

protected:
    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseReleaseEvent( QMouseEvent * );
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void keyPressEvent( QKeyEvent * );
    virtual void wheelEvent( QWheelEvent * );

    /*!
      \brief Determine the value corresponding to a specified poind

      This is an abstract virtual function which is called when
      the user presses or releases a mouse button or moves the
      mouse. It has to be implemented by the derived class.
      \param p point
    */
    virtual double valueAt( const QPoint & ) = 0;

    /*!
      \brief Determine what to do when the user presses a mouse button.

      This function is abstract and has to be implemented by derived classes.
      It is called on a mousePress event. The derived class can determine
      what should happen next in dependence of the position where the mouse
      was pressed by returning scrolling mode and direction. 

      \param pos point where the mouse was pressed
      \retval scrollMode The scrolling mode
    */
    virtual bool isScrollPosition( const QPoint &pos ) const = 0;

    void setMouseOffset( double );
    double mouseOffset() const;

    virtual void rangeChange();

    void incrementValue( double increment );

    QwtScaleMap sliderMap() const;

private Q_SLOTS:
    void emitScaleValue();

private:
    double alignedValue( double ) const;
    double boundedValue( double ) const;

    class PrivateData;
    PrivateData *d_data;
};

#endif
