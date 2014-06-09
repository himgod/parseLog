#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(600,550);
    w.show();
    return a.exec();
}
