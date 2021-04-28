#include "mainwindow.h"
#include <QMultimedia>
#include <QApplication>
#include <al.h>
#include <alc.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
