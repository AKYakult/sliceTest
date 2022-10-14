#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <QDir>

class Model
{
public:

    void destroy();
    static Model* createModel(QString path);

public:

    Model(QString path);
    ~Model();


    /*  模型数据  */
    QVector<Mesh*> meshes;                  //网格
    QDir directory;                         //模型所在路径

    //递归遍历结点
    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 mat4 = aiMatrix4x4());

    //加载网格
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 model);

};
#endif // MODEL_H
