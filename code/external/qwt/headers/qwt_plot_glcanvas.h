/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_GLCANVAS_H
#define QWT_PLOT_GLCANVAS_H

#include "qwt_global.h"
#include <qgl.h>

class QwtPlot;

/*!
  \brief A alternative canvas of a QwtPlot derived from QGLWidget
  
  QwtPlotGLCanvas implements the very basics to work as canvas
  inside of a QwtPlot widget. It might be extended to a full
  featured alternative to QwtPlotCanvas in a future version of Qwt.

  It is intended as start to develop full featured OpenGL canvas
  in future versions of Qwt. Contributions are welcome.

  \sa QwtPlot::setCanvas(), QwtPlotCanvas

  \note You might have to use the QPaintEngine::OpenGL paint engine
        to make antialiasing working: see QGL::setPreferredPaintEngine().

  \note Another way to introduce OpenGL rendering to Qwt
        is to use QGLPixelBuffer or QGLFramebufferObject. Both
        type of buffers can be converted into a QImage and 
        used in combination with a regular QwtPlotCanvas.
*/
class QWT_EXPORT QwtPlotGLCanvas : public QGLWidget
{
    Q_OBJECT

public:
    explicit QwtPlotGLCanvas( QwtPlot * = NULL );
    virtual ~QwtPlotGLCanvas();

    Q_INVOKABLE QPainterPath borderPath( const QRect & ) const;

public Q_SLOTS:
    void replot();

protected:
    virtual void paintEvent( QPaintEvent * );
};

#endif
