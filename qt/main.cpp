#include "mainwindow.h"

#include <QApplication>

const extern QString PROGRAM = QString::fromUtf8("/Users/abrahamoyelaran/Projects/sportsx/biscore/blockchain/blockchain");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    MainWindow w;
    w.show();
    return a.exec();
}

//QFile File("stylesheet.qss");
//File.open(QFile::ReadOnly);
//QString StyleSheet = QLatin1String(File.readAll());

//qApp->setStyleSheet(StyleSheet);
