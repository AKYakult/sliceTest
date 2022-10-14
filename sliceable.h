#ifndef SLICEABLE_H
#define SLICEABLE_H

#include <QImage>
#include <QVector>
#include <QVector3D>
#include <QPainterPath>

class Sliceable
{
public:
    int first_layer, last_layer;

public:
    Sliceable();
    virtual ~Sliceable();
    virtual void dump(QVector<QVector3D> &normal, QVector<QVector3D> &vertex,float f) const = 0;
};

#endif // SLICEABLE_H
