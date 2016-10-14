#include <QtGui>
#include <iostream>
#include <iomanip>
#include "tablearea.hpp"
#include "constants.h"


TableArea::TableArea(QWidget *parent): QWidget(parent), ts(this), colors(LEDPERLINE*NBLINES+1, 0x0)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

bool TableArea::setPort(QString port_str)
{
    bool ok;
    quint16 port = port_str.toUShort(&ok);
    if(!ok)
    {
        return ok;
    }
    ok = ts.listen(QHostAddress::Any, port);
    return ok;
}

QSize TableArea::minimumSizeHint() const
{
    return QSize(800, heightForWidth(800));
}

void TableArea::setData(QVector<quint32> data)
{
    colors = data;
    update();
}

QSize TableArea::sizeHint() const
{
    return QSize(800, heightForWidth(800));
}

int TableArea::heightForWidth (int w) const
{
    return (w*10)/16;
}

void TableArea::paintEvent(QPaintEvent * /* event */)
{
     QPainter painter(this);
     QBrush brush(Qt::SolidPattern);
     painter.setPen(QPen(Qt::NoPen));
     QSize pixelsize;
     int pixel_width = width()/LEDPERLINE;
     int pixel_height = height()/NBLINES;
     pixelsize.setHeight(pixel_height);
     pixelsize.setWidth(pixel_width);
     QRect rect(QPoint(0, 0), pixelsize);
     painter.setRenderHint(QPainter::Antialiasing, true);
     for(int y = 0; y <  NBLINES; y ++)
     {
         for(int x = 0; x < LEDPERLINE; x ++)
         {
             int p = x+LEDPERLINE*y;
             painter.save();
             brush.setColor(QColor(colors.value(p)));
             painter.setBrush(brush);
             painter.translate(x*pixel_width, y*pixel_height);
             painter.drawRect(rect);
             painter.restore();
         }
     }
}
