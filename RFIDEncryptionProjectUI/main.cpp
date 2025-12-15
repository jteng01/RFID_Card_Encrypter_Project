#include "mainwindow.h"
#include "serial.h"
#include "reader.h"
#include "encryptor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
