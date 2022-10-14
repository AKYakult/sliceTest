#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include"sliceable.h"

struct Vertex
{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
    QVector3D Tangent;
    QVector3D Bitangent;
};
class Mesh : public Sliceable
{
public:
    /*  网格数据  */
    QVector<Vertex> vertices;               //顶点数据
    QVector<unsigned int> indices;          //索引数组
    QMatrix4x4 model;                       //模型矩阵

    /*  函数  */

    Mesh(aiMatrix4x4 model);
    void dump(QVector<QVector3D>& normal, QVector<QVector3D>& vertex,float f) const override;

private:



};

#endif // MESH_H
