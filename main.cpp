#include "mainwindow.h"
#include <QApplication>
//#include <QTextCodec>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("plastique");
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    MainWindow w;
    w.show();
    
    return a.exec();
}
