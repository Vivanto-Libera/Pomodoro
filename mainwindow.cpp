#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(QFile::exists("data.bin"))
        init();
    aTimer= new QTimer(this);
    aTimer->setTimerType(Qt::CoarseTimer);
    aTimer->setInterval(500);
    aTimer->setSingleShot(false);
    aTimer->start();
    connect(aTimer,SIGNAL(timeout()),this,SLOT(setCurTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    listVisible=true;

}

void MainWindow::setCurTime()
{
    QTime curTime= QTime::currentTime();
    QString str=curTime.toString("hh:mm:ss");
    ui->lab_curTime->setText(str);
}
