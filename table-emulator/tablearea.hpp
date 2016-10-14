#ifndef TABLEAREA_H
#define TABLEAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QByteArray>
#include <QString>
#include <QVector>
#include "tableserver.hpp"

class TableArea : public QWidget
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    TableArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    virtual int heightForWidth(int w) const;
    bool setPort(QString Port);

public slots:
     void setData(QVector<quint32> data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    TableServer ts;
    QVector<quint32> colors;
};

#endif
