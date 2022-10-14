#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "EDLib.h"
#include <iostream>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;


typedef struct threshold
{
    int T_l = 20;
    float T_ratio = 0.001;
    int T_o = 5;// 5 10 15 20 25
    int T_r = 5;// 5 10 15 20 25
    float T_inlier = 0.35;//0.3 0.35 0.4 0.45 0.5 (the larger the more strict)
    float T_angle = 2.0;//
    float T_inlier_closed = 0.5;//0.5,0.6 0.7 0.8,0.9
    float sharp_angle = 60;//35 40 45 50 55 60

} T;

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f = 2.0f;
    clean();
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::clean()
{
    ui->horizontalSlider_ImageIndex->setEnabled(false);
    ui->horizontalSlider_ImageIndex->setRange(0, 0);
    ui->DefineButton->setEnabled(true);
    ui->DefineButton->setEnabled(false);
    ui->CalButton->setEnabled(false);
    ui->SaveButton->setEnabled(false);
    ui->ExportButton->setEnabled(false);
    ui->pushButton->setEnabled(false);
    imagesbyte.clear();
    queueimages.clear();
    queueimages.resize(5);
    QPixmap clear = QPixmap();
    ui->label_2->setPixmap(clear);
    ui->mlabel_1->setPixmap(clear);
    ui->mlabel_2->setPixmap(clear);
    ui->mlabel_3->setPixmap(clear);
    ui->mlabel_4->setPixmap(clear);
    ui->mlabel_5->setPixmap(clear);
    index = 0;
}
void MainWindow::on_ImportButton_clicked()
{
    if(model_temp)
    {
        delete model_temp;
        model_temp = nullptr;
        qDebug() << "delete model_temp";
    }
    model_temp = new Model("C:/Users/yakult/Documents/GreyImage/stl/xxxx_1_1.stl");

    if(model_temp)
    {
        qDebug() << "Import OK" ;
    }

    Show_Image();
}

void MainWindow::Show_Image()
{
    Slicer* slicer = new Slicer();
    if(model_temp)
    {
        for(int i = 0; i < model_temp->meshes.size(); i++)
        {
            Mesh* y = model_temp->meshes.at(i);
            slicer->items.append(y);
        }
        slicer->fillnormalandvertex(f);
        slicer->start();
    }
    else
    {
        return;
    }
    clean();
    imagesbyte = slicer->resultbyte();
    ui->horizontalSlider_ImageIndex->setEnabled(true);
    ui->horizontalSlider_ImageIndex->setRange(0, imagesbyte.size() - 1);
    ui->horizontalSlider_ImageIndex->setValue(imagesbyte.size() / 2);
    ui->DefineButton->setEnabled(true);
}
Mat MainWindow::fromImage(const QImage& image)
{
    switch (image.format())
    {
    case QImage::Format_RGB32:  //8UC4
        return Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
    case QImage::Format_RGB888:   //8UC3
        return Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
    case QImage::Format_Indexed8:   //8UC1
        return Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
    default:
        break;
    }
    return Mat();
}
QImage MainWindow::MatToImage(Mat& m)   //Mat格式转化QImage类型
{
    switch (m.type())      //判断Mat的类型，从而返回不同类型的img
    {
    case CV_8UC1:
    {
        // 通过QImage::QImage(uchar *data, int width, int height, int bytesPerLine,
        // QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr,
        //void *cleanupInfo = nullptr)，主要修改后两个非默认的参数
        QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 1, QImage::Format_Grayscale8);
        return img;
    }
    case CV_8UC3:
    {
        QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 3, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    case CV_8UC4:
    {
        QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 4, QImage::Format_ARGB32);
        return img;
    }
    default:
    {
        QImage img;
        return img;
    }
    }
}
void MainWindow::Check(const Mat& m)
{
    Mat result, gray;
    result = m;
    medianBlur(result, gray, 3);
    cvtColor(result, gray, COLOR_RGB2GRAY);
    std::vector<Vec3f> pcircles;
    HoughCircles(gray, pcircles, HOUGH_GRADIENT, 1, 30, 25, 25,  25*f,  35*f);
    for (size_t i = 0; i < pcircles.size(); i++)
    {
        Vec3f cc = pcircles[i];
        //画圆
        circle(result, Point(cc[0], cc[1]),  cc[2], Scalar(255, 255, 0), 1, LINE_AA);
        qDebug() << "R" << cc[2];
        //画圆心
        circle(result, Point(cc[0], cc[1]), 2, Scalar(255, 255, 255), 1, LINE_AA);
    }
    qDebug() << "----Check----";
}

void MainWindow::flashqueue()
{
    for(int i = 0; i < queueimages.size(); i++)
    {
        if(queueimages.at(i).isSaved == true)
        {
            switch (i)
            {
            case 0:
                ui->mlabel_1->setPixmap(QPixmap::fromImage(queueimages.at(i).data.scaled(ui->mlabel_1->width(), ui->mlabel_1->height(), Qt::KeepAspectRatio)));
                break;
            case 1:
                ui->mlabel_2->setPixmap(QPixmap::fromImage(queueimages.at(i).data.scaled(ui->mlabel_2->width(), ui->mlabel_2->height(), Qt::KeepAspectRatio)));
                break;
            case 2:
                ui->mlabel_3->setPixmap(QPixmap::fromImage(queueimages.at(i).data.scaled(ui->mlabel_3->width(), ui->mlabel_3->height(), Qt::KeepAspectRatio)));
                break;
            case 3:
                ui->mlabel_4->setPixmap(QPixmap::fromImage(queueimages.at(i).data.scaled(ui->mlabel_4->width(), ui->mlabel_4->height(), Qt::KeepAspectRatio)));
                break;
            case 4:
                ui->mlabel_5->setPixmap(QPixmap::fromImage(queueimages.at(i).data.scaled(ui->mlabel_5->width(), ui->mlabel_5->height(), Qt::KeepAspectRatio)));
                break;
            default:
                break;
            }
        }
    }
}
void MainWindow::on_horizontalSlider_ImageIndex_valueChanged(int value)
{
    ui->label_2->setPixmap((QPixmap::fromImage(QImage::fromData(imagesbyte.at(value)))).scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
    QString strtext = QString("%1/%2").arg(value + 1).arg(imagesbyte.size());
    ui->label_layer->setText(strtext);

    NowImages = QImage::fromData(imagesbyte.at(value));
}

void MainWindow::on_CalButton_clicked()
{
    ui->CalButton->setEnabled(false);
    Mat temp;
    temp = fromImage(queueimages.at(index - 1).data);
    Check(temp);

    QImage now;
    now = MatToImage(temp);
    ui->label_2->setPixmap(QPixmap::fromImage(now).scaled(ui->label_2->width(), ui->label_2->height(), Qt::KeepAspectRatio));
    QString paths = QString("C:/Users/yakult/Documents/GreyImage/TEMP/%1.png").arg(index);
    now.save(paths);
    ui->DefineButton->setEnabled(true);
}

void MainWindow::on_DefineButton_clicked()
{
    if(NowImages.isNull())
    {
        qDebug() << "Null"  ;
        return;
    }
    else
    {
        if(index < 5)
        {
            ui->CalButton->setEnabled(true);
            struct Queue_Image X;
            X.data = NowImages.copy();
            X.isSaved = true;
            X.id = ui->horizontalSlider_ImageIndex->value();
            queueimages.replace(index, X);
            index++;
        }
        else
        {
            qDebug() << "------full-------";
        }
    }
    flashqueue();
    ui->DefineButton->setEnabled(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    {
        qDebug() << "------Start Add-------";
        // You should create at least two directories: 'Images1' & 'result'
        // If you have the ground truth, you can create the directory  'GT'
        cv::String path = "C:/Users/yakult/Documents/GreyImage/TEMP/Images/";
        cv::String dst = "C:/Users/yakult/Documents/GreyImage/TEMP/result/";
        vector<cv::String> Filenames;
        T test_threshold;
        cv::glob(path, Filenames);
        for(int i = 0; i < Filenames.size(); i++)
        {
            QString name = Filenames.at(i).data();
            name = QDir::toNativeSeparators(name);
            name.replace("\\", "/");
            Filenames[i] = name.toStdString();
        }
        float fmeasureSum = 0.0;
        float precisionSum = 0.0;
        float recallSum = 0.0;
        float timeSum = 0.0;
        // Detect each image in the directory 'Images1'
        for (int i = 0; i < Filenames.size(); i++)
        {
            //read images
            cv::String file = Filenames[i];
            cv::String::size_type pos1, pos2;
            pos1 = file.find_last_of("/");
            pos2 = file.find(".");
            cv::String prefix = file.substr(pos1, pos2 - pos1);
            cv::String suffix = file.substr(pos2 + 1, pos2 + 3);
            //the name of saved detected images
            cv::String saveName = dst + prefix + "." + suffix;


            Mat testImgOrigin = imread(file, 1);//0:gray 1:color
            Mat testImg = testImgOrigin.clone();
            cvtColor(testImg, testImg, COLOR_BGR2GRAY);
            GaussianBlur(testImg, testImg, Size(9, 9), 2, 2);
            //cv::imshow("Clone Image", testImg);
            //waitKey(1000);
            int height = testImg.rows;
            int width = testImg.cols;
            /*---------Illustration for each step---------*/
            Mat test1 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test2 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test3 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test4 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test5 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test6 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test7 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test8 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test9 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test10 = Mat(height, width, CV_8UC1, Scalar(255));
            Mat test11 = Mat(height, width, CV_8UC1, Scalar(255));
            // EDPF Parameter-free Edge Segment Detection
            clock_t start, finish;
            start = clock();
            EDPF testEDPF = EDPF(testImg);
            Mat edgePFImage = testEDPF.getEdgeImage();
            Mat edge = edgePFImage.clone();
            edge = edge * -1 + 255;
            //cv::imshow("Edge Image Parameter Free", edge);
            //imwrite("D:/astudy/dataset/circle/temp/result/edge.jpg", edge);
            //waitKey(1000);
            vector<vector<Point> >EDPFsegments = testEDPF.getSegments();// get edge segments
            //plot edge images
            cvtColor(test10, test10, COLOR_GRAY2BGR);
            for (int es1 = 0; es1 < EDPFsegments.size(); es1++)
            {
                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;
                Scalar SegEdgesColor = Scalar(b, g, r);
                for (int es2 = 0; es2 < EDPFsegments[es1].size() - 1; es2++)
                {
                    cv::line(test10, EDPFsegments[es1][es2], EDPFsegments[es1][es2 + 1], SegEdgesColor, 2);//Scalar(0, 0, 0)
                }
            }
            //imshow("Edge Segments image", test10);
            //waitKey(1000);
            //imwrite("D:/Test/temp/result/edge_segment.jpg", test10);*/
            /*--------delete edge segments whose pixel number is less than 16-------------*/
            vector<vector<Point>> edgeList;
            for (int i = 0; i < EDPFsegments.size(); i++)
            {
                if (EDPFsegments[i].size() >= 16)// segments should have at least 16 pixels
                {
                    edgeList.push_back(EDPFsegments[i]);
                }//endif
            }//endfor
            /*----------extract closed edges-------------------*/
            closedEdgesExtract* closedAndNotClosedEdges;
            closedAndNotClosedEdges = extractClosedEdges(edgeList);
            vector<vector<Point> > closedEdgeList;
            closedEdgeList = closedAndNotClosedEdges->closedEdges;
            /*--------approximate edge segments using line segments by method RDP-------*/
            vector<vector<Point> > segList;
            for (int s0 = 0; s0 < edgeList.size(); s0++)
            {
                vector<Point> segTemp;
                RamerDouglasPeucker(edgeList[s0], 2.5, segTemp);//3.0
                segList.push_back(segTemp);
            }
            /*-------------reject sharp turn angles---------------*/
            sharpTurn* newSegEdgeList;
            newSegEdgeList = rejectSharpTurn(edgeList, segList, test_threshold.sharp_angle);
            //new seglist and edgelist
            vector<vector<Point>> newSegList = newSegEdgeList->new_segList;
            vector<vector<Point>> newEdgeList = newSegEdgeList->new_edgeList;
            //plot segLists after sharp turn splitting
            cvtColor(test2, test2, COLOR_GRAY2BGR);
            for (int j = 0; j < newSegList.size(); j++)
            {
                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;
                Scalar colorSharpTurn = Scalar(b, g, r);
                for (int jj2 = 0; jj2 < newEdgeList[j].size() - 1; jj2++)
                {
                    //circle(test2, newSegList[j][jj], 1, Scalar(0, 0, 0), 3);
                    line(test2, newEdgeList[j][jj2], newEdgeList[j][jj2 + 1], colorSharpTurn, 2);
                }
            }
            //imshow("After sharp turn", test2);
            //waitKey(1000);
            //imwrite("sharpTurn.jpg", test2);
            /*-----------------Detect inflexion points--------------*/
            InflexionPt* newSegEdgeListAfterInflexion;
            newSegEdgeListAfterInflexion = detectInflexPt(newEdgeList, newSegList);
            // new seglist and edgelist
            vector<vector<Point>> newSegListAfterInflexion = newSegEdgeListAfterInflexion->new_segList;
            vector<vector<Point>> newEdgeListAfterInfexion = newSegEdgeListAfterInflexion->new_edgeList;
            /*--------delete short edgeLists or near line segments----------*/
            vector<vector<Point>>::iterator it = newEdgeListAfterInfexion.begin();
            while (it != newEdgeListAfterInfexion.end())
            {
                /*compute the line segment generated by the two endpoints of the arc,
                    and then judge the midpoint of the arc if lying on or near the line
                    */
                Point edgeSt = Point((*it).front().x, (*it).front().y);
                Point edgeEd = Point((*it).back().x, (*it).back().y);
                int midIndex = (*it).size() / 2;
                Point edgeMid = Point((*it)[midIndex].x, (*it)[midIndex].y);
                double distStEd = sqrt(pow(edgeSt.x - edgeEd.x, 2) + pow(edgeSt.y - edgeEd.y, 2));
                double distStMid = sqrt(pow(edgeSt.x - edgeMid.x, 2) + pow(edgeSt.y - edgeMid.y, 2));
                double distMidEd = sqrt(pow(edgeEd.x - edgeMid.x, 2) + pow(edgeEd.y - edgeMid.y, 2));
                double distDifference = abs((distStMid + distMidEd) - distStEd);
                if ((*it).size() <= test_threshold.T_l || distDifference <= test_threshold.T_ratio * (distStMid + distMidEd))// 2 3 fixed number; (*it).size() <=20
                {
                    it = newEdgeListAfterInfexion.erase(it);
                }
                else
                {
                    it++;
                }
            }//endwhile
            cvtColor(test11, test11, COLOR_GRAY2BGR);
            for (int j = 0; j < newEdgeListAfterInfexion.size(); j++)
            {
                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;
                Scalar colorAfterDeleteLinePt = Scalar(b, g, r);
                for (int jj2 = 0; jj2 < newEdgeListAfterInfexion[j].size() - 1; jj2++)
                {
                    //circle(test2, newSegList[j][jj], 1, Scalar(0, 0, 0), 3);
                    line(test11, newEdgeListAfterInfexion[j][jj2], newEdgeListAfterInfexion[j][jj2 + 1], colorAfterDeleteLinePt, 2);
                }
            }
            //imshow("After short and line segments remove", test11);
            //waitKey(1000);
            //imwrite("D:/Test/temp/result/remove_short_line.jpg", test3);
            /*-----extract closed edgeLists and not closed edgeLists after inflexion point operation------*/
            closedEdgesExtract* closedAndNotClosedEdges1;
            closedAndNotClosedEdges1 = extractClosedEdges(newEdgeListAfterInfexion);
            vector<vector<Point> > closedEdgeList1;
            vector<vector<Point> > notclosedEdgeList1;
            closedEdgeList1 = closedAndNotClosedEdges1->closedEdges;
            notclosedEdgeList1 = closedAndNotClosedEdges1->notClosedEdges;
            //plot closed edgeLists
            cvtColor(test4, test4, COLOR_GRAY2BGR);
            for (int j = 0; j < closedEdgeList1.size(); j++)
            {
                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;
                Scalar colorClosedEdges = Scalar(b, g, r);
                for (int jj = 0; jj < closedEdgeList1[j].size() - 1; jj++)
                {
                    //circle(test4, newSegListAfterInflexion[j][jj], 1, Scalar(0, 0, 0), 3);
                    cv::line(test4, closedEdgeList1[j][jj], closedEdgeList1[j][jj + 1], colorClosedEdges, 2);
                }
                //imshow("After infexion point remove", test2);
                //waitKey();
            }
            //imshow("closedEdges2", test4);
            //waitKey(1000);
            //imwrite("closedEdges2.jpg", test4);
            /*----------sort notclosedEdgeList for grouping-------------*/
            std::vector<std::vector<Point>> sortedEdgeList = sortEdgeList(notclosedEdgeList1);
            /*--------------group sortededgeList---------------*/
            groupArcs* arcs = coCircleGroupArcs(sortedEdgeList, test_threshold.T_o, test_threshold.T_r);
            vector<vector<Point> > groupedArcs = arcs->arcsFromSameCircles;
            vector<vector<Point> > groupedArcsThreePt = arcs->arcsStartMidEnd;
            vector<Vec3f>  groupedOR = arcs->recordOR;
            /*--------circle verification using estimated center and radius parameters*/
            vector<Circles> groupedCircles;// grouped arcs
            groupedCircles = circleEstimateGroupedArcs(groupedArcs, groupedOR, groupedArcsThreePt, test_threshold.T_inlier, test_threshold.T_angle);//fit grouped arcs
            // closed arcs
            for (auto ite = closedEdgeList1.begin(); ite != closedEdgeList1.end(); ite++)
            {
                closedEdgeList.push_back(*ite);
            }//endfor
            vector<Circles> closedCircles;// closedCircles
            closedCircles = circleEstimateClosedArcs(closedEdgeList, test_threshold.T_inlier_closed);// fit closed edges
            //put grouped and closed circles together
            vector<Circles> totalCircles;
            if (!groupedCircles.empty())
            {
                totalCircles = groupedCircles;
            }
            if (!closedCircles.empty())
            {
                for (auto it = closedCircles.begin(); it != closedCircles.end(); it++)
                {
                    totalCircles.push_back(*it);
                }
            }
            //cluster circles----------------->no clusteringT_inlier
            finish = clock();
            vector<Circles> preCircles;
            vector<Circles> preCirclesNew;
            vector<int> flag;
            preCircles = clusterCircles(totalCircles);//-----------------------------------
            qDebug()<<"precircles size"<<preCircles.size();

            int n = preCircles.size();
            int k = 0;
            if(n>2)
            {
                for(int i = 0 ; i < n ; i++)
                {
                    for(int j =i+1 ;j < n ;)
                    {
                        if(abs(preCircles[i].xc-preCircles[j].xc)<=15.0 && abs(preCircles[i].yc-preCircles[j].yc)<=15.0)
                        {

                            preCircles[i].xc = (preCircles[i].xc + preCircles[j].xc)/2.00;
                            preCircles[i].yc = (preCircles[i].yc + preCircles[j].yc)/2.00;
                            preCircles[i].r = (preCircles[i].r + preCircles[j].r)/2.00;
                            preCircles[i].inlierRatio = (preCircles[i].inlierRatio + preCircles[j].inlierRatio)/2.00;

                            for(k=j;k<n-1;k++)
                            {
                                preCircles[k] = preCircles[k+1];
                            }
                            n--;
                            preCircles.pop_back();
                        }
                        else
                        {
                            j++;
                        }
                    }
                }
            }




            //finish = clock();
            timeSum += ((float)(finish - start) / CLOCKS_PER_SEC);
            //draw fit circles after clustering
            Mat detectCircles = drawResult(true, testImgOrigin, saveName, preCircles);//totalCircles preCircles
            //}//endfor    run 100 times and then calculate the average
            /*-----compute precision, recall and fmeasure-------*/
            //pre_rec_fmeasure totalResult = Evaluate(gt, preCircles, 0.8f, testImg);
            //waitKey();
            ////fmeasureSum += totalResult.fmeasure;
            //precisionSum += totalResult.precision;
            //recallSum += totalResult.recall;
        }




        float avePre = precisionSum / Filenames.size();//Filenames.size()
        float aveRec = recallSum / Filenames.size();//Filenames.size()
        float aveTime = timeSum / Filenames.size();
        float aveFmea = 2 * avePre * aveRec / (avePre + aveRec);
        cout << "Pre Rec Fmea Time: " << avePre << " " << aveRec << " " << aveFmea << " " << aveTime << endl;
        //waitKey(1000);
    }
    qDebug() << "---------add circle--------";
}
void MainWindow::on_pushButton_3_clicked()
{
    int i = 0;
    while( i < imagesbyte.size())
    {
        qDebug() << i;
        QImage img = QImage::fromData(imagesbyte.at(i));
        img.save(QString("C:/Users/yakult/Documents/GreyImage/TEMP/Images/%1.png").arg(i + 1));
        i++;
    }
}
