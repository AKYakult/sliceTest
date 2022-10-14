#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <QOpenGLWidget>
#include <QVector>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QTimer>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/opencv.hpp>

#include"model.h"
#include"slicer.h"

using namespace cv ;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    bool readfile(const std::string& pfile);
    Mat fromImage(const QImage& image);
    QImage MatToImage(Mat& m);
    void Check(const Mat& m);
    void flashqueue();
    void clean();
    void Show_Image();
    struct Queue_Image
    {
        QImage data;
        bool isSaved = false;
        int id = -1;
    };
    int index;

private slots:
    void on_ImportButton_clicked();
    void on_horizontalSlider_ImageIndex_valueChanged(int value);
    void on_CalButton_clicked();
    void on_DefineButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow* ui;
    Model* model_temp = nullptr;
    QVector<QByteArray> imagesbyte;
    QVector<struct Queue_Image> queueimages;
    QImage NowImages;
    float f;
};

#endif // MAINWINDOW_H
