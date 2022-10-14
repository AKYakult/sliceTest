#include <QApplication>
#include "mainwindow.h"
int main(int argc, char* argv[])
{
    //test
    QApplication a(argc, argv);
    MainWindow x;
    x.show();
    return a.exec();
}
