/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_OVERLAY_H
#define QWT_PLOT_OVERLAY_H

#include <qwidget.h>
#include <qregion.h>

class QPainter;

/*!
   \brief An overlay widget for the plot canvas

   In combination with the backingstore of the plot canvas
   overlay widgets avoid expensive replots. Their main use case
   is to support graphical interactions where it is unaccapteble, 
   when the updates of the canvas is not in time with the mouse 
   movements.

   QwtPlotOverlay is an abstract base class. Deriving classes are
   supposed to reimplement the following methods:

   - drawOverlay()
   - maskHint()

   \sa QwtPlotCanvas::BackingStore
 */
class QwtPlotOverlay: public QWidget
{
public:
    /*!
       \brief Mask mode

       When using masks the canvas gets paint events for
       the masked regions of the overlay only. Otherwise
       Qt triggers full repaints. On less powerful hardware
       ( f.e embedded systems ) - or when using the raster paint 
       engine on a remote desktop - bit blitting is a noticeable
       operation, that needs to be avoided.
       
       If and how to mask depends on how expensive the calculation 
       of the mask is and how many pixels can be excluded by the mask.

       The default setting is MaskHint.

       \sa setMaskMode(), maskMode()
     */
    enum MaskMode
    {
        //! Don't use a mask.
        NoMask,

        /*!
           \brief Use maskHint() as mask

           For many situations a fast approximation is good enough 
           and it is not necessary to build a more detailed mask
           ( f.e the bounding rectangle of a text ).
         */
        MaskHint,

        /*!
           \brief Calculate a mask by checking the alpha values

           Sometimes it is not possible to give a fast approximation
           and the mask needs to be calculated by drawing the overlay
           and testing the result.
          
           When a valid maskHint() is available
           only pixels inside this approximation are checked.
         */
        AlphaMask
    };

    /*!
       \brief Render mode

       For calculating the alpha mask the overlay has already
       been painted to a temporary QImage. Instead of rendering
       the overlay twice this buffer can be copied for drawing
       the overlay.

       On graphic systems using the raster paint engine ( QWS, Windows )
       it means usually copying some memory only. On X11 it results in an
       expensive operation building a pixmap and for simple overlays
       it might not be recommended.

       \note The render mode has no effect, when maskMode() != AlphaMask.
     */
    enum RenderMode
    {
        //! Copy the buffer, when using the raster paint engine.
        AutoRenderMode,

        //! Always copy the buffer
        CopyAlphaMask,

        //! Never copy the buffer
        DrawOverlay
    };

    QwtPlotOverlay( QWidget* canvas );
    virtual ~QwtPlotOverlay();

    void setMaskMode( MaskMode );
    MaskMode maskMode() const;

    void setRenderMode( RenderMode );
    RenderMode renderMode() const;

    void updateOverlay();

    virtual bool eventFilter( QObject *, QEvent *);

protected:
    virtual void paintEvent( QPaintEvent* event );
    virtual void resizeEvent( QResizeEvent* event );

    virtual void drawOverlay( QPainter * ) const = 0;
    virtual QRegion maskHint() const;

private:
    void updateMask();
    void draw( QPainter * ) const;

private:
    class PrivateData;
    PrivateData *d_data;

    uchar* m_rgbaBuffer;
};

#endif
