#include "mesh.h"
#include <QtOpenGLExtensions/QOpenGLExtensions>
Mesh::Mesh(aiMatrix4x4 model)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            this->model(i, j) = model[i][j];
        }
    }
}



void Mesh::dump(QVector<QVector3D>& normal, QVector<QVector3D>& vertex, float f) const
{
    normal.clear();
    vertex.clear();
    for(int i = 0; i < this->vertices.size(); i++)
    {
        float p1,p2,p3;
        p1=vertices.at(i).Position.x()*f;
        p2=vertices.at(i).Position.y()*f;
        p3=vertices.at(i).Position.z()*f;
        vertex.append(QVector3D(p1,p2,p3));
        if(i % 3 == 0)
        {
            normal.append(vertices.at(i).Normal);
        }
    }
}
