#ifndef SLICER_H
#define SLICER_H

#include <QList>
#include <QThread>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QAtomicInt>
#include <QPainterPath>
#include <vector3.h>
#include <QImage>


class Sliceable;

class Slicer : public QThread
{
    Q_OBJECT

private:
    struct MLayer
    {
        float z;
        QList<int> index;
        QByteArray data;
        bool isEmpty = false;
    };
    struct Line
    {
        QVector2D p1, p2;
        bool connected;
    };
    int max_layer;
    QVector<struct MLayer> queue;
    QVector<QVector3D> normal, vertex;

    QVector<QByteArray> images;
public:
    float thickness = 1.0f;
    QSize resolution;
    QVector<Sliceable*> items;
public:
    explicit Slicer();
    ~Slicer();
    QVector<QByteArray> resultbyte();
    void fillnormalandvertex(float f);//因为vbo无法在多线程下读取，所以只能主线程来调用
protected:
    void run() override;
private:
    void clean();
    void cleanAll();
    void groupMLayers(Sliceable* item);
    void dispatchMLayers();
    void processMLayer(struct MLayer& layer);
    QVector2D intersect(QVector3D v1, QVector3D v2, float z);
};

#endif // SLICER_H
