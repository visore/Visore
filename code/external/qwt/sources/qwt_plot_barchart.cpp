/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_plot_barchart.h"
#include "qwt_scale_map.h"
#include "qwt_column_symbol.h"
#include "qwt_painter.h"
#include <qpainter.h>

class QwtPlotBarChart::PrivateData
{
public:
    PrivateData():
        symbol( NULL ),
        legendMode( QwtPlotBarChart::LegendChartTitle )
    {
    }
 
    ~PrivateData()
    {
        delete symbol;
    }

    QwtColumnSymbol *symbol;
    QwtPlotBarChart::LegendMode legendMode;
};

QwtPlotBarChart::QwtPlotBarChart( const QwtText &title ):
    QwtPlotAbstractBarChart( title )
{
    init();
}

QwtPlotBarChart::QwtPlotBarChart( const QString &title ):
    QwtPlotAbstractBarChart( QwtText( title ) )
{
    init();
}

QwtPlotBarChart::~QwtPlotBarChart()
{
    delete d_data;
}

void QwtPlotBarChart::init()
{
    d_data = new PrivateData;
    setData( new QwtPointSeriesData() );
}

//! \return QwtPlotItem::Rtti_PlotBarChart
int QwtPlotBarChart::rtti() const
{
    return QwtPlotItem::Rtti_PlotBarChart;
}

void QwtPlotBarChart::setSamples(
    const QVector<QPointF> &samples )
{
    setData( new QwtPointSeriesData( samples ) );
}

void QwtPlotBarChart::setSamples(
    const QVector<double> &samples )
{
    QVector<QPointF> points;
    for ( int i = 0; i < samples.size(); i++ )
        points += QPointF( i, samples[ i ] );

    setData( new QwtPointSeriesData( points ) );
}

void QwtPlotBarChart::setSymbol( QwtColumnSymbol *symbol )
{
    if ( symbol != d_data->symbol )
    {
        delete d_data->symbol;
        d_data->symbol = symbol;

        legendChanged();
        itemChanged();
    }
}

/*!
  \return Current symbol or NULL, when no symbol has been assigned
  \sa setSymbol()
*/
const QwtColumnSymbol *QwtPlotBarChart::symbol() const
{
    return d_data->symbol;
}

void QwtPlotBarChart::setLegendMode( LegendMode mode )
{
    if ( mode != d_data->legendMode )
    {
        d_data->legendMode = mode;
        legendChanged();
    }
}

QwtPlotBarChart::LegendMode QwtPlotBarChart::legendMode() const
{
    return d_data->legendMode;
}

/*!
  \return Bounding rectangle of all samples.
  For an empty series the rectangle is invalid.
*/
QRectF QwtPlotBarChart::boundingRect() const
{
    const size_t numSamples = dataSize();
    if ( numSamples == 0 )
        return QwtPlotSeriesItem::boundingRect();

    const double baseLine = baseline();

    QRectF rect = QwtPlotSeriesItem::boundingRect();
    if ( rect.bottom() < baseLine )
        rect.setBottom( baseLine );
    if ( rect.top() > baseLine )
        rect.setTop( baseLine );

    if ( rect.isValid() && ( orientation() == Qt::Horizontal ) )
        rect.setRect( rect.y(), rect.x(), rect.height(), rect.width() );

    return rect;
}

/*!
  Draw an interval of the bar chart

  \param painter Painter
  \param xMap Maps x-values into pixel coordinates.
  \param yMap Maps y-values into pixel coordinates.
  \param canvasRect Contents rect of the canvas
  \param from Index of the first point to be painted
  \param to Index of the last point to be painted. If to < 0 the
         curve will be painted to its last point.

  \sa drawSymbols()
*/
void QwtPlotBarChart::drawSeries( QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRectF &canvasRect, int from, int to ) const
{
    if ( to < 0 )
        to = dataSize() - 1;

    if ( from < 0 )
        from = 0;

    if ( from > to )
        return;


    const QRectF br = data()->boundingRect();
    const QwtInterval interval( br.left(), br.right() );

    painter->save();

    for ( int i = from; i <= to; i++ )
    {
        drawSample( painter, xMap, yMap,
                    canvasRect, interval, i, sample( i ) );
    }

    painter->restore();
}

/*!
  Draw a sample

  \param painter Painter
  \param xMap x map
  \param yMap y map
  \param canvasRect Contents rect of the canvas
  \param boundingInterval Bounding interval of sample values
  \param from Index of the first point to be painted
  \param to Index of the last point to be painted

  \sa drawSeries()
*/
void QwtPlotBarChart::drawSample( QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRectF &canvasRect, const QwtInterval &boundingInterval,
    int index, const QPointF &point ) const
{
    QwtColumnRect barRect;

    if ( orientation() == Qt::Horizontal )
    {
        const double barHeight = sampleWidth( yMap, canvasRect.height(),
            boundingInterval.width(), point.y() );

        const double x1 = xMap.transform( baseline() );
        const double x2 = xMap.transform( point.y() );

        const double y = yMap.transform( point.x() );
        const double y1 = y - 0.5 * barHeight;
        const double y2 = y + 0.5 * barHeight;

        barRect.direction = ( x1 < x2 ) ?
            QwtColumnRect::LeftToRight : QwtColumnRect::RightToLeft;

        barRect.hInterval = QwtInterval( x1, x2 ).normalized();
        barRect.vInterval = QwtInterval( y1, y2 );
    }
    else
    {
        const double barWidth = sampleWidth( xMap, canvasRect.width(),
            boundingInterval.width(), point.y() );

        const double x = xMap.transform( point.x() );
        const double x1 = x - 0.5 * barWidth;
        const double x2 = x + 0.5 * barWidth;

        const double y1 = yMap.transform( baseline() );
        const double y2 = yMap.transform( point.y() );

        barRect.direction = ( y1 < y2 ) ?
            QwtColumnRect::TopToBottom : QwtColumnRect::BottomToTop;

        barRect.hInterval = QwtInterval( x1, x2 );
        barRect.vInterval = QwtInterval( y1, y2 ).normalized();
    }

    drawBar( painter, index, point, barRect );
}

void QwtPlotBarChart::drawBar( QPainter *painter,
    int sampleIndex, const QPointF &point, 
    const QwtColumnRect &rect ) const
{
    const QwtColumnSymbol *specialSym = 
        specialSymbol( sampleIndex, point );

    const QwtColumnSymbol *sym = specialSym;
    if ( sym == NULL )
        sym = d_data->symbol;

    if ( sym )
    {
        sym->draw( painter, rect );
    }
    else
    {
        // we build a temporary default symbol
        QwtColumnSymbol sym( QwtColumnSymbol::Box );
        sym.setLineWidth( 1 );
        sym.setFrameStyle( QwtColumnSymbol::Plain );
        sym.draw( painter, rect );
    }

    delete specialSym;
}

QwtColumnSymbol *QwtPlotBarChart::specialSymbol( 
    int index, const QPointF &point ) const
{
    Q_UNUSED( index );
    Q_UNUSED( point );

    return NULL;
}

QwtText QwtPlotBarChart::barTitle( int sampleIndex ) const
{
    Q_UNUSED( sampleIndex );
    return QwtText();
}

QwtText QwtPlotBarChart::label( 
    int sampleIndex, const QPointF& point ) const
{
    Q_UNUSED( sampleIndex );
    Q_UNUSED( point );

    return QwtText();
}

QList<QwtLegendData> QwtPlotBarChart::legendData() const
{
    QList<QwtLegendData> list;

    if ( d_data->legendMode == LegendBarTitles )
    {
        const size_t numSamples = dataSize();
        for ( size_t i = 0; i < numSamples; i++ )
        {
            QwtLegendData data;

            QVariant titleValue;
            qVariantSetValue( titleValue, barTitle( i ) );
            data.setValue( QwtLegendData::TitleRole, titleValue );

            if ( !legendIconSize().isEmpty() )
            {
                QVariant iconValue;
                qVariantSetValue( iconValue,
                    legendIcon( i, legendIconSize() ) );

                data.setValue( QwtLegendData::IconRole, iconValue );
            }

            list += data;
        }
    }
    else
    {
        return QwtPlotAbstractBarChart::legendData();
    }

    return list;
}

QwtGraphic QwtPlotBarChart::legendIcon( 
    int index, const QSizeF &size ) const
{
    QwtColumnRect column;
    column.hInterval = QwtInterval( 0.0, size.width() - 1.0 );
    column.vInterval = QwtInterval( 0.0, size.height() - 1.0 );

    QwtGraphic icon;
    icon.setDefaultSize( size );
    icon.setRenderHint( QwtGraphic::RenderPensUnscaled, true );

    QPainter painter( &icon );
    painter.setRenderHint( QPainter::Antialiasing,
        testRenderHint( QwtPlotItem::RenderAntialiased ) );

    int barIndex = -1;
    if ( d_data->legendMode == QwtPlotBarChart::LegendBarTitles )
        barIndex = index;
        
    drawBar( &painter, barIndex, QPointF(), column );

    return icon;
}

