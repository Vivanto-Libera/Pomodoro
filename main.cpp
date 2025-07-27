#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>
QTranslator trans;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/fonts/Huxiaobo.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Weilaiyuan.ttf");
    trans.load("localization/Pomodoro_zh_CN.qm");
    a.installTranslator(&trans);
    MainWindow w;
    w.show();
    return a.exec();
}
